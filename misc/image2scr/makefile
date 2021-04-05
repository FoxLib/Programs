all:
	g++ main.cc -o main
	make img
	make test
img:
	#convert input.jpg -auto-gamma -resize 256x192^ -gravity Center -extent 256x192 output.ppm
	convert input.jpg -auto-gamma -resize 256x192^ -gravity Center -extent 256x192 output.ppm
test:
	./main output.ppm
