#include <regex.h>
#include <stddef.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

static int is_person(const char * c);
static void remove_space(char *c);

typedef struct PersonName
{
	int length;
	char  name[];
}			PersonName;

int main() {
/*    char *a = "Featherstone,Albert Basil Ernest George Harold Randolph William";
    printf("the testing result is %d\n", is_person(a));
    return 0;*/
/*
    char * str = "He,  Sixiang";
    int name_len = strlen(str);
	char * given = strchr(str, ',');
	given++;
	int family_len = name_len - strlen(given);
	if (given[0] == ' ') {
		name_len = name_len - 1;
		given++;
	}
	int given_len = strlen(given);
	PersonName * destination = (PersonName *) malloc(4 + sizeof(char) * (name_len + 1));
	//SET_VARSIZE(destination, 4 + sizeof(char) * (name_len + 1));

	//put the value in 
	memcpy(destination->name, str, family_len);
	memcpy(destination->name+family_len, given, given_len);
    printf("The value is:%s\n", destination->name);

    */
    char * str = "Sixiang,He 123 as";
    char * given = strchr(str, ',');
    char * space = strchr(given, ' ');
    int family_len = strlen(str) - strlen(given);
    given++;
    int given_len = 0;
    if (space == NULL) {
        given_len = strlen(given);
    } else {
        given_len = strlen(given) - strlen(space);
    }
    char* a = malloc((family_len+given_len+2)*sizeof(char));
    memcpy(a, given, given_len);
    memcpy(a+given_len, " ", 1);
    memcpy(a+given_len+1, str, family_len);
    memcpy(a+given_len+1+family_len, "\0", 1);
 
    printf("%s\n", a);

    free(a);

}

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
