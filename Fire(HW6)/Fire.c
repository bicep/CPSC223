#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "/c/cs223/hw6/dict.h"
#include "/c/cs223/hw6/heap.h"


#define STACK_MAX 1000


struct Stack {
    struct room * data[STACK_MAX];
    int size;
};
typedef struct Stack Stack;


void Stack_Init(Stack *S)
{
    S->size = 0;
}

bool Stack_Empty(Stack *S) 
{
    if (S->size==0) {return true;}
    else {return false;}
}

struct room * Stack_Top(Stack *S)
{
    if (Stack_Empty(S)) {
        fprintf(stderr, "Error: stack empty\n");
        return 0;
    } 

    return S->data[S->size-1];
}

void Stack_Push(Stack *S, struct room * d)
{
    if (S->size < STACK_MAX)
        S->data[S->size++] = d;
    else
        fprintf(stderr, "Error: stack full\n");
}

void Stack_Pop(Stack *S)
{
    if (Stack_Empty(S))
        fprintf(stderr, "Error: stack empty\n");
    else
        S->size--;
}


//gets unvisited room (in the event of unconnected graph)
struct room * getUnvisited(Dict d) {
	if (d->n == 0) {fprintf(stderr,"empty dict.\n"); return 0;}
	for (int i = 0; i<d->size; i++) {
		if (d->table[i]) {
			if (d->table[i]->value->visited==0) {
				return d->table[i]->value;
			}
		}
	}
	return 0;
}

void visitReset(Dict d) {
	if (d->n == 0) {fprintf(stderr,"empty dict.\n");}
	for (int i = 0; i<d->size; i++) {
		if (d->table[i]) {
			d->table[i]->value->visited=0;
		}
	}
}

bool seenBefore(struct room * curr, char * neigh, int index) {
	for (int i = 0; i<index; i++) {
		if (!strcmp(neigh,curr->neighbors[i])) {return true;}
	}
	return false;
}
	

//sees if any entry has a neighbor that is not in the dictionary
void warnNoNeighbor(Dict d) {
	if (d->n == 0) {fprintf(stderr,"empty dict.\n"); return;}
	for (int i = 0; i<d->size; i++) {
		if (d->table[i]) {
			for (int j = 0; j<d->table[i]->value->ncount; j++) {
				char * nextkey = d->table[i]->value->neighbors[j];
				struct room * next = DictSearch(d,nextkey);
				if (!next && !(seenBefore(d->table[i]->value,nextkey,j))) {fprintf(stderr,"Warning: room %s has neighbor %s which is not in the dictionary.\n", d->table[i]->value->room, nextkey);}
			}
		}
	}
}


void dfsf(Dict d, struct room * curr,  Stack * s) {
	//if (everynodevisited(d)) {printf("  FAILURE\n"); return;}
	Stack_Push(s,curr);
	//while stack is not empty
	while(!Stack_Empty(s)) {
		//get element at the top
		struct room * v = Stack_Top(s);
		Stack_Pop(s);
		//make sure element is not visited
		if (!v->visited) {
			printf("%s ",v->room);
			//if temp more than 400 we end it here
			if (v->temp>400) {printf(" SUCCESS!\n"); return;}
			//visited!
			v->visited=1;
			//we add all the neighbors
			for (int i = (v->ncount-1); i>=0; i--) {
				char * nextkey = v->neighbors[i];
				struct room * next = DictSearch(d,nextkey);
				if (next) {Stack_Push(s,next);}
				/*else {fprintf(stderr,"Warning: room %s has neighbor %s which is not in the dictionary.\n", v->room, nextkey);}*/
			}
		}
	}

	printf(" FAILED\n");
	return;
}

//depth first search without returning when temperature more than 400
void dfsfwoprint(Dict d, struct room * curr, Stack * s) {
	Stack_Push(s,curr);
	//while stack is not empty
	while(!Stack_Empty(s)) {
		//get element at the top
		struct room * v = Stack_Top(s);
		Stack_Pop(s);
		//make sure element is not visited
		if (!v->visited) {
			//visited!
			v->visited=1;
			//we add all the neighbors
			for (int i = (v->ncount-1); i>=0; i--) {
				char * nextkey = v->neighbors[i];
				struct room * next = DictSearch(d,nextkey);
				if (next) {Stack_Push(s,next);}
			}
		}
	}
	return;
}

void connf(Dict d, struct room * curr, Stack * s) {
	dfsfwoprint(d,curr,s);
	//if there are still unvisited entries then not connected
	if(getUnvisited(d)) {printf("Graph is NOT connected.\n");}
	else {printf("Graph is connected.\n");}
}

bool inNeighbors (struct room * r, char * key) {
	for (int i = 0; i<r->ncount; i++) {
		if (!strcmp(r->neighbors[i],key)) {return true;}
	}
	return false;
}

//gets unvisited room (in the event of unconnected graph)
void dirf(Dict d) {
	bool dir = false;
	if (d->n == 0) {fprintf(stderr,"empty dict.\n"); return;}
	for (int i = 0; i<d->size; i++) {
		if (d->table[i]) {
			struct room * v = d->table[i]->value;
			for (int i = 0; i<v->ncount ; i++) {
				char * nextkey = v->neighbors[i];
				struct room * next = DictSearch(d,nextkey);	
				if(next) {
					if (!inNeighbors(next, v->room)) {
						dir = true;
						printf("Rooms %s and %s are not symmetric.\n",nextkey,v->room);}}

				//ghost room!
				else {
					dir = true;
					printf("Rooms %s and %s are not symmetric.\n",nextkey,v->room);}
			}
		}
	}
	if (dir) {printf("Graph is directed.\n");}
	else {printf("Graph is NOT directed.\n");}
}

