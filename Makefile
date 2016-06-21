CC = g++
CFLAGS = -Wall -g -std=c++11

all: main

clean:
	rm -rf *o
	rm getopt_ex

main.o:
	${CC} ${CFLAGS} -c getopt_ex.cc

main: main.o
	${CC} ${CFLAGS} getopt_ex.o -o getopt.out
 

	
