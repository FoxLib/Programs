all:
	g++ main.cc -o main
img:
	convert input3.jpg -auto-gamma -modulate 100,150 -resize 256x192^ -gravity Center -extent 256x192 output.ppm
test:
	./main output.ppm
