all:
	g++ -c `sdl-config --cflags --libs` main.cc -Wall -o main.o
	g++ main.o -ljpeg -lSDL -lglut -lGL -lGLU  -o main
	./main

