CC=g++
SDL=`sdl-config --cflags --libs`
LIBS=-lSDL -lm
WARN=-Wall
CFLAGS=-I../../include $(SDL) -O3 $(WARN)
OBJ=main.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o main
	./main

%.o: %.cc
	$(CC) $(CFLAGS) $(LIBS) $< -c -o $@

clean:
	rm -f *.o record-*.ppm
