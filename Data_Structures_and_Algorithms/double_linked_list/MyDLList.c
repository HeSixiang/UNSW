#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

static int is_vaild_number(const char * c);
static char * read_file(FILE* fptr, int type);
static void insert_node(DLList * list, int value);
static int contian_node(DLList * list, int value);

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;

	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
DLList *CreateDLListFromFileDlist(const char *filename)
{
    DLList * result = newDLList();
    char * input_result;
    if (strcmp(filename, "stdin")) {
        FILE *fptr;
        fptr = fopen(filename, "r");
        if (fptr == NULL) {
            printf("Invalid input! \n");
            return NULL;
        }
        input_result = read_file(fptr, 0);
        fclose(fptr);
    } else {
        input_result = read_file(stdin, 1);
        char * find_end = strstr(input_result, "end");
        if (find_end == NULL) {
            printf("Invalid input!\n");
            return NULL;
        }
        find_end[0] = '\0';
        int input_len = strlen(input_result) + 1; // include \0
        input_result = realloc(input_result, sizeof(char)*input_len);
        assert (input_result != NULL);
    }
    char delim[] = " ";
    char *ptr = strtok(input_result, delim);
    int node_value;
    while(ptr != NULL)
    {
        if (!is_vaild_number(ptr)) {
            printf("Invalid input!\n");
            return NULL;
        }
        sscanf(ptr, "%d", &node_value);
        insert_node(result, node_value);
        ptr = strtok(NULL, delim);
    }
    free(input_result);
    return result;
}


// clone a DLList
// put your time complexity analysis for cloneList() here
DLList *cloneList(DLList *u)
{
    if (u == NULL) {
        return NULL;
    }
	DLList * result = newDLList();
	DLListNode * temp = u->first;
	if (temp == NULL) {
		return result;
	}
	while (temp != NULL) {
		insert_node(result, temp->value);
		temp = temp->next;
	}
	return result;
}

// compute the union of two DLLists u and v
DLList *setUnion(DLList *u, DLList *v)
{
	DLList * result = cloneList(u);
	DLListNode * temp = v->first;
	while(temp != NULL) {
		if (contian_node(result, temp->value) != 1) {
			insert_node(result, temp->value);
		}
        temp = temp->next;
	}
	return result;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
DLList *setIntersection(DLList *u, DLList *v)
{
	DLList * result = newDLList();
	DLListNode * temp = v->first;
	while(temp != NULL) {
		if (contian_node(u, temp->value) == 1) {
			insert_node(result, temp->value);
		}
        temp = temp->next;
	}
	return result;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here
void freeDLList(DLList *L)
{
    if (L == NULL) {
        return;
    }
	DLListNode * temp = L->last;
	int L_size = L->size;
	for (int i =0; i < L_size; i++) {
		temp = temp->prev;
		free(L->last);
		L->last = temp;
	}
	free(L);
}


// display items of a DLList
// put your time complexity analysis for printDDList() here
void printDLList(DLList *u)
{
    if (u == NULL) {
        return;
    }
	DLListNode * temp = u->first;
	while(temp != NULL) {
		printf("%d   ", temp->value);
		temp = temp->next;
	}
    printf("\n");
}

static int is_vaild_number(const char * c) {
    const char * p = c;
    char temp = p[0];
    if (temp == '\0') {
        return 1;
    }
    if (temp != '+' && temp != '-' && !isdigit(temp)){
        return 0;
    }
    while (temp != '\0') {
        p++;
        temp = p[0];
        if (!isdigit(temp) && temp != '\0') {
            return 0;
        }
    }
    return 1;
}

// 0 for read file, 1 for user input
static char * read_file(FILE* fptr, int type) {
    char c;
    int size = 10; //initial buffer size
    int len = 0;
    char * result = malloc(sizeof(char)*size);
    assert (result != NULL);
    while((c = fgetc(fptr)) != EOF) {
        if (type == 1 && c == '\n') {
            break;
        }
        if (c == '\r' || c == '\n') {
            c = ' ';
        }
        result[len++]=c;
        if(len==size){
            result = realloc(result, sizeof(char)*(size+=128));
            assert (result != NULL);
        }
    }
    result[len++]='\0';
    return realloc(result, sizeof(char)*len);;
}

static void insert_node(DLList * list, int value) {
    DLListNode * node = newDLListNode(value);
    assert(list != NULL);
    if (list == NULL) {
        return;
    }
    if (list->size == 0) {
        list->first = node;
        list->last = node;
    } else {
        node->prev = list->last;
        list->last->next = node;
        list->last = node;
    }
    list->size++;
}

//return 1 if contains, 0 if not.
static int contian_node(DLList * list, int value) {
	if (list->size == 0) {
		return 0;
	}
	DLListNode * temp = list->first;
	while (temp != NULL) {
		if (temp->value == value) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int main()
{
    DLList *list1, *list2, *list3, *list4;

    list1=CreateDLListFromFileDlist("File1.txt");
    printDLList(list1);

    list2=CreateDLListFromFileDlist("File2.txt");
    printDLList(list2);    


    list3=setUnion(list1, list2);
    printDLList(list3);

    list4=setIntersection(list1, list2);
    printDLList(list4);

    freeDLList(list1);
    freeDLList(list2);
    freeDLList(list3);
    freeDLList(list4);

    printf("please type all the integers of list1\n");
    list1=CreateDLListFromFileDlist("stdin");

    printf("please type all the integers of list2\n");
    list2=CreateDLListFromFileDlist("stdin");

    list3=cloneList(list1);
    printDLList(list3);
    list4=cloneList(list2);
    printDLList(list4);

    freeDLList(list1);
    freeDLList(list2);
    freeDLList(list3);
    freeDLList(list4);

    return 0; 
}
