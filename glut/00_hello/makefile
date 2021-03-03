all:
	g++ main.cc -lm -lGL -lGLU -lGLEW -lglut -o main
	./main

# Конвертирование файла ppm -> mp4
rec:
	./main rec | ffmpeg -framerate 30 -r 30 -i - -vf "scale=w=1920:h=1080,pad=width=1920:height=1080:x=0:y=0:color=black" -sws_flags neighbor -sws_dither none -f mp4 -q:v 0 -vcodec mpeg4 -y record.mp4
