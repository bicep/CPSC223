#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "hash.h"

// my own version of strdup
char *strdup(const char *str)
{
  int len = strlen(str);
  char *tmp = malloc(len + 1);
  if (tmp) {
    strcpy(tmp, str);
    //tmp[len] = '\0';
  }
  return tmp;
}


struct elt {
  struct elt *next;
  char *key;
};


struct hash {
  int size;           /* size of the pointer table */
  int n;              /* number of elements stored */
  int buckets;
  struct elt **table;
};



/* hashtable initialization code used in both HashCreate and grow */
Hash
internalHashCreate(int size)
{
    Hash d;
    int i;

    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->buckets = 0;
    d->table = malloc(sizeof(struct elt *) * d->size);

    assert(d->table != 0);

    for(i = 0; i < d->size; i++) d->table[i] = 0;

    return d;
}

Hash
HashCreate(void)
{
    return internalHashCreate(INITIAL_SIZE);
}

void
HashDestroy(Hash d)
{
    int i;
    struct elt *e;
    struct elt *next;

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->key);
            // free(e->value);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define MULTIPLIER (97)

static unsigned long
hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h;

    h = 0;

    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

static void
grow(Hash d)
{
    //HashDisplay(d);
    Hash d2;            /* new hashtable we'll create */
    struct hash swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

    d2 = internalHashCreate(d->size * GROWTH_FACTOR);

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            /* note: this recopies everything */
            /* a more efficient implementation would
             * patch out the strdups inside HashInsert
             * to avoid this problem */
	  HashInsert(d2, e->key);
        }
    }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call HashDestroy on d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;

    if (debugflag) { printf("Growing to size: %d. n: %d. Used buckets: %d. Occupancy rate: %.2f\n", d->size, d->n, d->buckets, (double) (d->buckets*1.0 / d->n));}
    HashDestroy(d2);
}

/* insert a new key into an existing hashtable */
void
HashInsert(Hash d, const char *key)
{
    struct elt *e;
    unsigned long h;

    assert(key);
    // assert(value);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);

    h = hash_function(key) % d->size;

    e->next = d->table[h];
    if (d->table[h] == NULL) {
      d->buckets++;
    }
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

/* return true if the key is in the table
   else, false */
bool
HashSearch(Hash d, const char *key)
{
    struct elt *e;

    for(e = d->table[hash_function(key) % d->size]; e != 0; e = e->next) {
      if(!strcmp(e->key, key)) {
	/* got it */
	return true;
      }
    }

    return false;
}

void HashDisplay(Hash h){

  int i;
  struct elt *e;
  
  for(i = 0; i < h->size; i++) {
    if (h->table[i] != 0) {
      printf("[%d]",i);
      for(e = h->table[i]; e != 0; e = e->next) {
	printf(" (%s)", e->key);
      }
      printf("\n");
    }
  }
}
