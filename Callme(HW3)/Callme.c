#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "hash.h"

bool isalphabet (char * s) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (!isalpha(s[i])) {return false;}
	}
	return true;
}

bool isnumeric (char * s) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (!isdigit(s[i])) {return false;}
	}
	return true;
}

//FUNCTIONS as defined in my implementation of hash.c, called ver 2 so it does not clash with 
//same function defined in hash.c
struct elt * insert2(struct elt * b, char const * key) {	
	struct elt * e = malloc(sizeof(struct elt));
	e->key = strdup(key);
	e->next = b;
	return(e);
}

//Functions copied from hash.c ends here.

void invarg(char * s) {
	fprintf(stderr, "Fatal error: invalid argument %s\n", s);
}

long chartonum(char c) {
	c = tolower(c);
	if (c=='a' || c=='b' || c=='c') {return 2;}
	if (c=='d' || c=='e' || c=='f') {return 3;}
	if (c=='g' || c=='h' || c=='i') {return 4;}
	if (c=='j' || c=='k' || c=='l') {return 5;}
	if (c=='m' || c=='n' || c=='o') {return 6;}
	if (c=='p' || c=='q' || c=='r' || c=='s') {return 7;}
	if (c=='t' || c=='u' || c=='v') {return 8;}
	if (c=='w' || c=='x' || c=='y' || c=='z') {return 9;}
	else {return -1;}
}

void alphatonum(char * s, long * arr) {
	int count = 0;
	for (int i = 0; s[i]!='\0'; i++) {
		long n = chartonum(s[i]);
		assert(n!=-1);
		arr[i] = n;
		count++;
	}
	arr[count] = '\0';
}

void fill(char c1, char c2, char c3, char arr[]) {
	arr[0] = c1;
	arr[1] = c2;
	arr[2] = c3;
	arr[3] = '\0';
}

void fill2(char c1, char c2, char c3, char c4, char arr[]) {
	arr[0] = c1;
	arr[1] = c2;
	arr[2] = c3;
	arr[3] = c4;
	arr[4] = '\0';
}

void numtochar(char c, char arr[]) {
	int i = c-'0';
	if (i==2) {fill('a','b','c',arr);}
	else if (i==3) {fill('d','e','f',arr);}
	else if (i==4) {fill('g','h','i',arr);}
	else if (i==5) {fill('j','k','l',arr);}
	else if (i==6) {fill('m','n','o',arr);}
	else if (i==7) {fill2('p','q','r','s',arr);}
	else if (i==8) {fill('t','u','v',arr);}
	else if (i==9) {fill2('w','x','y','z',arr);}
	else {arr[0] = '\0';}
}


void genComb(struct elt ** gen, char * str, char * x, int count, int l) {
	if (count == l) {
		//+1 because extra space for null character
		*gen = insert2(*gen,str);
		return;
	}
	char c = x[count];
	char comb[5];
	numtochar(c,comb);
	if(comb[0] == '\0') {printf("numeric: %s => ** no matches **\n", x); exit(0);}
	for (int j = 0; comb[j] != '\0'; j++) {
		//+1 because extra space for null character
		char * nstr = malloc(sizeof(char) * (l+1));
		nstr = strcpy(nstr,str);
		nstr[count] = comb[j];
		nstr[count+1] = '\0';
		genComb(gen,nstr,x,count+1,l);	
		free(nstr);
	}

}

void strtolower (char s[]) {
	for (int i=0; s[i]!='\0'; i++) {
		s[i] = tolower(s[i]);
	}
}

