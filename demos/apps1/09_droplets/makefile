CC=g++
SDL=`sdl-config --cflags --libs`
LIBS=-lSDL -lm
WARN=-Wall
CFLAGS=-I../../../includes $(SDL) -O3 $(WARN)
OBJ=main.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o main
	./main

%.o: %.cc
	$(CC) $(CFLAGS) $(LIBS) $< -c -o $@

rec:
	./main rec | ffmpeg -framerate 60 -r 60 -i - -vf "scale=w=1920:h=1080,pad=width=1920:height=1080:x=0:y=0:color=black" -sws_flags neighbor -sws_dither none -f mp4 -q:v 0 -vcodec mpeg4 -y record.mp4

clean:
	rm -f *.o main *.ppm