void bfsf(Dict d, struct room * curr, Heap h) {
	insert(h,0,curr);
	int count = 0;
	while(!empty(h)) {
		struct room * v = findmin(h);
		deletemin(h);
		if (!v->visited) {
			printf("%s ",v->room);

			if (v->temp>=400) {printf(" SUCCESS!\n"); return;}
			v->visited=1;
			for (int i = 0; i<v->ncount ; i++) {
				char * nextkey = v->neighbors[i];
				struct room * next = DictSearch(d,nextkey);
				if (next) {
					insert(h,count,next);
					count++;
				}
			}
		}
	}

	printf(" FAILED\n");
	return;
}

void bestf(Dict d, struct room * curr, Heap h) {
	insert(h,0,curr);
	while(!empty(h)) {
		struct room * v = findmin(h);
		deletemin(h);
		if (!v->visited) {
			printf("%s ",v->room);

			if (v->temp>=400) {printf(" SUCCESS!\n"); return;}
			v->visited=1;
			for (int i = 0; i<v->ncount ; i++) {
				char * nextkey = v->neighbors[i];
				struct room * next = DictSearch(d,nextkey);
				if (next) {
					int negt = -(next->temp);
					insert(h,negt,next);
				}
			}
			//printHeap(h);
		}
	}

	printf(" FAILED\n");
	return;
}

void destroyHeap2(Heap h) {
	for (int i = 0; i<=h->n; i++) {
		//printf("Freeing: %d\n", h->heap[i]->key);
		//free the room - yes, because now all the rooms are deleted from the heap
		//free the heapslot
		free(h->heap[i]);
	}
	//free the heapslot array
	free(h->heap);
	//free the heap itself
	free(h);
}


		
int main (int argc, char ** argv) {
	char * start;
	bool room = false;
	bool dfs = false;
	bool bfs = false;
	bool best = false;
	bool conn = false;
	bool dir = false;
	if (argc<=1) {fprintf(stderr,"Usage: Fire -room value [-dfs | -bfs | -best | -conn | -dir]\n"); return(0);}
	for (int i = 1; i<argc; i++) {
		if (!strcmp(argv[i],"-room")) {
			if (i<(argc-1)) {	
				room = true;
				start = argv[i+1];
			}
		}
		else if (!strcmp(argv[i],"-dfs")) {dfs = true;}
		else if (!strcmp(argv[i],"-bfs")) {bfs = true;}
		else if (!strcmp(argv[i],"-best")) {best = true;}
		else if (!strcmp(argv[i],"-conn")) {conn = true;}
		else if (!strcmp(argv[i],"-dir")) {dir = true;}
	}

	if (!room) {
		fprintf(stderr,"Fatal error: no room given.\n");
		fprintf(stderr,"Usage: Fire -room value [-dfs | -bfs | -best | -conn | -dir]\n");
		return (0);
	}

	//if none of the above flags set off and we haven't exited to program yet 
	if (!dfs && !bfs && !best && !conn && !dir) {
		dfs = true;
	}

	int bsize = 1000;
	char buffer[bsize];
	int i = 0;
	char ch;
	Dict d = DictCreate();
	while ((ch = getchar())!=EOF) {
		while((ch!='\n') && (ch!=EOF)) {
			//get string
			buffer[i++] = ch;
		        ch = getchar();	
		}
		buffer[i] = '\0';
		//counter is the overall count for the room string
		int counter = 0;
		//ncounter is the internal counter for all the neighbors in the string
		int ncounter = 0;
		char * k = strtok(buffer," ");
		char * key;	
		struct room * r = malloc(sizeof(struct room));
		r->visited = 0;
		while(k) {
			//char * str = strdup(k);
			
			//first thing is the room name
			if (counter == 0) {
				key = k;
				strcpy(r->room,k);
			}
			//second thing is the room temp
			else if (counter == 1) {
				r->temp = atoi(k);
			}
			//third onwards is all the neighbors
			else {
				//all neighbors	
				strcpy(r->neighbors[ncounter],k);
				ncounter++;
			}

			counter++;
			k = strtok(NULL," ");
		}
		r->ncount = ncounter;
/*		printf("key %s\n",key);
		printf("temp %d\n",r->temp);
		printf("room %s\n",r->room);
		for (int j = 0; j<ncounter; j++) {
			printf("neighbor %s\n",r->neighbors[j]);
		}
*/
		struct room * dup = DictSearch(d,key);
		if (dup) {
			fprintf(stderr,"Room %s already in graph.  Replacing it.\n", key);
			DictDelete(d,key);
		}
		//Add room to dictionary
		DictInsert(d,key,r);
		//reset
		i = 0;
	}

	//DictPrint(d);
	warnNoNeighbor(d);

	//search to see if start is in the dictionary
	struct room * gstart;
	if (!(gstart = DictSearch(d,start))) {fprintf(stderr,"Fatal error: room %s not included in graph.\n",start); return 0;}

	//dfs bfs and best
	if (dfs) {
		printf("Starting depth first search: ");
		Stack * s = malloc(sizeof(Stack));
		Stack_Init(s);
		dfsf(d,gstart,s);
		free(s);
		visitReset(d);
	}
	if (bfs) {
		printf("Starting breadth first search: ");
		Heap h = initheap();
		bfsf(d,gstart,h);
		destroyHeap2(h);
		visitReset(d);
	}
	if (best) {
		printf("Starting best first search: ");
		Heap h = initheap();
		bestf(d,gstart,h);
		destroyHeap2(h);
		visitReset(d);
	}
	if (conn) {
		Stack * s = malloc(sizeof(Stack));
		Stack_Init(s);
		connf(d,gstart,s);
		free(s);
		visitReset(d);
	}
	if(dir) {
		dirf(d);
	}

	//connected and directed

	DictDestroy(d);
	return 0;
}
