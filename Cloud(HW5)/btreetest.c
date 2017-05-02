#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "btree.h"

void treeAdd(struct node * tree, char * key){
  struct node * oldnode;

  printf("Adding %s\n", key);
  oldnode = treeSearch(tree, key);
  if (oldnode) {
    oldnode->count++;
  } else {
    treeInsert(tree, key);
  }
}

/* how far to indent each level of the tree */
#define INDENTATION_LEVEL (2)

/* print contents of a tree, indented by depth */
// uses inorder travesal.  Adapted from AVLtree.c
static void
treePrintIndented(struct node *root, int depth)
{
    int i;

    if(root != 0) {
        treePrintIndented(root->left, depth+1);

        for(i = 0; i < INDENTATION_LEVEL*depth; i++) {
            putchar(' ');
        }
        printf("%s Height: %d Size: %d (%p)\n", root->key, treeHeight(root), treeSize(root), (void *) root);

        treePrintIndented(root->right, depth+1);
    }
}

/* print the contents of a tree */
void 
treePrint(struct node *root)
{
    treePrintIndented(root, 0);
}


int main(int argc, char **argv){

  printf("Welcome to btree!\n");

  struct node * root = makeNode("**root**");

  treeAdd(root, "a");
  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  treeAdd(root, "b");
  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  treeAdd(root, "z");
  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  treeAdd(root, "d");

  treeAdd(root, "d");
  treeAdd(root, "b");
  treeAdd(root, "b");
  treeAdd(root, "b");


  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  printTreePre(root);
  printf("\n");
  printTreeIn(root);
  printf("\n");
  printTreePost(root);
  printf("\n");

  printf("\n");
  treePrint(root);
  printf("\n");

  treeDestroy(root);

}
