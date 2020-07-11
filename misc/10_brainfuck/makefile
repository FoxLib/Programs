CC=g++
SDL=`sdl-config --cflags --libs`
LIBR=-ljpeg -lSDL -lm -lglut -lGL -lGLU -lGLEW -lcurl
WARN=-Wall -Wno-strict-overflow -Wno-unused-result
CFLAGS=$(SDL) -Iclass -Ofast

all: main.o bf.o
	$(CC) $(CFLAGS) bf.o main.o $(LIBR) $(WARN) -o main

main.o: main.cc
	$(CC) $(CFLAGS) main.cc -c -o main.o

bf.o: bf.cc bf.h
	$(CC) $(CFLAGS) bf.cc -c -o bf.o

clean:
	rm *.o
