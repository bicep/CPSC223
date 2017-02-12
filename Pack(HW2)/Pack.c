/* 
 Plan.c

 CS 223b, hw2  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h1.v0.html

 Name: <Ko Tin Yue (Roger)>
 netid: <tk553>

 We have included some useful libraries.

 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool isFlag(char * str) {
	if (strcmp(str, "-next")==0) {return true;}
	else if (strcmp(str, "-first")==0) {return true;}
	else if (strcmp(str, "-best")==0) {return true;}
	else if (strcmp(str, "-ffd")==0) {return true;}
	else if (strcmp(str, "-bfd")==0) {return true;}
	else if (strcmp(str, "-optm")==0) {return true;}
	else if (strcmp(str, "-trace")==0) {return true;}
	else {return false;}
} 

bool allZero(char * str) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] != '0') {return false;}
	}
	return true;
}

bool isInt(char * str) {
	//Not sure why but
	if (allZero(str)) {return false;}
	for (int i = 0; str[i] != '\0'; i++) {
		if(!isdigit(str[i])) {return false;}
	}
	return true;
}

bool isLim(char * str) {
	int i = atoi(str);
	if (i<=100 && i>=0) {return true;}
	else {return false;}
}

void getItems(int argc, char ** argv, int arr[]) {
	int counter = 0;
	for (int i = 1; i<argc; i++) {
		if (isFlag(argv[i])) {
			//If there is any point after flag that is NOT flag, then raise error
			//get all the flags out of the way
			while(i<argc-1) {
				i++;
				if (!isFlag(argv[i])) {fprintf(stderr, "Fatal error: Size option out of order: %s \n", argv[i]); exit(0);}
			}
		}
		else if (!isInt(argv[i])) {fprintf(stderr, "Fatal error: bad argument: %s \n", argv[i]); exit(0);}
		else if (!isLim(argv[i])) {fprintf(stderr, "Fatal error: Invalid size: %s \n", argv[i]); exit(0);}
		else {arr[counter] = atoi(argv[i]); counter++;}
	}
	if (counter > 0 && counter < (argc-1)) {arr[counter] = '\0';}
}

void traceF(int i, int item, int bin, int cap) {
	printf("arg: %d val: %d bin: %d total: %d\n", i, item, bin, 100-cap);
}

int nextFit(int items[], int maxbin, bool trace) {
	int bins = 0;
	int prevbin=0;
	int cap[maxbin];
	for (int i = 0; i<maxbin; i++) {
		cap[i] = 0;
	}
	for (int i = 0; items[i] != '\0'; i++) {
		if (items[i]<=cap[prevbin]) {cap[prevbin] = cap[prevbin]-items[i];if(trace) {traceF(i,items[i],prevbin,cap[prevbin]);}}
		else {
			bins++; 
			cap[bins-1] = 100-items[i];
			if (trace) {traceF(i,items[i],bins-1,cap[bins-1]);}
			prevbin = bins-1;
		}
	}
	return (bins);
}

int getBin(int item, int * cap, int maxbin) {
	for (int i = 0; i<maxbin; i++) {
		if (item <= cap[i]) {return i;}
	}
	return -1;
}

int firstFit(int items[], int maxbin, bool trace) {
	int bins = 0;
	int bin;
	int cap[maxbin];
	for (int i = 0; i<maxbin; i++) {
		cap[i] = 0;
	}
	for (int i = 0; items[i] != '\0'; i++) {
		if ((bin = getBin(items[i],cap,maxbin)) != -1) {cap[bin] = cap[bin] - items[i]; if(trace) {traceF(i,items[i],bin,cap[bin]);}}
		else {bins++; cap[bins-1] =100; cap[bins-1] = 100 - items[i]; if(trace) {traceF(i,items[i],bins-1,cap[bins-1]);}}	
	}
	return(bins);
}

int bestFit(int items[], int maxbin, bool trace) {
	int bins = 0;
	int minbin;
	int mincap;
	int cap[maxbin];
	for (int i = 0; i<maxbin; i++) {
		cap[i] = 0;
	}
	//for each item i
	for (int i = 0; items[i] != '\0'; i++) {
		minbin = -1;
		mincap = 101;
		for (int j = 0; j<maxbin; j++) {
			if ((items[i] <= cap[j]) && (cap[j] < mincap)) {minbin = j; mincap = cap[j];}
		}
		//This means minbin didn't change- no bin can accomodate
		if (minbin == -1) {bins++; cap[bins-1] = 100-items[i]; if(trace) {traceF(i,items[i],bins-1,cap[bins-1]);}}
		else {cap[minbin] = cap[minbin]-items[i]; if(trace) {traceF(i,items[i],minbin,cap[minbin]);}}
	}
	return(bins);
}

//Descending 
int cmpfuncd (const void * a, const void * b) {
	   return ( *(int*)b - *(int*)a );
}	

//Ascending
int cmpfunca (const void * a, const void * b) {
	   return ( *(int*)a - *(int*)b );
}	

int * sortd(int items[]) {
	int size = 0;
	for (int i = 0; items[i] != '\0'; i++) {
		size++;
	}	

	qsort(items,size,sizeof(int),cmpfuncd);
	return(items);
}

int * sorta(int items[]) {
	int size = 0;
	for (int i = 0; items[i] != '\0'; i++) {
		size++;
	}	

	qsort(items,size,sizeof(int),cmpfunca);
	return(items);
}

int ffdFit(int items[], int maxbin, bool trace) {
	int * sorteditems = sortd(items);
	return(firstFit(sorteditems,maxbin,trace));
}

int bfdFit(int items[], int maxbin, bool trace) {
	int * sorteditems = sortd(items);
	return(bestFit(sorteditems,maxbin,trace));
}

int pop (int arr[], int n) {
	int temp = arr[n-1];
	arr[n-1] = '\0';
	return(temp);
}

int minimum (int a, int b) {
	if (a<b) {return (a);} 
	else {return (b);}
}

void cpyarr (int p[], int src[], int n) {
	for (int i = 0; i<n; i++) {
		p[i] = src[i];
	}
}

//backtracking
int optFit(int items[], int cap[], int nitems, int maxbin, bool trace, int minbin) {
	if (nitems==0) {return minbin;}
	int min = nitems+1000;
	int minj;
	int item = items[nitems-1];
	for (int j = 0; j<maxbin; j++) {
		//if there is a bin that item can be fitted into
		if (item<=cap[j]) {
			//place the item in that bin
			cap[j] = cap[j] - item;
			//recursively find minbin for remaining items
			int opt = optFit(items,cap,nitems-1,maxbin,trace,minbin);
			if (opt < min) {minj = j;}
			min = minimum(opt,min);
			//remove item from bin (reset)
			cap[j] = cap[j] + item;
		}
	}
	//If no change in min that means that item didn't fit into any bin
	if (min == (nitems+1000)) {
		//Add new bin
		minbin++; 
		cap[minbin-1] = 100-item; 
		if (trace) {traceF(maxbin-nitems,item,minbin-1,cap[minbin-1]);}
		//recursively find minbin for remining items
		min = optFit(items,cap,nitems-1,maxbin,trace,minbin);
	}
	else{if (trace) {traceF(maxbin-nitems,item,minj,cap[minj]);}}
	return(min);
}
/*	
int bp (…) {
	if all items have been placed in bins
		return the total number of bins used
	for each bin that we can place the next item in
		place the item in that bin
		call bp() recursively to find the minimum number of bins used to store all of the items given that the items already stored cannot be moved
		remove the item from the bin
	return the minimum number of bins needed
}
*/
int main(int argc, char **argv) {

	if (argc == 1) {printf("usage: Pack [sizes]* [-next | -first | -best | -ffd | -bfd | -optm]+ -trace* \n"); return 0;}

	int items[argc-1];
	//memset(items,'\0',argc-1);
	getItems(argc,argv,items);
	int nitems = 0;
	bool next = false;
	bool first = false;
	bool best = false;
	bool ffd = false;
	bool bfd = false;
	bool optm = false;
	bool trace = false;
 
	for (int i  = 0; i<argc; i++) {
		if (strcmp(argv[i], "-next")==0) {next = true;}
		else if (strcmp(argv[i], "-first")==0) {first = true;}
		else if (strcmp(argv[i], "-best")==0) {best = true;}
		else if (strcmp(argv[i], "-ffd")==0) {ffd = true;}
		else if (strcmp(argv[i], "-bfd")==0) {bfd = true;}
		else if (strcmp(argv[i], "-optm")==0) {optm = true;}
		else if (strcmp(argv[i], "-trace")==0) {trace = true;}
	}

	if (!next && !first && !best && !ffd && !bfd && !optm) {fprintf(stderr, "Fatal error: no algorithm specified.\n"); return -1;}
	
	for (int i = 0; items[i]!='\0';i++) {
		nitems++;
	}
	
	int nextb;
	int firstb;
	int bestb;
	int ffdb;
	int bfdb;
	int optmb;

	if (next) {if (trace) {printf("Trace -next\n");} nextb = nextFit(items,(argc-1),trace);}	
	if (first) {if (trace) {printf("Trace -first\n");} firstb = firstFit(items,(argc-1),trace);}
	if (best) {if (trace) {printf("Trace -best\n");} bestb = bestFit(items,(argc-1),trace);}
	if (ffd) {if (trace) {printf("Trace -ffd\n");} ffdb = ffdFit(items,(argc-1),trace);}
	if (bfd) {if (trace) {printf("Trace -bfd\n");} bfdb = bfdFit(items,(argc-1),trace);}
	if (optm) {
		//sort ascending because item is popped from the end of the arr
		if (trace) {printf("Trace -optm \n");}
		int * sorteditems = sorta(items);	
	/*	for (int i = 0; items[i]!='\0';i++) {
			printf("item: %d \n", items[i]);
	}*/
		int cap[nitems];	
		for (int i = 0; i<nitems; i++) {
			cap[i] = 0;
		}
		optmb = optFit(sorteditems,cap,nitems,nitems,trace,0); 
	}

	if (next) {printf("-next: %d\n", nextb);}
	if (bfd) {printf("-bfd: %d\n", bfdb);}
	if (best) {printf("-best: %d\n", bestb);}
	if (ffd) {printf("-ffd: %d\n", ffdb);}
	if (first) {printf("-first: %d\n", firstb);}
	if (optm) {printf("-optm: %d\n", optmb);}





	return 0;
}

