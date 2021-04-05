FF1=ffmpeg -framerate 30 -r 30 -i
FF2=-f mp4 -q:v 0 -vcodec mpeg4 -y
SCALE=-vf "scale=w=1280:h=960,pad=width=1920:height=1080:x=320:y=60:color=black" -sws_flags neighbor -sws_dither none

all:
	g++ -std=c++11 main.cc -o main
img:
	convert input.jpg -auto-gamma -modulate 100,150 -resize 256x192^ -gravity Center -extent 256x192 output.ppm
	./main floyd output.ppm a.ppm b.scr
collect:
	#$(FF1) dst/u_%05d.png $(SCALE) $(FF2) output.mp4
	$(FF1) dst/u_%05d.png $(FF2) output.mp4
