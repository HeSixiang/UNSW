/*

 ******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */

#include <string.h>
#include <regex.h>

#include "utils/hashutils.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

/*  
PersonName ::= Family','Given | Family', 'Given

^[A-Z]([A-Z]|[a-z]|[-]|['])+([ ][A-Z]([A-Z]|[a-z]|[-]|['])+)*,
[ ]?[A-Z]([A-Z]|[a-z]|[-]|['])+([ ][A-Z]([A-Z]|[a-z]|[-]|['])+)*$

Family     ::= NameList
Given      ::= NameList

NameList   ::= Name | Name' 'NameList     [A-Z]([A-Z]|[a-z]|[-]|['])+([ ][A-Z]([A-Z]|[a-z]|[-]|['])+)*

Name       ::= Upper Letters              [A-Z]([A-Z]|[a-z]|[-]|['])+

Letter     ::= Upper | Lower | Punc       ([A-Z]|[a-z]|[-]|['])

Letters    ::= Letter | Letter Letters    ([A-Z]|[a-z]|[-]|['])+

Upper      ::= 'A' | 'B' | ... | 'Z'      [A-Z]
Lower      ::= 'a' | 'b' | ... | 'z'      [a-z]
Punc       ::= '-' | "'"                  [-']
*/
static int is_person(const char * c) {
    regex_t regex;
    int result;
    const char * pattern = "^[A-Z]([A-Z]|[a-z]|[-]|['])+([ ][A-Z]([A-Z]|[a-z]|[-]|['])+)*,"
                            "[ ]?[A-Z]([A-Z]|[a-z]|[-]|['])+([ ][A-Z]([A-Z]|[a-z]|[-]|['])+)*$";
    result = regcomp(&regex, pattern, REG_EXTENDED);
    result = regexec(&regex, c, 0, NULL, 0);
    regfree(&regex);
    return result;
}

typedef struct PersonName
{
	int32 length;
	char  name[FLEXIBLE_ARRAY_MEMBER];
}	PersonName;


static int is_person(const char * c);
static int pname_cmp_internal(PersonName * a, PersonName * b);

/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_in);

Datum
pname_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	int name_len, family_len, given_len;
	char * given;
	PersonName * destination;
	if (is_person(str) != 0) {
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type PersonName")
				));
	}
	//dealing with the possible space after ','
	name_len = strlen(str);
	given = strchr(str, ',');
	given++;
	family_len = name_len - strlen(given);
	if (given[0] == ' ') {
		name_len = name_len - 1;
		given++;
	}
	given_len = strlen(given);
	destination = (PersonName *) palloc(VARHDRSZ + sizeof(char) * (name_len + 1));
	SET_VARSIZE(destination, VARHDRSZ + sizeof(char) * (name_len + 1));

	//put the value in 
	memcpy(destination->name, str, family_len);
	memcpy(destination->name+family_len, given, given_len+1);

	PG_RETURN_POINTER(destination);
}

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
	PersonName    *pname = (PersonName *) PG_GETARG_POINTER(0);
	char	   *result;

	result = psprintf("%s", pname->name);
	PG_RETURN_CSTRING(result);
}


/*****************************************************************************
 * Operator class for defining B-tree index
 *
 * It's essential that the comparison operators and support function for a
 * B-tree index opclass always agree on the relative ordering of any two
 * data values.  Experience has shown that it's depressingly easy to write
 * unintentionally inconsistent functions.  One way to reduce the odds of
 * making a mistake is to make all the functions simple wrappers around
 * an internal three-way-comparison function, as we do here.
 *****************************************************************************/

static int
pname_cmp_internal(PersonName * a, PersonName * b)
{
	char* a_given;
	char* b_given;
	int result;
	char* a_name = a->name;
	char* b_name = b->name;
	a_given = strchr(a_name, ',');
	b_given = strchr(b_name, ',');
	a_given[0] = '\0';
	b_given[0] = '\0';
	result = strcmp(a_name, b_name);
	a_given[0] = ',';
	b_given[0] = ',';
	if (result != 0) {
		return result;
	} 
	return strcmp(a_given, b_given);
}


PG_FUNCTION_INFO_V1(pname_lt);

Datum
pname_lt(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) < 0);
}

PG_FUNCTION_INFO_V1(pname_le);

Datum
pname_le(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) <= 0);
}

PG_FUNCTION_INFO_V1(pname_eq);

Datum
pname_eq(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) == 0);
}

PG_FUNCTION_INFO_V1(pname_ge);

Datum
pname_ge(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) >= 0);
}

PG_FUNCTION_INFO_V1(pname_gt);

Datum
pname_gt(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) > 0);
}

PG_FUNCTION_INFO_V1(pname_neq);

Datum
pname_neq(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(pname_cmp_internal(a, b) != 0);
}

PG_FUNCTION_INFO_V1(family);

Datum
family(PG_FUNCTION_ARGS)
{
	PersonName    *pname = (PersonName *) PG_GETARG_POINTER(0);
	char * result;
	char * str = pname->name;
	char * temp = strchr(str, ',');
	int length= strlen(str) - strlen(temp);
	result = (char*)palloc(sizeof(char)*length + 1);
	memcpy(result, temp, length);
	memcpy(result+length, "\0", 1);
	PG_RETURN_TEXT_P(cstring_to_text(result));
}

PG_FUNCTION_INFO_V1(given);

Datum
given(PG_FUNCTION_ARGS)
{
	PersonName    *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *result;
	char * str = pname->name;
	char * temp = strchr(str, ',');
	temp++;
	int length= strlen(temp);
	result = (char*)palloc(sizeof(char)*length + 1);
	memcpy(result, temp, length + 1);
	PG_RETURN_TEXT_P(cstring_to_text(result));
}

PG_FUNCTION_INFO_V1(show);

Datum
show(PG_FUNCTION_ARGS)
{
	PersonName    *pname = (PersonName *) PG_GETARG_POINTER(0);
	char * temp;
	int given_len;
	
	char * str = pname->name;

	char * given = strchr(str, ',');
    char * space = strchr(given, ' ');
    int family_len = strlen(str) - strlen(given);
    given++;
    given_len = 0;
    if (space == NULL) {
        given_len = strlen(given);
    } else {
        given_len = strlen(given) - strlen(space);
    }
	
	temp = (char *) palloc(sizeof(char) * (family_len+given_len+2));

    memcpy(temp, given, given_len);
    memcpy(temp+given_len, " ", 1);
    memcpy(temp+given_len+1, str, family_len);
    memcpy(temp+given_len+1+family_len, "\0", 1);
 
	PG_RETURN_TEXT_P(cstring_to_text(temp));
}

PG_FUNCTION_INFO_V1(pname_hash);

Datum
pname_hash(PG_FUNCTION_ARGS)
{
	PersonName    *pname = (PersonName *) PG_GETARG_POINTER(0);
	int hash_code = DatumGetInt32(
						hash_any(
							(unsigned char *) pname->name, 
							sizeof(char) * strlen(pname->name)
						)
					);
	PG_RETURN_INT32(hash_code);
}

PG_FUNCTION_INFO_V1(pname_cmp);

Datum
pname_cmp(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

	PG_RETURN_INT32(pname_cmp_internal(a, b));
}

