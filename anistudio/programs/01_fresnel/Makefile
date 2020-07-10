LIBR=-ljpeg -lSDL -lm
WARN=-Wall -Wno-strict-overflow -Wno-unused-result
INCL=-I../../classes -I../..

all:
	g++ `sdl-config --cflags --libs` $(INCL) -Ofast main.cpp $(LIBR) $(WARN) -o main
	./main

