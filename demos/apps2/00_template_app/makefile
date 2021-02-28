CC=g++
SDL=`sdl-config --cflags --libs`
LIB=-lSDL2 -lm
WARN=-Wall
CFLAGS=-I../../include $(SDL) -O3 $(WARN)

all:
	$(CC) $(CFLAGS) main.cc $(LIB) -o main
	./main

# Создание видео из серии PPM картинок размером 1920x1080x60 с центрированным дисплеем 320x200x5
rec:
	./main rec | ffmpeg -framerate 60 -r 60 -i - -vf "scale=w=1920:h=1080,pad=width=1920:height=1080:x=0:y=0:color=black" -sws_flags neighbor -sws_dither none -f mp4 -q:v 0 -vcodec mpeg4 -y record.mp4

rec320:
	./main rec | ffmpeg -framerate 60 -r 60 -i - -vf "scale=w=1600:h=1000,pad=width=1920:height=1080:x=160:y=40:color=black" -sws_flags neighbor -sws_dither none -f mp4 -q:v 0 -vcodec mpeg4 -y record.mp4

%.o: %.cc
	$(CC) $(CFLAGS) $(LIBS) $< -c -o $@
