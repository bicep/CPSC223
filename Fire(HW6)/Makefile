## Makefile

CC     = gcc
#CFLAGS = -g3 -std=gnu99 -pedantic -Wall
CFLAGS = -g3 -std=c99 -pedantic -Wall
HWK    = /c/cs223/hw6

Fire:  Fire.o dict.o heap.o
	${CC} ${CFLAGS} -o Fire Fire.o dict.o heap.o

Fire.o: Fire.c
	${CC} ${CFLAGS} -c Fire.c

heap.o: heap.c /c/cs223/hw6/heap.h
	${CC} ${CFLAGS} -c heap.c

dict.o: /c/cs223/hw6/dict.c /c/cs223/hw6/dict.h
	${CC} ${CFLAGS} -c /c/cs223/hw6/dict.c

heaptest:  heaptest.o heap.o
	${CC} ${CFLAGS} -o heaptest heaptest.o heap.o

clean:
	rm -i *.o *~

