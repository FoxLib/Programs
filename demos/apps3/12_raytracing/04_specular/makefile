CC=g++
LIBR=-lm
WARN=-Wall -Wno-strict-overflow -Wno-unused-result -Wno-narrowing
CFLAGS=-I.. -I. -Ofast
OBJ=ppm.o raycast.o main.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBR) $(WARN) -o main

main.o: main.cc main.h
	$(CC) $(CFLAGS) main.cc -c $(WARN) -o main.o

raycast.o: raycast.cc raycast.h
	$(CC) $(CFLAGS) raycast.cc -c $(WARN) -o raycast.o

ppm.o: ../ppm.h ../ppm.cc
	$(CC) $(CFLAGS) ../ppm.cc -c $(WARN) -o ppm.o

clean:
	rm *.o