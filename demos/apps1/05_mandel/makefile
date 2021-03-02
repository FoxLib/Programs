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

video:
	ffmpeg -framerate 60 -r 60 -i record-%06d.ppm -vf "scale=w=1600:h=1000,pad=width=1920:height=1080:x=160:y=40:color=black" -sws_flags neighbor -sws_dither none -f mp4 -q:v 0 -vcodec mpeg4 -y record.mp4

clean:
	rm -f *.o record-*.ppm *.mp4
