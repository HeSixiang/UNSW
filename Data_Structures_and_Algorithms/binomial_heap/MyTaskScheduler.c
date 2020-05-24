#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// data type for heap nodes
typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	// add additional fields here
	struct HeapNode * parent;
	struct HeapNode * sibling;
	struct HeapNode * child;
	int degree;
} HeapNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap{ //this is heap header
	int  size;      // count of items in the heap
	// add additional fields here
	struct HeapNode * head;
} BinomialHeap;

static char * read_file(FILE* fptr);
static int is_number(char * string);
static int get_release_time_queue(char * filename, BinomialHeap* result);
static BinomialHeap* remove_min_build_tree(HeapNode * node);
static HeapNode * find_min_node(BinomialHeap *T, HeapNode ** prev_result);
static void merge_same_degree(HeapNode* parent, HeapNode* child);
static void self_merge(BinomialHeap* T);
static void destory_heap_node(HeapNode * node);

BinomialHeap * Merge(BinomialHeap *T1, BinomialHeap *T2);
void Destory_Heap(BinomialHeap *T);


// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d, HeapNode * parent, HeapNode * sibling, 
						HeapNode * child, int degree)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself) 	 
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r; 
	new->Dline = d;
	new->parent = parent;
	new->sibling = sibling;
	new->child = child;
	new->degree = degree;
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	T->head = NULL;
	T->size = 0;
	return T;
}

// Time complexity is O(logn)  
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d) {
	// k: key, n: task name, c: execution time, r: release time, d:deadline 
	// You don't need to check if this task already exists in T 
	if (T == NULL) {
		//do nothing
		return;
	}
	HeapNode* new_node = newHeapNode(k, n, c, r, d, NULL, T->head, NULL, 0);
	T->head = new_node;
	T->size++;
	self_merge(T);
}

// Time complexity is O(logn) 
HeapNode *RemoveMin(BinomialHeap *T) {
	HeapNode * prev_result;
	HeapNode * result = NULL;
	result = find_min_node(T, &prev_result);
	if (result == NULL) {
		return NULL; 
	}
	if (prev_result == NULL) {
		T->head = result->sibling;
	} else {
		prev_result->sibling = result->sibling;
	}
	result->sibling = NULL; //for protection
	BinomialHeap* T2 = remove_min_build_tree(result);
	T->size = T->size - 1 - T2->size;
	BinomialHeap* new_heap = Merge(T, T2);
	T->head = new_heap->head;
	T->size = new_heap->size;
	new_heap->head = NULL;
	Destory_Heap(new_heap);
	Destory_Heap(T2);
	return result;
}

// Time complexity is O(logn) 
int Min(BinomialHeap *T) {
	HeapNode * prev_result;
	HeapNode * result = find_min_node(T, &prev_result);
	if (result == NULL) {
		return -1; //error
	}
	return result->key;
}

//  Time complexity is O(nlogn)
//  Becasue the insert, remove and min are all O(logn) 
int TaskScheduler(char *f1, char *f2, int m ) {
	//core should greater than 0
	if (m <= 0 || f1 == NULL || f2 == NULL) {
		return 0;
	}

	//open file for writing
	FILE *fptr;
	fptr = fopen(f2,"w");
	if (fptr == NULL) {
		return 0; //error
	}

	//build release queue
	BinomialHeap* release_q = NULL;
	release_q = newHeap();
	if (get_release_time_queue(f1, release_q)) {
		return 0;
	}

	//build core queue 
	BinomialHeap* core_q = NULL;
	core_q = newHeap();
	for(int i = 1; i < m+1; i++) {
		Insert(core_q, 0, i, 0, 0, 0);
	}

	//build deadline queue
	BinomialHeap* deadline_q = NULL;
	deadline_q = newHeap();

	int current_time = 0;

	while(release_q->head != NULL || deadline_q->head != NULL) {
		//update the core queue
		while(Min(core_q) < current_time) {
			HeapNode * temp = RemoveMin(core_q);
			Insert(core_q, current_time, temp->TaskName, 0, 0, 0);
			free(temp);
		}

		//get all new release tasks at that time and update dealine queue
		while(release_q->head != NULL && Min(release_q) <= current_time) {
			HeapNode * temp = RemoveMin(release_q);
			Insert(deadline_q, temp->Dline, temp->TaskName, temp->Etime, temp->Rtime, temp->Dline);
			free(temp);
		}

		//assign the task to core
		while(Min(core_q) == current_time && deadline_q->head != NULL) {
			HeapNode * next_task = RemoveMin(deadline_q);
			HeapNode * next_core = RemoveMin(core_q);
			fprintf(fptr, "%d Core%d %d ", next_task->TaskName, next_core->TaskName, current_time);
			if (next_task->Dline < current_time + next_task->Etime) {
				//pass due
				fprintf(fptr,"%s", "... ");
				free(next_task);
				free(next_core);
				fclose(fptr);
				Destory_Heap(release_q);
				Destory_Heap(deadline_q);
				Destory_Heap(core_q);
				return 0;
			}
			Insert(core_q, current_time + next_task->Etime, next_core->TaskName, 0, 0, 0);
			free(next_task);
			free(next_core);
		}
		current_time++;
	}
	fclose(fptr);
	Destory_Heap(release_q);
	Destory_Heap(deadline_q);
	Destory_Heap(core_q);
	return 1;
}

