SDL=`sdl-config --cflags --libs`

all:
	g++ main.cc $(SDL) -lSDL -O3 -Wno-unused-result -o main
	./main
