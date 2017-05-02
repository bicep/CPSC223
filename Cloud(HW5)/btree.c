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

char * strdup (const char * s) {
	char * ret = malloc(sizeof(char) * (strlen(s)+1));
	assert(ret!=NULL);
	strcpy(ret,s);
	return(ret);
}

// create and initialize a node
struct node *makeNode(char * key) {
	struct node * r = malloc(sizeof(struct node));
	r->key = strdup(key);
	r->count = 1;
	r->left = NULL;
	r->right = NULL;
	r->next = NULL;
	r->height = 0;
	r->parent = NULL;
	return(r);
}

// see Aspnes
int treeSize(struct node *root) {
	if (!root) {
		return 0;
	}
	else {
		return (1 + treeSize(root->left) + treeSize(root->right));
	}
}

int treeHeight(struct node *root) {
	if (!root) {
		return -1;
	}
	else {
		int l = treeHeight(root->left);
		int r = treeHeight(root->right);
		return (1 + ((l>r)?l:r));
	}
}


/* returns node with given target key */
/* or null if no such node exists */
struct node * treeSearch(struct node *root, char * target) {
	if (!root) {return NULL;}
	int ret = strcmp(target,(root->key));
	if (!ret) {
		return (root);
	}
	else if (ret>0) {
		return(treeSearch(root->right, target));
	}
	else {
		return(treeSearch(root->left, target));
	}
}

void updateHeight(struct node *t) {
	if (!t) {return;}
	t->height=treeHeight(t);
	updateHeight(t->parent);
}

// adds a node with given key to tree
void treeInsert(struct node *root, char * key) {
	//If empty tree from the get go
	if (!root) {return;}
	int ret = strcmp(key,(root->key));
	if (!ret) {
		root->count++;
		return;
	}
	else if (ret>0) {
		if (!root->right) {
			root->right= makeNode(key);
			root->right->parent = root;
			if (!root->left) {
				updateHeight(root);
			}
			return;
		}
		else {treeInsert(root->right,key);}
	}
	else {
		if (!root->left) {
			root->left = makeNode(key);
			root->left->parent = root;
			if (!root->right) {
				updateHeight(root);
			}
			return;
		}
		else {treeInsert(root->left,key);}
	}
}

void printTree(struct node * tree) {
	printf("%s [%d / %d] ",tree->key,tree->count,tree->height);
}

// print the contents of the tree in preorder, inorder, and postorder,
// respectively.
void printTreePre(struct node * tree) {
	if (!tree) {return;}
	printTree(tree);
	printTreePre(tree->left);
	printTreePre(tree->right);
}

void printTreeIn(struct node * tree) {
	if (!tree) {return;}
	printTreeIn(tree->left);
	printTree(tree);
	printTreeIn(tree->right);
}

void printTreePost(struct node * tree) {
	if (!tree) {return;}
	printTreePost(tree->left);
	printTreePost(tree->right);
	printTree(tree);
}

// free up the space allocated for the tree
void treeDestroy(struct node * tree) {
	if (!tree) {return;}
	else {
		free(tree->key);
		struct node * l = tree->left;
		struct node * r = tree->right;
		free(tree);
		treeDestroy(l);
		treeDestroy(r);
	}
}


