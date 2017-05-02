#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "AVLtree.h"
//test to compile: gcc -g3 -std=gnu99 -pedantic -Wall AVLtree.c

#define TEST_MAIN
#define PRINT_AFTER_OPERATIONS

char *
strdup2 (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

int
avlHeight(const struct tree *root)
{
    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        return root->height;
    }
}

/* recompute height from height of kids */
static int
treeComputeHeight(const struct tree *root)
{
    int childHeight;
    int maxChildHeight;
    int i;

    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        maxChildHeight = TREE_EMPTY_HEIGHT;

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            childHeight = avlHeight(root->child[i]);
            if(childHeight > maxChildHeight) {
                maxChildHeight = childHeight;
            }
        }

        return maxChildHeight + 1;
    }
}

size_t
avlSize(const struct tree *root)
{
    if(root == 0) {
        return 0;
    } else {
        return root->size;
    }
} 

/* recompute size from size of kids */
static int
treeComputeSize(const struct tree *root)
{
    int size;
    int i;

    if(root == 0) {
        return 0;
    } else {
        size = 1;

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            size += avlSize(root->child[i]);
        }

        return size;
    }
}

/* fix aggregate data in root */
/* assumes children are correct */
static void
treeFix(struct tree *root)
{
    if(root) {
        root->height = treeComputeHeight(root);
        root->size = treeComputeSize(root);
    }
}

/* rotate child in given direction to root */
static void
treeRotate(struct tree **root, int direction)
{
    struct tree *x;
    struct tree *y;
    struct tree *b;

    /*
     *      y           x 
     *     / \         / \
     *    x   C  <=>  A   y
     *   / \             / \
     *  A   B           B   C
     */

    y = *root;                  assert(y);
    x = y->child[direction];    assert(x);
    b = x->child[!direction];

    /* do the rotation */
    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;

    /* fix y then x */
    treeFix(y);
    treeFix(x);
}

/* restore AVL property at *root after an insertion or deletion */
static void
treeRebalance(struct tree **root)
{
    int tallerKid;

    if(*root) {
        for(tallerKid = 0; tallerKid < TREE_NUM_CHILDREN; tallerKid++) {
            if(avlHeight((*root)->child[tallerKid]) >= avlHeight((*root)->child[!tallerKid]) + 2) {

                /* check if zig-zag: opposite-direction nephew is the tall one */
                /* this also covers case where both nephews are too tall */
                if(avlHeight((*root)->child[tallerKid]->child[!tallerKid]) 
                            >= avlHeight((*root)->child[tallerKid]) - 1) {
                    /* zig zag case */
                    treeRotate(&(*root)->child[tallerKid], !tallerKid);
                }

                /* fall through to zig zig case */
                treeRotate(root, tallerKid);

                /* don't bother with other kid */
                break;
            }
        }
    }
}


/* free all elements of a tree, replacing it with TREE_EMPTY */
void 
avlDestroy(struct tree **root)
{
    int i;

    if(*root) {
        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            avlDestroy(&(*root)->child[i]);
        }
	free((*root)->key);
        free(*root);
        *root = 0;
    }
}



/* insert an element into a tree pointed to by root */
void
avlInsert(struct tree **root, char * newElement)
{
    struct tree *e;
    //printf("INSERT element: %s\n", newElement);

    if(*root == 0) {
        /* not already there, put it in */

        e = malloc(sizeof(*e));
        assert(e);

        e->key = strdup2(newElement);
	e->count = 1;
        e->child[LEFT] = e->child[RIGHT] = 0;

        *root = e;
    } else {
        /* do this recursively so we can fix data on the way back out */
	int cmp = strcmp(newElement,(*root)->key);
	if (!cmp) {
		((*root)->count)++;
	}
	else if (cmp>0) {
		avlInsert(&(*root)->child[1],newElement);
	}
	else {
		avlInsert(&(*root)->child[0],newElement);
	}
    }

    /* fix the aggregate data */
    treeFix(*root);
    treeRebalance(root);
}

/* return 1 if target is in tree, 0 otherwise */
int
treeContains(const struct tree *t, char * target)
{
    while(t) {
	int cmp = strcmp(target,(t)->key);
	if (cmp==0) {break;}
	else if (cmp>0) {t = t->child[1];}
	else {t = t->child[0];}
    }

    return (t!=0);
}

/* how far to indent each level of the tree */
#define INDENTATION_LEVEL (2)

