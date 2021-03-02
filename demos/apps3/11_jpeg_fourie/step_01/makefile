CC=g++
LD=ld
SDL=`sdl-config --cflags --libs`
LIB=-lSDL -lm 
INC=-I..
CONF=-Wall -Wno-unused-value

all: main.o sdlclass.o
	$(CC) $(CONF) sdlclass.o main.o $(LIB) $(SDL) -o main

main.o: main.cc
	$(CC) $(SDL) $(LIB) $(INC) $(CONF) -c main.cc -o main.o

sdlclass.o: ../sdl.class.h ../sdl.class.cc
	$(CC) $(SDL) $(LIB) $(INC) $(CONF) -c ../sdl.class.cc -o sdlclass.o