// merge two tree, and after that T1 and T2 will be empty
BinomialHeap * Merge(BinomialHeap *T1, BinomialHeap *T2) {
	if (T1 == NULL || T2 == NULL) {
		return NULL;
	}
	BinomialHeap * result = newHeap();
	if (T1 == NULL) {
		result->size = T2->size;
	} else if (T2 == NULL) {
		result->size = T1->size;
	} else {
		result->size = T1->size + T2->size;
	}
	HeapNode* dummy_node = newHeapNode(0,0,0,0,0,NULL, NULL, NULL, 0);
	result->head = dummy_node;
	HeapNode* current;
	current = result->head;
	while(T1->head != NULL || T2->head != NULL) {
		if (T1->head == NULL) {
			current->sibling = T2->head;
			T2->head = NULL;
		} else if (T2->head == NULL) {
			current->sibling = T1->head;
			T1->head = NULL;
		} else {
			if (T1->head->degree < T2->head->degree) {
				current->sibling = T1->head;
				T1->head = T1->head->sibling;
			} else {
				current->sibling = T2->head;
				T2->head = T2->head->sibling;
			}
		}
		current = current->sibling;
	}
	result->head = dummy_node->sibling;
	free(dummy_node);

	self_merge(result);
	return result;
}

void Destory_Heap(BinomialHeap *T) {
	destory_heap_node(T->head);
	free(T);
}

static BinomialHeap* remove_min_build_tree(HeapNode * node) {
	assert(node != NULL);
	BinomialHeap* result = newHeap();
	if (node->child == NULL) {
		return result;
	}
	// like reverse a single linked list
	HeapNode * current = node->child;
	HeapNode * next = NULL;
	result->head = current;
	result->size += (int)pow(2.0, (double)current->degree);
	current = current->sibling;
	result->head->sibling = NULL;
	while(current != NULL) {
		result->size += (int)pow(2.0, (double)current->degree);
		next = current->sibling;
		current->sibling = result->head;
		result->head = current;
		current = next;
	}
	return result;
}

static HeapNode * find_min_node(BinomialHeap *T, HeapNode ** prev_result) {
	HeapNode * result = NULL;
	*prev_result = NULL;
	if (T == NULL || T->head == NULL) {
		return result;
	}
	int current_min = T->head->key;
	result = T->head;
	HeapNode * current = T->head->sibling;
	HeapNode * prev = T->head;
	//Compare key first, if equal, compare task name
	int task_name = T->head->TaskName;
	while (current != NULL) {
		if (current->key < current_min || (current->key == current_min && current->TaskName < task_name)) {
			current_min = current->key;
			*prev_result = prev;
			result = current;
			task_name = current->TaskName;
		}
		prev = current;
		current = current->sibling;
	}
	return result;
}

static void destory_heap_node(HeapNode * node) {
	if (node == NULL) {
		return;
	}
	destory_heap_node(node->child);
	destory_heap_node(node->sibling);
	free(node);
}

