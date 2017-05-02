/*
struct elt {
  struct elt *next;
  char *key;
};


struct hash {
  int size;           // size of the pointer table 
  int n;              // number of elements stored 
  int buckets;
  struct elt **table;
};
*/

typedef struct hash *Hash;

// hash table parameters
#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

// global debugging flag
bool debugflag;


/* create a new empty hashtable */
Hash HashCreate(void);

/* destroy a hash table */
void HashDestroy(Hash);

/* insert a new key into an existing hashtable */
void HashInsert(Hash, const char *key);

/* return true if the key is in the table
   else, false */
bool HashSearch(Hash, const char *key);


/* print out a hash table */
// mainly for debugging
void HashDisplay(Hash);


