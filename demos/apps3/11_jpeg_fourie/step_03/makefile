CC=g++
LD=ld
SDL=`sdl-config --cflags --libs`
LIB=-lSDL -lm 
INC=-I..
CONF=-Wall -Wno-unused-value -Wno-unused-result
OPT=-O3
OBJ=jpeg.o sdlclass.o main.o

all: $(OBJ)
	$(CC) $(CONF) $(OBJ) $(LIB) $(SDL) -o main

main.o: main.cc
	$(CC) $(SDL) $(LIB) $(INC) $(CONF) $(OPT) -c main.cc -o main.o

sdlclass.o: ../sdl.class.h ../sdl.class.cc
	$(CC) $(SDL) $(LIB) $(INC) $(CONF) $(OPT) -c ../sdl.class.cc -o sdlclass.o

jpeg.o: jpeg.h jpeg.cc
	$(CC) $(LIB) $(CONF) $(OPT) -c jpeg.cc -o jpeg.o

clean:
	rm *.o