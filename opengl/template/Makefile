CC=g++
INCLUDE=-I../include/
LIBRARY=-ljpeg -lSDL -lm -lglut -lGL -lGLU -lGLEW
WARNING=-Wall -Wno-strict-overflow -Wno-unused-result
SDLCONF=`sdl-config --cflags --libs`
OPTIONS=-O3
OBJECTS=main.o

all: $(OBJECTS)
	$(CC) $(SDLCONF) $(INCLUDE) $(OBJECTS) $(LIBRARY) -o main
	./main

%.o: %.cc
	$(CC) $(SDLCONF) $(INCLUDE) $(OPTIONS) $(WARNING) $< -c -o $@

clean:
	rm *.o
