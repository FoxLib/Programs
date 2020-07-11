CC=g++
SDL=`sdl-config --cflags --libs`
LIBR=-lm
WARN=-Wall -Wno-strict-overflow -Wno-unused-result
CFLAGS=$(SDL) -I.. -Ofast

all: main.o ppm.o
	$(CC) $(CFLAGS) ppm.o main.o $(LIBR) $(WARN) -o main

main.o: main.cc main.h
	$(CC) $(CFLAGS) main.cc -c -o main.o

ppm.o: ../ppm.h ../ppm.cc
	$(CC) $(CFLAGS) ../ppm.cc -c -o ppm.o

clean:
	rm *.o