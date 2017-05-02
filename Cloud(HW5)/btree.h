#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


struct node {
  char * key;
  int count;
  struct node *left;  /* left child */
  struct node *right; /* right child */
  struct node *next;  /* link to other nodes over threshold for Cloud */
  // option for more efficient height calculation
  int height;
  struct node *parent;  
};

// create and initialize a node
struct node *makeNode(char * key);


// see Aspnes
int treeSize(struct node *root);
int treeHeight(struct node *root);

/* returns node with given target key */
/* or null if no such node exists */
struct node * treeSearch(struct node *root, char * target);

// adds a node with given key to tree
void treeInsert(struct node *root, char * key);

// print the contents of the tree in preorder, inorder, and postorder,
// respectively.
void printTreePre(struct node * tree);
void printTreeIn(struct node * tree);
void printTreePost(struct node * tree);

// free up the space allocated for the tree
void treeDestroy(struct node * tree);

