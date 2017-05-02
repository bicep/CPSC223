#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "/c/cs223/hw4/stack.h"


bool allnum(char * str) {
	bool decflag = false;
	for (int i = 0; str[i]; i++) {
		if (!isdigit(str[i]) && str[i]!='.' && str[i]!='-') {return false;}
		else if (str[i]=='.' && !decflag) {decflag=true;}
		else if (str[i]=='.' && decflag) {return false;}
		else if (str[i]=='-' && i!=0) {return false;}
	}
	return true;
}

bool islpar(char * str) {
	if (str[0] == '(' && str[1] == '\0') {return true;}
	else {return false;}
}

bool isrpar(char * str) {
	if (str[0] == ')' && str[1] == '\0') {return true;}
	else {return false;}
}

bool isop1(char * str) {
	if ((str[0] == '+' || str[0] == '-') && str[1] == '\0') {return true;}
	else {return false;}
}

bool isop2(char * str) {
	if ((str[0] == '/' || str[0] == '*') && str[1] == '\0') {return true;}
	else {return false;}
}

bool isvalid(char * str) {
	if (allnum(str) || islpar(str) || isrpar(str) || isop1(str) || isop2(str)) {return true;}
	else {return false;}
}	

char *strdup (const char *s); 

void ptoken (Token t) {
	printf("Token:%s: type: %d value: %.2f\n", t->str,t->type,t->value);
}


//Queue implementation

struct elt {
    Token t;
    struct elt * next;
};

struct queue {
    struct elt *head;
    struct elt *tail;
};

struct queue * queueCreate(void)
{
    struct queue *q;

    q = malloc(sizeof(struct queue));

    q->head = q->tail = 0;

    return q;
}

/* push a new value onto top of Queue */
void enq(struct queue *q, Token t)
{
    struct elt *e;

    e = malloc(sizeof(struct elt));
    assert(e);

    e->t = t;

    /* Because I will be the tail, nobody is behind me */
    e->next = 0;

    if(q->head == 0) {
        /* If the queue was empty, I become the head */
        q->head = e;
    } else {
        /* Otherwise I get in line after the old tail */
        q->tail->next = e;
    }

    /* I become the new tail */
    q->tail = e;
}

int queueEmpty(const struct queue *q)
{
    return (q->head == 0);
}

int queueCount(struct queue *q) {
	    int count = 0;
	    struct elt * iter = q->head;
	    while(iter!=NULL) {
		    count++;
		    iter=iter->next;
	    }
	    return(count);
}

Token deq(struct queue *q)
{
    Token ret;
    struct elt *e;

    assert(!queueEmpty(q));

    ret = q->head->t;

    /* patch out first element */
    e = q->head;
    q->head = e->next;

    free(e);

    return ret;
}

/* print contents of queue on a single line, head first */
void queuePrint(struct queue *q)
{
    struct elt *e;

    for(e = q->head; e != 0; e = e->next) {
       ptoken (e->t);
    }
}

/* free a queue and all of its elements */
void queueDestroy(struct queue *q)
{
    while(!queueEmpty(q)) {
        Token t = deq(q);
	free(t->str);
	free(t);
    }

    free(q);
}

double eval(double a, double b, char * s) {
	if (!strcmp(s,"+")) {
		return(a+b);
	}
	else if (!strcmp(s,"-")) {
		return(b-a);
	}
	else if (!strcmp(s,"*")) {
		return(a*b);
	}
	else {
		return(b/a);
	}
}

Token tokendup(Token t) {
	Token ret = malloc(sizeof(struct token));
	ret->str=strdup(t->str);
	ret->value=t->value;
	ret->type=t->type;
	return(ret);
}