void loadD(Hash h, int l, bool file, char * fileN) {
	FILE * f;
	int wcount = 0;
	if (file) {
		if(debugflag) {printf("Loading dictionary: %s\n",fileN);}
		f = fopen(fileN,"r");
	}
	else {
		if (debugflag) {printf("Loading dictionary: /usr/share/dict/words\n");}
		f = fopen("/usr/share/dict/words","r");
	}

	char line[256];
	while(fgets(line,sizeof(line),f)) {
		//+1 because of the newline
		if (strlen(line) == l+1) {
			//get rid of newline
			if (line[l] == '\n') {line[l] = '\0';}
		       	if (isalphabet(line)) {
				//to lower case
				strtolower(line);
			       	if (!HashSearch(h,line)) {
					//this is what we want, alpha, never seen before
					HashInsert(h,line);
					wcount++;
				}
			}
		}
	}

	if (debugflag) {printf("Word Count: %d\n",wcount);}
	fclose(f);
}


void checkD(Hash h, struct elt  * ls, char * x) {
	struct elt * iter = ls;
	int match = 0;
	printf("numeric: %s =>",x);
	while (iter != NULL) {
		char * k = iter->key;
		if(HashSearch(h,k)) {match++; printf(" %s", k);}
		iter = iter->next;
	}
	if (match == 0) {printf(" ** no matches **");}
	printf("\n");
}


void reversels (struct elt ** lp) {
	if ((*lp != NULL) && ((*lp)->next != NULL)) {
		struct elt * prev = NULL;
		struct elt * current = *lp;
		struct elt * next = NULL;
		while (current != NULL) {
			next = current->next;
			current->next = prev;
			prev = current;
			current = next;
		}
		*lp = prev;
	}
}


int main(int argc, char **argv) {

	if (argc == 1 || argc > 5) {fprintf(stderr,"usage: Callme (digits | letters) [-debug]? [-file filename]\n"); return 0;}

	bool alpha = false;
	bool num = false;
	bool debug = false;
	bool file = false;
	char * fileN = NULL;

	//Get the string, which will be 
	char * x = argv[1];
	//if alpha it is easy
	if (isalphabet(x)) {alpha = true;}
	//if numeric it is harder
	else if (isnumeric(x)) {num = true;}
	else {invarg(x); return 0;}

	//get length of the arguments
	int arglen = 0;
	for (int i = 0; argv[i]!='\0'; i++) {
		arglen++;
	}
	
	//if there is only one arg, and we havne't thrown error, we proceed
	//if there are more than one args, then we set our flags accordingly
	if (arglen==3) {
		if (!strcmp(argv[2],"-debug")) {debug = true;}
		else {invarg(argv[2]); return 0;}
	}
	else if (arglen==4) {
		if (!strcmp(argv[2],"-file")) {file = true; fileN = argv[3];}
		else {invarg(argv[2]); return 0;}
	}
	else if (arglen==5) {
		if (!strcmp(argv[2],"-debug") && !strcmp(argv[3],"-file")) {
			debug = true;
			file = true;
			fileN = argv[4];
		}
		else {
			if (strcmp(argv[2],"-debug")) {invarg(argv[2]); return 0;}
			else {invarg(argv[3]); return 0;}
		}
	}

	//we haven't thrown an error yet so we can proceed to process
	if(debug) {debugflag=true;}

	if (alpha) {
		long arr[strlen(x)+1]; 
		alphatonum(x,arr);
		printf("alphabetic: %s => ", x);
	        for (int i = 0; arr[i]!='\0'; i++) {
			printf("%li",arr[i]);
		}
		printf("\n");
	}

	else if (num) {
		int l = strlen(x);
		//Remember to malloc pointer space
		struct elt ** gen = malloc (sizeof(struct elt *));
		*gen = NULL;
		char str[l+1];
		str[0] = '\0';
		genComb(gen, str, x, 0, l);
		reversels(gen);
		Hash h = HashCreate();
		loadD(h, l, file, fileN);
		checkD(h, (*gen), x);

		//freeing
		struct elt * iter = *gen;
		struct elt * next;
		while (iter!=NULL) {
			next = iter->next;
			free(iter->key);
			free(iter);
			iter = next;
		}
		free(gen);
		HashDestroy(h);

	}
	return 0;
}

