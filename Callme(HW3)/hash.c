
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct elt {
  struct elt *next;
  char *key;
};


struct hash {
  int size;           // size of the pointer table 
  int n;              // number of elements stored 
  int buckets;
  //This is an array of pointers to elt
  struct elt **table;
};


typedef struct hash *Hash;

char *strdup(const char *s) {
    char *str;
    char *p;
    int len = 0;
    while (s[len]) {
        len++;
    }
    str = malloc(len + 1);
    p = str;
    while (*s) {
        *p++ = *s++;
    }
    *p = '\0';
    return str;
}

// hash table parameters
#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

// global debugging flag
bool debugflag;

/* create a new empty hashtable */


Hash HashCreateSize(int n) {
	Hash h;
	h = malloc(sizeof(*h));
	assert(h != 0);
	h->size = n;
	h->n = 0;
	h->buckets = 0;
	h->table = malloc(sizeof(struct elt *) * h->size);
	assert(h->table != 0);

	for (int i = 0; i < (h->size); i++) {h->table[i] = NULL;}
	return h;
}

Hash HashCreate(void) {
	return (HashCreateSize(INITIAL_SIZE));
}

/* destroy a hash table */
void HashDestroy(Hash h) {
	struct elt * e;
	struct elt * next;
	for (int i=0; i<(h->size); i++) {
		for (e=h->table[i]; e!=NULL; e=next) {
			next = e->next;
			free(e->key);
			free(e);
		}
	}
	free(h->table);
	free(h);
}

#define MULTIPLIER (97)

unsigned long getBucket(char const* key) {
	unsigned const char * s;
	int h=0;
	for(s = (unsigned const char *) key; *s; s++) {
		h = h * MULTIPLIER + *s;
	}
	return (h);
}

struct elt * insert(struct elt * b, char const * key) {	
	struct elt * e = malloc(sizeof(struct elt));
	e->key = strdup(key);
	e->next = b;
	return(e);
}

//HashInsert but without the expand function
void hInsert(Hash h, char const *key) {
	int b = getBucket(key)%(h->size);
	(h->n)++;
	//Add bucket if there's nothing in the bucket
	if (h->table[b] == NULL) {(h->buckets)++;}
	h->table[b] = insert(h->table[b],key);
}

void expand(Hash h) {
	Hash h2;
	struct hash swap;
	struct elt * e;
	h2 = HashCreateSize(h->size * GROWTH_FACTOR);
	for(int i=0; i<(h->size); i++) {
		for (e=h->table[i]; e!=NULL; e=e->next) {
			hInsert(h2,e->key);
		}
	}
	swap = *h;
	*h = *h2;
	*h2 = swap;
	HashDestroy(h2);
}


/* insert a new key into an existing hashtable */
void HashInsert(Hash h, char const *key) {
	int b = (getBucket(key))%(h->size);
	(h->n)++;
	//Add bucket if there's nothing in the bucket
	if (h->table[b] == NULL) {(h->buckets)++;}
	h->table[b] = insert(h->table[b],key);
	//expand the hash table if necessary
	if ((h->n) >= ((h->size)*MAX_LOAD_FACTOR)) {
		int growto = h->size * MAX_LOAD_FACTOR * 2;
		int n = h->n;
		int buckets = h->buckets;
		float or = (float) buckets / (float) n;
		if (debugflag) {printf("Growing to size: %d. n: %d. Used buckets: %d. Occupancy rate: %f\n",growto,n,buckets,or);}
		expand(h);
	}
}

/* print out a hash table */
// mainly for debugging
void HashDisplay(Hash h) {
	struct elt * e;
	for (int i = 0; i<(h->size); i++) {
		if (h->table[i] != NULL) {printf("[%d] ",i);}
		for(e=h->table[i]; e!=NULL; e=e->next) {
			printf("(%s) ", e->key);
			if (e->next==NULL) {printf("\n");}
		}
	}
}

/* return true if the key is in the table
   else, false */

bool HashSearch(Hash h, char const *key) {
	int b = (getBucket(key))%(h->size);
	struct elt * e;
	for (e=h->table[b]; e!=NULL; e=e->next) {
		if(!strcmp(e->key, key)) {return true;}
	}
	return false;
}
