#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "/c/cs223/hw5/btree.h"
#include "AVLtree.h"

bool validToken (char * s) {
	for (int i = 0; s[i]; i++) {
		if (!isalpha(s[i])) {return false;}
	}
	return true;
}

void lowerStr (char * s) {
	for (int i = 0; s[i]; i++) {
		s[i] = tolower(s[i]);
	}
}

int main(int argc, char **argv){
	int thresh = 5;
	bool debug = false;
	bool preo = false;
	bool ino = false;
	bool posto = false;
	bool html = false;
	bool avl = false;

	for (int i = 1; i<argc; i++) {
		if (!strcmp(argv[i],"-debug")) {debug = true;}
		else if (!strcmp(argv[i],"-threshold")) {
			int converted;
			if ((i+1)==argc) {fprintf(stderr,"Missing threshold argment on command line.\n");}
			else if ((converted=atoi(argv[i+1]))) {thresh = converted;i++;}
			else {fprintf(stderr,"Invalid threshold value: %s\n", argv[(i+1)]);}
		}
		else if (!strcmp(argv[i],"-preorder")) {preo=true;}
		else if (!strcmp(argv[i],"-inorder")) {ino=true;}
		else if (!strcmp(argv[i],"-postorder")) {posto=true;}
		else if (!strcmp(argv[i],"-html")) {html=true;}
		else if (!strcmp(argv[i],"-avl")) {avl=true;}
		else {fprintf(stderr,"Fatal error: invalid command line argument: %s\n", argv[i]); exit(0);}
	}
/*
	if (debug) {printf("debug\n");}
	if (preo) {printf("pre\n");}
	if (ino) {printf("in\n");}
	if (posto) {printf("post\n");}
	if (html) {printf("html\n");}
	if (avl) {printf("avl\n");}
	printf("threshold: %d\n", thresh);
*/
	//create tree and linkedlist head
	struct node * tree = makeNode("**root**");
	struct node * head = NULL;

	//create avltree if avl flag set
	struct tree * avlt = TREE_EMPTY;

	int bsize = 1000;
	char buffer[bsize];
	int i = 0;
	char ch;
	while ((ch=getchar()) != EOF) {
		if (ch=='\n') {
			buffer[i]='\0';
			if (debug) {printf("Input: %s\n",buffer);}
			i=0;
			char * k = strtok(buffer," ");
			//assess token
			//continue to get tokens
			while(k) {
				if (validToken(k)) {
					lowerStr(k);
					if (avl) {avlInsert(&avlt,k);}
					treeInsert(tree,k);
					struct node * n = treeSearch(tree, k);
					//if it is equal to thresh, then we add to linked list.
					if (n->count == thresh) {n->next=head; head=n;}
				}
				k = strtok(NULL," ");
			}

		}	
		else{buffer[i++] = ch;}
	}

	//if (debug) {printf("Input: %s\n",buffer);}
	//get first token
	
	if (debug) {printf("Tree height: %d\n",treeHeight(tree)); printf("Tree size: %d\n",treeSize(tree));}
	if (avl) {treePrint(avlt);}
	if (preo) {
		printf("PREORDER\n");
		if (avl) {printAVLPre(avlt);} else {printTreePre(tree);}
		printf("\n");
	}
	if (ino) {
		printf("INORDER\n");
		if (avl) {printAVLIn(avlt);} else {printTreeIn(tree);}
		printf("\n");
	}
	if (posto) {
		printf("POSTORDER\n");
		if (avl) {printAVLPost(avlt);} else {printTreePost(tree);}
		printf("\n");
	}

	if (!head) {printf("No words seen %d times.\n", thresh);}
	else {
		struct node * iter = head;
		int i = 0;
		if (html) {
			while (iter) {
				printf("<span style=\"font-size: %dpx\"> %s </span>\n", iter->count,iter->key);
				iter = iter->next;
			}
		}
		else {
			printf("The Word Cloud:\n");
	
			while (iter) {
				printf("[%d] %s [%d]\n", i, iter->key, iter->count);
				i++;
				iter = iter->next;
			}
		}	
	}

	treeDestroy(tree);
	if(avl) {avlDestroy(&avlt);}
	return(0);
}
	

