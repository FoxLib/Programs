all:
	g++ main.cc -o main
	make img
img:
	convert input.jpg -auto-gamma -modulate 100,150 -resize 256x192^ -gravity Center -extent 256x192 output.ppm
	./main floyd output.ppm a.ppm b.scr
