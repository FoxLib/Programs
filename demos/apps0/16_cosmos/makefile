CC=g++
LD=ld
SDL=`sdl-config --cflags --libs`
LIB=-lSDL -lm -ljpeg
INC=-I../../classes -I.
CONF=-Wall -Wno-unused-value
OBJ=jpeg.o minisdl.o main.o
CLASRC=../../classes
CPL=$(SDL) $(LIB) $(INC) $(CONF)

all: $(OBJ)
	$(CC) $(SDL) $(CONF) $(OBJ) $(LIB) -o main

main.o: main.cc
	$(CC) $(CPL) -c main.cc -o main.o

minisdl.o: $(CLASRC)/minisdl.cc $(CLASRC)/minisdl.h
	$(CC) $(CPL) -c $(CLASRC)/minisdl.cc -o minisdl.o

jpeg.o: $(CLASRC)/jpeg.cc $(CLASRC)/jpeg.h
	$(CC) $(CPL) -c $(CLASRC)/jpeg.cc -o jpeg.o
