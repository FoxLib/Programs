LIBR=-ljpeg -lSDL -lm -lglut -lGL -lGLU -lGLEW
WARN=-Wall -Wno-strict-overflow -Wno-unused-result
INCL=-I../../classes
all:
	g++ `sdl-config --cflags --libs` $(INCL) -Ofast main.cc $(LIBR) $(WARN) -o main
	./main
