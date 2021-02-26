CC=g++
SDL=`sdl-config --cflags --libs`
LIB=-lSDL2 -lm
WARN=-Wall
CFLAGS=-I../../include $(SDL) -O3 $(WARN)

all:
	$(CC) $(CFLAGS) main.cc $(LIB) -o main
	./main

%.o: %.cc
	$(CC) $(CFLAGS) $(LIBS) $< -c -o $@
