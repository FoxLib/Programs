CC=g++
LIBR=-lm
WARN=-Wall -Wno-strict-overflow -Wno-unused-result -Wno-narrowing -Wno-unused-but-set-variable -Wno-unused-variable
CFLAGS=-I.. -I. -Ofast
OBJ=ppm.o object.o scene.o raycast.o main.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBR) $(WARN) -o main

main.o: main.cc main.h
	$(CC) $(CFLAGS) main.cc -c $(WARN) -o main.o

raycast.o: raycast.cc raycast.h
	$(CC) $(CFLAGS) raycast.cc -c $(WARN) -o raycast.o

object.o: object.cc object.h
	$(CC) $(CFLAGS) object.cc -c $(WARN) -o object.o

scene.o: scene.cc scene.h
	$(CC) $(CFLAGS) scene.cc -c $(WARN) -o scene.o

ppm.o: ../ppm.h ../ppm.cc
	$(CC) $(CFLAGS) ../ppm.cc -c $(WARN) -o ppm.o

clean:
	rm *.o