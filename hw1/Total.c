/* 
 Total.c

 CS 223b, hw1  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h1.v0.html

 Name: <Ko Tin Yue (Roger)>
 netid: <tk553>

 start with Total.c: simple C program: counts characters from input
 
 We have included some useful libraries.

 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define ungetchar(c)  ungetc(c, stdin)    // unread char read from stdin


// here is where you can gobble up line splices
int mygetchar() {
  int ch = getchar();
  //handle line splices as if there is nothing there
  if (ch == '\\') {
	  ch = getchar();
	  if (ch == '\n') {ch = getchar();}
	  else {if (ch == '\"') {exit(0);} else{ungetchar(ch); ch = '\\';}}
  }
  //handle double quote strings, skipping escaped characters
  else {
	  if (ch == '\"') {
	  	ch = getchar();
	  	while (ch != '\"' && ch != EOF) {
			if (ch == '\\') {ch = getchar();}
			ch = getchar();}
		if (ch == '\"') {ch = getchar();}
	  }}
  return ch;
}


//returns null if this is not an int. This function handles ints other than 0
char * getintstr(int ch, char buffer[]) {
  int count = 0;
  //deal with first char: if it's an int, only - or 1-9 can be the first char
  if (!isdigit(ch) || (ch == '0')) {return NULL;}
  buffer[count] = ch;
  count++;
  while((ch = mygetchar()) != EOF) {
	  if (!isdigit(ch)) {ungetchar(ch); return buffer;}
          buffer[count] = ch;
	  count++;
  }
  return buffer;
}

bool isoct(char ch) {
  if (ch>'7' || ch<'0') {return false;}
  else {return true;}
}

//returns null if this is not an oct. This function handles (neg/pos) oct and standalone 0
char * getoctstr(int ch, char buffer[]) {
  int count = 0;
  //deal with first char; if it is not 0 then this is not octal
  if (ch != '0') {return NULL;}
  buffer[count] = ch;
  count++;
  while((ch = mygetchar()) != EOF) {
	  //if we stop meeting an octal digit, then we return what we've got
	  if (!isoct(ch)) {ungetchar(ch); return buffer;}
          buffer[count] = ch;
	  count++;
  }
  return buffer;
}

//atoi for oct string
long getdecfromoct(char buffer[]) {
  long n = 0;
  //Note i starts with 1 because the first number would be 0 
  for (int i = 0; buffer[i] != '\0'; i++) {
	  n*=8; 
	  n += (buffer[i]-'0');
  }
  return n;
}

bool isbn(char ch) {
  if (ch>'1' || ch<'0') {return false;}
  else {return true;}
}

//returns null if not binary. Function handles binary
char * getbnstr(int ch, char buffer[]) {
  int count = 0;
  int firstch = ch;
  ch = mygetchar();
  int secondch = ch;
  //check first char and second char
  if ((firstch != '0') || ((secondch != 'b') && (secondch != 'B'))) {ungetchar(ch); return NULL;}
  buffer[count] = firstch;
  count++;
  buffer[count] = secondch;
  count++;
  while((ch = mygetchar()) != EOF) {
	  if (!isbn(ch)) {ungetchar(ch); return buffer;}
          buffer[count] = ch;
	  count++;
  }
  return buffer;
}

//atoi for bn string
long getdecfrombn(char buffer[]) {
  long n = 0;
  if (buffer[2] == '\0') {return 0;}
  for (int i = 2; buffer[i] != '\0'; i++) {
	  if (buffer[i] == '1') {n = n*2 +1;}
	  else {if (buffer[i] == '0') {n = n*2;}}
  }
  return n;
}

//returns null if not hex. Function handles hex
char * gethxstr(int ch, char buffer[]) {
  int count = 0;
  int firstch = ch;
  ch = mygetchar();
  int secondch = ch;
  //check first and second char
  if ((firstch != '0') || ((secondch != 'x') && (secondch != 'X'))) {ungetchar(ch); return NULL;}
  buffer[count] = firstch;
  count++;
  buffer[count] = secondch;
  count++;
  while((ch = mygetchar()) != EOF) {
	  if (!isxdigit(ch)) {ungetchar(ch); return buffer;}
          buffer[count] = ch;
	  count++;
  }
  return buffer;
}

//atoi for hx string
long getdecfromhx(char buffer[]) {
  long number = (long)strtol(buffer, NULL, 0);
  return number; 
}

int main(int argc, char **argv) {
  bool debug = false;
  if (argc == 2 && strcmp(argv[1], "-debug") == 0) {
    debug = true;
  }
  int ch;
  long total = 0;
  int bsize = 100;
  char buffer[bsize];
  while ((ch = mygetchar()) != EOF) {
    //clear buffer at the start
    memset(buffer,'\0',bsize);
    int negp = 1;
    //get all the neg out of the way
    while (ch == '-') {negp = -1; ch = mygetchar();}
    char * intstr = getintstr(ch,buffer);
    if (intstr != NULL) {total += (negp*atol(intstr));}
    else {
	    char * bnstr = getbnstr(ch,buffer);
	    if (bnstr != NULL) {total += (negp*getdecfrombn(bnstr));}
	    else {
		    char * hxstr = gethxstr(ch,buffer);
		    if (hxstr != NULL) {total += (negp*getdecfromhx(hxstr));}
		    else {
			    char * octstr = getoctstr(ch,buffer);
			    if (octstr != NULL) {total += (negp*getdecfromoct(octstr));}}}}

    /*
    if (debug && (total % 100 == 0))
      printf("!");*/
}

  if (debug) puts("\n");
  printf("Total: %ld\n", total);
}

