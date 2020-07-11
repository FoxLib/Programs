CC=g++
LIBS=-lm
WARN=-Wall -Wno-strict-overflow -Wno-unused-result
CFLAGS=-I. -O3
OBJ=number.o main.o 

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(WARN) -o main

main.o: main.cc
	$(CC) $(CFLAGS) main.cc -c -o main.o

number.o: number.cc number.h
	$(CC) $(CFLAGS) number.cc -c -o number.o

clean:
	rm *.o