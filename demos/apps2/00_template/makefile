CC=g++
SDL=`sdl-config --cflags --libs`
LIB=-lSDL2 -lm
WARN=-Wall
CFLAGS=-I../../include $(SDL) -O3 $(WARN)
OBJ=main.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o main
	./main

%.o: %.cc
	$(CC) $(CFLAGS) $(LIBS) $< -c -o $@