/* print contents of a tree, indented by depth */
static void
treePrintIndented(const struct tree *root, int depth)
{
    int i;

    if(root != 0) {
        treePrintIndented(root->child[LEFT], depth+1);

        for(i = 0; i < INDENTATION_LEVEL*depth; i++) {
            putchar(' ');
        }
        printf("%s Height: %d Size: %lu count: %d (%p)\n", root->key, root->height, root->size, root->count, (void *) root);

        treePrintIndented(root->child[RIGHT], depth+1);
    }
}

/* print the contents of a tree */
void 
treePrint(const struct tree *root)
{
    treePrintIndented(root, 0);
}

void printAVL(struct tree * tree) {
	printf("%s [%d / %d] ",tree->key,tree->count,tree->height);
}

// print the contents of the tree in preorder, inorder, and postorder,
// respectively.
void printAVLPre(struct tree * tree) {
	if (!tree) {return;}
	printAVL(tree);
	printAVLPre(tree->child[0]);
	printAVLPre(tree->child[1]);
}

void printAVLIn(struct tree * tree) {
	if (!tree) {return;}
	printAVLIn(tree->child[0]);
	printAVL(tree);
	printAVLIn(tree->child[1]);
}

void printAVLPost(struct tree * tree) {
	if (!tree) {return;}
	printAVLPost(tree->child[0]);
	printAVLPost(tree->child[1]);
	printAVL(tree);
}



size_t
treeRank(const struct tree *t, char * target)
{
    size_t rank = 0;

    while(t && (strcmp(target,t->key)!=0)) {
        if (strcmp(target,t->key)>0) {
            /* go right */
            /* root and left subtree are all less than target */
            rank += (1 + avlSize(t->child[LEFT]));
            t = t->child[RIGHT];
        } else {
            /* go left */
            t = t->child[LEFT];
        }
    }

    /* we must also count left subtree */
    return rank + avlSize(t->child[LEFT]);
}

char *
treeUnrank(const struct tree *t, size_t rank)
{
    size_t leftSize;

    /* basic idea: if rank < avlSize(child[LEFT]), recurse in left child */
    /* if it's equal, return the root */
    /* else recurse in right child with rank = rank - avlSize(child[LEFT]) - 1 */
    while(rank != (leftSize = avlSize(t->child[LEFT]))) {
        if(rank < leftSize) {
            t = t->child[LEFT];
        } else {
            t = t->child[RIGHT];
            rank -= (leftSize + 1);
        }
    }

    return t->key;
}

/*
//#ifdef TEST_MAIN
// check that aggregate data is correct throughout the tree 
static void
treeSanityCheck(const struct tree *root)
{
    int i;

    if(root) {
        assert(root->height == treeComputeHeight(root));
        assert(root->size == treeComputeSize(root));

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            treeSanityCheck(root->child[i]);
        }
    }
}
*/

/*

int
main(int argc, char **argv)
{
    int i;
    //const int n = 10;
    struct tree *root = TREE_EMPTY;

    if(argc == 1) {
        fprintf(stderr, "Usage: %s [keys]+\n", argv[0]);
        return 1;
    }

    for(i = 1; i < argc; i++) {
      
      assert(!treeContains(root, argv[i]));
      avlInsert(&root, argv[i]);
      assert(treeContains(root, argv[i]));
#ifdef PRINT_AFTER_OPERATIONS
      treePrint(root);
      puts("---");
#endif
    }

//COMMENT THIS OUT
    printf("** check ranks\n");
    // check ranks 
    for(i = 1; i < argc; i++) {
      printf("treeRank(root, %s) == %lu i == [%d]\n", argv[i], treeRank(root, argv[i]), i);
      assert(treeRank(root, argv[i]) == (i-1));
      printf("treeUnrank(root, %d) == %s argv[i] == [%s]\n", i-1, treeUnrank(root, i-1), argv[i]);
      // assert(treeUnrank(root, i-1) == argv[i]);
    }
//END COMMENT

    printf("** sanity check\n");
    treeSanityCheck(root);

//COMMENT THIS OUT
    printf("** delete everyting\n");
    // now delete everything 
    for(i = 1; i < argc; i++) {
      assert(treeContains(root, argv[i]));
      treeDelete(&root, argv[i]);
      assert(!treeContains(root, argv[i]));
#ifdef PRINT_AFTER_OPERATIONS
      treePrint(root);
      puts("---");
#endif
    }
//END COMMENT

    printf("** sanity check\n");
    treeSanityCheck(root);

    printf("** destroy\n");
    avlDestroy(&root);

    return 0;
}
#endif
*/
