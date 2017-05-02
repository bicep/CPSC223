#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "/c/cs223/hw7/hash.h"

bool isValid (char * str, Hash h) {
	if (HashSearch(h,str)) {return true;}
	else {return false;}
}

char * substr (char * dest, char * src, int start, int end) {
	int j = 0;
	for (int i = start; i<end; i++) {
		dest[j] = src[i];
		j++;
	}
	dest[j] = '\0';
	return dest;
}

bool isalphabet (char * s) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (!isalpha(s[i])) {return false;}
	}
	return true;
}

void strtolower (char s[]) {
	for (int i=0; s[i]!='\0'; i++) {
		s[i] = tolower(s[i]);
	}
}

int main (int argc, char ** argv) {

	bool debug = false;
	char defaultd[] = "words";
	char * dictN = defaultd;
	if (argc>1) {
		for (int i = 1; i<argc; i++) {
			if (!strcmp(argv[i],"-debug")) {debug=true;}
			else if(!strcmp(argv[i],"-dict")) {
				if (i>=argc-1) {}
				else {
					//load dictionary
					dictN = argv[++i];
				}
			}
			else {fprintf(stderr,"usage: Words [-dict filename | -debug]\n");exit(0);}
		}
	}

	//load dictionary
	Hash h = HashCreate();
	FILE * f;
	if (debug) {printf("Loading dictionary: %s\n", dictN);}
	f = fopen(dictN,"r");
	char line[256];
	int wcount = 0;
	while(fgets(line,sizeof(line),f)) {
			int len = strlen(line);
			line[len-1]='\0';
			if(isalphabet(line)) {
				strtolower(line);
				if(!HashSearch(h,line)) {
					HashInsert(h,line);
					wcount++;
				}
			}
	
	
	}
	fclose(f);
	if (debug) {printf("Word count: %d\n",wcount);}

	int bsize = 1000;
	char ch;
	//Let k be the length of the string we're segmenting ("breakingupishardtodo"). Use arr[] to store intermediate results.
	while((ch = getchar()) != EOF) {
		char str[bsize];
		int k = 0;	
		while((ch!='\n') && (ch!=EOF)) {
			str[k++] = ch;
		        ch = getchar();	
		}
		str[k] = '\0';	
		//Now, buffer contains the string
		//k contains the number of characters+1		
		if(debug) {printf("Input: '%s'\n",str);}

		//two word solution:
		char s1[k];
		char s2[k];
		printf("Two words: ");
		if (k>0) {
			int i;
			for (i = 1; i<k; i++) {
				char *sub1 = substr(s1,str,0,i);
				char *sub2 = substr(s2,str,i,k);
				if (isValid(sub1,h) && isValid(sub2,h)) {printf("SUCCESS: %s %s.  \n",sub1,sub2); break;}
			}
			if (i==k) {printf("FAILURE\n");}
		}
		else {printf("FAILURE\n");}

		//DP solution:
		//More generally, for every index i going backwards, check if the substring starting with str[i] is a valid word -- if so, set arr[i] = k. Else, for every possible split at j , check both that str[i]:str[j] is a word and that there's a valid split starting with str[j] (which means that arr[j] != -1). Note: slight revision per Sachith.
		int arr[k];
		for (int i=(k-1); i>=0; i--) {
			char sub[k];
			if (isValid(substr(sub,str,i,k),h)) {arr[i] = k;}
			else {
				int j;
				//for every possible split j
				for (j=(i+1); j<k; j++) {
					if (isValid(substr(sub,str,i,j),h) && arr[j]!=-1) {arr[i] = j; break;}
				}

				if (j == k) {arr[i] = -1;}
			}
		}

		//resurrect string
		//When this process is done, we can check arr[0] to see if we found a valid split. If it has some j != -1, then we can retrieve the split by printing str[0:j], then going to str[j:arr[j]] and printing that, and so on until we reach the end of the string.
		printf("DP:");
		if (k) {
			int index = 0;
			if (arr[index] != -1) {
				printf(" SUCCESS:");
				char sub[k];
				while (index != k) {
					printf(" %s",substr(sub,str,index,arr[index]));
					index = arr[index];
				}
				printf("\n");
				
				if (debug) {
					for (int l=0; l<k; l++) {
						printf("%d ",arr[l]);
					}
					printf("\n");
				}
	
			}
			else {printf(" FAILURE\n");}
		}
		else {printf(" SUCCESS:\n\n");}
		printf("---\n");	
	}
	HashDestroy(h);
	return 0;
}
