#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct token {
  int type;
  double value;
  char * str;
};

typedef struct token *Token;

// enumerated types for tokens:
enum {NUM, OP1, OP2, LPAR, RPAR};

// global variable for debugging
bool debugflag;

typedef struct {
  Token *contents;
  int maxSize;
  int top;
  int count;
} stackT;

char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}


void StackInit (stackT *stackP, int maxSize) {
	stackP->maxSize = maxSize;
	Token * c = malloc(sizeof(Token)*maxSize);
	stackP->contents = c;
	stackP->top = -1;
}

bool StackIsEmpty (stackT *stackP) {
	if (stackP->top == -1) {return true;}
	else {return false;}
}


bool StackIsFull (stackT * stackP) {
	if (((stackP->top)+1) >= stackP->maxSize) {return true;}
	else {return false;}
}

void StackDestroy (stackT * stackP) {
	if (!StackIsEmpty(stackP)) {
		for (int i = 0; i<stackP->top+1; i++) {
			free(stackP->contents[i]->str);
			free(stackP->contents[i]);
		}
	}
	free(stackP->contents);
}

void StackPush (stackT * stackP, Token element) {
	//check if full
	if(StackIsFull(stackP)) {
		fprintf(stderr, "Can't push element on stack: stack is full.\n");
		exit(0);
	}	
	//else push it into array increase top
	else {
		(stackP->top)++;
		stackP->contents[stackP->top] = element;
	}
}

Token StackPop (stackT * stackP) {
	//check if empty
	if(StackIsEmpty(stackP)) {
		fprintf(stderr, "Can't pop element on stack: stack is empty.\n");
		exit(0);
	}
	//else pop decrease top and return the token 
	else {
		Token t = stackP->contents[stackP->top];
		(stackP->top--);
		return (t);
	}
}

int StackCount (stackT * stackP) {
	if (StackIsEmpty(stackP)) {return 0;}
	else {return ((stackP->top)+1);}
}

Token StackTop (stackT * stackP) {
	if (StackIsEmpty(stackP)) {
		fprintf(stderr,"No element on top of the stack.");
		return(NULL);
	}
	else {
		return(stackP->contents[stackP->top]);
	}
}

void StackPrint(stackT * stackP) {
	printf("Stack: size: %d :",StackCount(stackP));
	for (int i = stackP->top; i>-1; i--) {
		Token t = (stackP->contents)[i];
		printf("[%d %s %.1f] ",t->type,t->str,t->value);
	}
	printf("\n");
}
