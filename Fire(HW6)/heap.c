#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "/c/cs223/hw6/heap.h"
#include "/c/cs223/hw6/dict.h"

#define MAXHEAP 4

Heap initheap() {	
	Heap h = malloc(sizeof(struct heap));
	h->heap = malloc(sizeof(heapslot *)*MAXHEAP);
	heapslot * hs = malloc(sizeof(heapslot));
	hs->key = INT_MIN;
	hs->value = NULL;
	h->heap[0] = hs;
	h->size = MAXHEAP;
	h->n = 0;
	return h;
}

bool empty(Heap h){
	return (h->n == 0);
}

void demand(int cond, char * msg) {
	if (!cond) {fprintf(stderr,"%s\n",msg); exit(0);}
	else {return;}
}

void printHeap(Heap h) {
	printf("HEAP:");
	for (int i = 1;i<=(h->n);i++) {
		printf(" [%d: %s %d/%d]", i, h->heap[i]->value->room, h->heap[i]->key, h->heap[i]->value->temp);
	}
	printf("\n");
}

struct room * findmin(Heap h) {
	demand(!(empty(h)),"findmin not allowed on empty heap!");
	return (h->heap[1]->value);
}

void swap(heapslot *s1, heapslot *s2) {
	heapslot temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void insert(Heap h, int key, struct room * r) {
	int parent;
	int curr;
	if ((h->n)>=((h->size)-1)) {
		h->heap = realloc(h->heap,(sizeof(heapslot *)*(h->size)*2));
		demand(h->heap!=0,"Realloc failed");
		h->size = h->size * 2;
	}
	//printf("%d\n",h->n);
	curr = ++(h->n);
	heapslot * hs = malloc(sizeof(heapslot));
	hs->key = key;
	hs->value = r;
	h->heap[curr]= hs;
	parent = curr/2;
	while((h->heap[curr]->key)<(h->heap[parent]->key)) {
		demand(parent>0,"Inserted item rising past root.");
		swap(h->heap[curr],h->heap[parent]);
		curr = parent;
		parent = curr/2;
	}
}

void deletemin(Heap h) {
	int curr;
	int child;
	demand((!empty(h)),"deletemin not allowed one empty heap");
	//We still want the room to be in dict; we don't free it just yet.
	//free(h->heap[1]->value);
	free(h->heap[1]);
	h->heap[1] = h->heap[(h->n)--];
	curr = 1;
	child = 2;
	while (child<=(h->n)) {
		if ((child != (h->n)) && (h->heap[child+1]->key < h->heap[child]->key)) {
			child++;
		}
		if (h->heap[curr]->key > h->heap[child]->key) {
			demand(curr<=(h->n),"falling past leaves");
			swap(h->heap[curr], h->heap[child]);
			curr = child;
			child = curr*2;
		}
		else {
			break;
		}
	}
}

void destroyHeap(Heap h) {
	for (int i = 0; i<=h->n; i++) {
		//printf("Freeing: %d\n", h->heap[i]->key);
		//free the room - yes, because now all the rooms are deleted from the heap
		if (h->heap[i]->value!=NULL) {free(h->heap[i]->value);}
		//free the heapslot
		free(h->heap[i]);
	}
	//free the heapslot array
	free(h->heap);
	//free the heap itself
	free(h);
}

