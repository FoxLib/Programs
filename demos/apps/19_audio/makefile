SDL=`sdl-config --cflags --libs`
all:
	g++ $(SDL) -I/home/fox/dev/programs/includes -Wno-unused-result -O3 main.cc -lSDL -o main
	arecord -v -f U8 -r 44100 -B 1 -t raw 2>info.txt | ./main > record.raw
wav:
	arecord -v -f U8 -r 44100 -t raw > raw.bin
