CC=g++
LD=ld
SDL=`sdl-config --cflags --libs`
LIB=-lSDL -lm 
INC=-I..
CONF=-Wall -Wno-unused-value -Wno-unused-result
OPT=-O3

all: main.o sdlclass.o jpeg.o
	$(CC) $(CONF) jpeg.o sdlclass.o main.o $(LIB) $(SDL) -o main

main.o: main.cc
	$(CC) $(SDL) $(LIB) $(INC) $(OPT) $(CONF) -c main.cc -o main.o

sdlclass.o: ../sdl.class.h ../sdl.class.cc
	$(CC) $(SDL) $(LIB) $(INC) $(OPT) $(CONF) -c ../sdl.class.cc -o sdlclass.o

jpeg.o: jpeg.h jpeg.cc
	$(CC) $(LIB) $(OPT) $(CONF) -c jpeg.cc -o jpeg.o

clean:
	rm *.o