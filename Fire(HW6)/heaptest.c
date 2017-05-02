#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "heap.h"
#include "dict.h"

//void * mymalloc(size_t s, int i)


int main(int argc, char ** argv){

  debugflag = true;

  Heap h = initheap();

  struct room * r;

  //printHeap();

  if (argc > 1) {
    for (int i=1; i < argc; i++) {
      int val = atoi(argv[i]);
      if (val > 0) {
	r = malloc(sizeof(struct room));
	r->temp = val*val;
	sprintf(r->room, "R%d", val);
	printf("INSERT: %d, %s, %d ", val,r->room,r->temp);
	insert(h, val, r);
	printHeap(h);
      } else {
	deletemin(h);
      }
    }
  } else {
    fprintf(stderr, "Usage: heap [numbers]+\n");
  }
  printf("DestroyHeap\n");
  destroyHeap(h);

}
