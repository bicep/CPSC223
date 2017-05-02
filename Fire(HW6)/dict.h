
struct elt {
  struct elt *next;
  char *key;
  struct room *value;
};

struct dict {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct elt **table;
};


struct room {
  char room[4];
  int temp;
  int ncount;
  int visited;
  char neighbors[10][4];
};


typedef struct dict *Dict;

/* create a new empty dictionary */
Dict DictCreate(void);

/* destroy a dictionary */
void DictDestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
void DictInsert(Dict, const char *key, struct room *value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
struct room *DictSearch(Dict, const char *key);

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void DictDelete(Dict, const char *key);

// printout contents of dictionary
void DictPrint(Dict d);

void printRoom(const struct room * r);
