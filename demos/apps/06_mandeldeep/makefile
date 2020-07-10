CC=g++
SDL=`sdl-config --cflags --libs`
LIBS=-lSDL -lm -lOpenCL
WARN=-Wall
CFLAGS=-I../../include $(SDL) -O3 $(WARN) -std=c++11
OBJ=main.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o main
	make clean
	./main

%.o: %.cc
	$(CC) $(CFLAGS) $(LIBS) $< -c -o $@

clean:
	rm *.o