//this function will merge the BinomialHeap if neccessary and make it vaild.
//Note: expect input head nodes is well-order, and no more than 3 duplicate degree
//helper function for merge
static void self_merge(BinomialHeap* T) {
	if (T == NULL || T->head == NULL || T->head->sibling == NULL) {
		return;
	}
	HeapNode* prev = NULL;
	HeapNode* current = T->head;
	HeapNode* next = current->sibling;
	while(next != NULL) {
		if (current->degree == next->degree && 
			(next->sibling == NULL || next->sibling->degree != current->degree)) {
			if (current->key < next->key || (current->key == next->key && current->TaskName < next->TaskName)) {
				current->sibling = next->sibling;
				merge_same_degree(current, next);
			} else {
				if (prev == NULL) {
					T->head = next;
				} else {
					prev->sibling = next;
				}
				merge_same_degree(next, current);
				current = next;
			}
			next = current->sibling;
		} else {
			prev = current;
			current = next;
			next = next->sibling;
		}
	}
}

// merge two same degree tree, with bigger degree tree
// helper fucntion for self_merge	 
static void merge_same_degree(HeapNode* parent, HeapNode* child) {
	assert(parent != NULL);
	assert(child != NULL);
	parent->degree += 1;
	child->sibling = parent->child;
	child->parent = parent;
	parent->child = child;
}


static int get_release_time_queue(char * filename, BinomialHeap* result) {
	assert(result != NULL);
	assert(result->head == NULL);
	assert(result->size == 0);
	FILE *fptr;
	fptr = fopen(filename, "r");
	if (fptr == NULL) {
		char * err = "file1 does not exist";
		printf("%s\n", err);
		return 1;
	}
	char * file_read_result = read_file(fptr);
	fclose(fptr);
	char delim[] = " ";
    char *ptr = strtok(file_read_result, delim);
	int task, execution, release, deadline;
    while(ptr != NULL) {
		char * taskname = ptr;
		if (!is_number(ptr)) {
			printf("input error when reading the attribute of the task %s\n", taskname);
			return 1;
		}
		sscanf(ptr, "%d", &task);

		ptr = strtok(NULL, delim);
		if (ptr == NULL || !is_number(ptr)) {
			printf("input error when reading the attribute of the task %s\n", taskname);
			return 1;
		}
		sscanf(ptr, "%d", &execution);
		// all the execution times and the deadlines are natural numbers. 
		if (execution <= 0) {
			printf("input error when reading the attribute of the task %s\n", taskname);
			return 1;
		}

		ptr = strtok(NULL, delim);
		if (ptr == NULL || !is_number(ptr)) {
			printf("input error when reading the attribute of the task %s\n", taskname);
			return 1;
		}
		sscanf(ptr, "%d", &release);
		// All the release times are non-negative integers
		if (release < 0) {
			printf("input error when reading the attribute of the task %s\n", taskname);
			return 1;
		}

		ptr = strtok(NULL, delim);
		if (ptr == NULL || !is_number(ptr)) {
			printf("input error when reading the attribute of the task %s\n", taskname);
			return 1;
		}
		sscanf(ptr, "%d", &deadline);
		if (deadline <= 0) {
			printf("input error when reading the attribute of the task %s\n", taskname);
			return 1;
		}

		Insert(result, release, task, execution, release, deadline);

		ptr = strtok(NULL, delim);
		
	}
	free(file_read_result);
	return 0;
}

static int is_number(char * string) {
	if (string[0] == '\n') {
		return 1;
	}
	while(string[0] != '\n') {
		if (!isdigit(string[0])) {
			return 1;
		}
		string++;
	}
	return 0;
}

static char * read_file(FILE* fptr) {
    char c;
    int size = 10; //initial buffer size
    int len = 0;
    char * result = malloc(sizeof(char)*size);
    assert (result != NULL);
    while((c = fgetc(fptr)) != EOF) {
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

int main() {
	//sample main for testing 
	int i;
	i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
	if (i==0) printf("No feasible schedule!\n");
	//There is a feasible schedule on 4 cores 
	i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
	if (i==0) printf("No feasible schedule!\n");
	//There is no feasible schedule on 3 cores 
	i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
	if (i==0) printf("No feasible schedule!\n");
	// There is a feasible schedule on 5 cores 
	i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
	if (i==0) printf("No feasible schedule!\n");
	// There is no feasible schedule on 4 cores 
	i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
	if (i==0) printf("No feasible schedule!\n");
	// There is no feasible schedule on 2 cores 
	i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
	if (i==0) printf("No feasible schedule!\n");
	// There is a feasible schedule on 2 cores 
	return 0; 
}