int main(int argc, char **argv){
	
	debugflag = false;
	if (argc==2 && !strcmp(argv[1],"-debug")) {debugflag=true;}
	else if (argc>1) {fprintf(stderr, "Usage: Calc [-debug]\n");exit(1);}
	
	//Get input and make sure input is safe
	int bsize = 1000;
	char buffer[bsize];
	bool eoff = false;
	while(1) {
		char ch;
		int i = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF)) {
			buffer[i++] = ch;
		}

		if (ch == EOF) {eoff=true;}
		if (eoff) {exit(1);}	

		buffer[i]='\0';
		printf("Input: %s\n",buffer);
		
		//st will contain all the inputs
		stackT st;
		StackInit(&st, bsize);

		//Get the first token string
		char * toks = strtok(buffer," ");
		if (toks==NULL) {fprintf(stderr,"Fatal error: Too many operands\n"); StackDestroy(&st); exit(0);}
		while(toks != NULL) {
			if (!isvalid(toks)) {fprintf(stderr,"Fatal error: Bad token: %s\n",toks); StackDestroy(&st); exit(0);}
			//make token
			Token t = malloc(sizeof(struct token));
			//These should all be one of the types
			if (allnum(toks)) {t->type=NUM;}
			if (isop1(toks)) {t->type=OP1;}
			if (isop2(toks)) {t->type=OP2;}
			if (islpar(toks)) {t->type=LPAR;}
			if (isrpar(toks)) {t->type=RPAR;}
			t->str = strdup(toks);
			t->value = strtod(t->str,NULL);

			StackPush(&st, t);
			//Get the next token string
			toks = strtok(NULL," ");
		}
		
		//Create output queue and operator stack
		struct queue * opq = queueCreate();
		stackT ops;
		StackInit(&ops,bsize);
		
		//Shunting yard algorithm to get reverse polish notation
		for (int i = 0; i<=(st.top); i++) {
			Token temp = st.contents[i];
			Token t = tokendup(temp);
			if (debugflag) {
				StackPrint(&ops);
				ptoken(t);
			}
			//if token is number
			if (t->type==NUM) {
				//pop to output q
				enq(opq,t);
			}
			//else if token is operator
			else if (t->type==OP1 || t->type==OP2) {
				Token op2;
				//while there is operator at the top of the op stack o2
				while ((!StackIsEmpty(&ops)) && ((op2=StackTop(&ops)) != NULL) && (op2->type>=t->type) && (op2->type==OP2 || op2->type==OP1) && (queueCount(opq)>1)) {
					//if len stack >1 and o2 has precedence then enq the operator

					enq(opq, StackPop(&ops));
				}
				//else push the operator onto the stack
				StackPush(&ops, t);
			}
			//else if token is (
			else if (t->type==LPAR) {
				//push to operator stack
				StackPush(&ops,t);
			}
			//else if token is )
			else if (t->type==RPAR) {
				Token tk;
				//pop everything from operator stack onto output q until (
				while((!StackIsEmpty(&ops)) && ((tk=StackTop(&ops))!=NULL) && (tk->type!=LPAR)) {
					
					enq(opq, StackPop(&ops));
				}
				//if no (, then mismatch
				if (tk->type!=LPAR) {
					fprintf(stderr,"Fatal error: missing left paren.\n");
					free(t->str);
					free(t);
					StackDestroy(&st);
					StackDestroy(&ops);
					queueDestroy(opq);
					exit(0);
				}
				//Do not put ( onto the output queue (just discard it)
				else {
					free(t->str);
					free(t);
					Token othert = StackPop(&ops);
					free(othert->str);
					free(othert);			
				}
			}
		}
		if (debugflag) {StackPrint(&ops);}
		
		//there are no more tokens:
		//if operator at the top of the stack is paran, mismatch
		if (!StackIsEmpty(&ops) && (StackTop(&ops)->type==LPAR || StackTop(&ops)->type==RPAR)) {
			fprintf(stderr,"Error: Mismatched parentheses\n");
			StackDestroy(&st);
			StackDestroy(&ops);
			queueDestroy(opq);
			exit(0);
		}
		//pop the operators onto the output queue
		while(!StackIsEmpty(&ops)) {
			enq(opq,StackPop(&ops));
		}
	
		//Use stacks to calculate value based on RPN

		if (debugflag) {printf("OUTPUT:\n");}
		stackT calcs;
		StackInit(&calcs, bsize);
		while(!queueEmpty(opq)) {
			Token t = deq(opq);
			if (debugflag) {
				StackPrint(&calcs);
				ptoken(t);
			}
			if (t->type==NUM) {
				StackPush(&calcs,t);
			}
			//has to be an operator
			else {
				if (StackCount(&calcs)>1) {
					Token t1 = StackPop(&calcs);
					Token t2 = StackPop(&calcs);
					double val = eval(t1->value,t2->value,t->str);
					t1->type=NUM;
					t1->value=val;
					char * s = malloc(bsize*sizeof(char));
					sprintf(s,"%.1f", val);
					free(t1->str);
					t1->str=s;
					StackPush(&calcs,t1);
					//free t2 and free t
					free(t2->str);
					free(t2);
					free(t->str);
					free(t);
				}
				else {
					fprintf(stderr,"Fatal error: fewer than 2 operands available.\n");
					free(t->str);
					free(t);
					StackDestroy(&st);
					StackDestroy(&ops);
					queueDestroy(opq);
					exit(0);
				}
			}
		}

		if (StackCount(&calcs)==1) {
			Token t =StackPop(&calcs);
			double val = t->value;
			printf("Result: %.2f\n",val);
			free(t->str);
			free(t);
		}
		else {
			fprintf(stderr,"Fatal error: Too many operands\n");
		}
		queueDestroy(opq);
		StackDestroy(&ops);
		StackDestroy(&st);
		StackDestroy(&calcs);
	}

}


