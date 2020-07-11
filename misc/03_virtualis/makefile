# Этот файл отвечает за сборку проектов на GCC, G++ и т.д.
# apt install libjpeg-dev freeglut3-dev freeglut3 libgl1-mesa-dev mesa-common-dev libgl1-mesa-glx libglew2.0 libglew-dev

# Переменная CC - ее используем, что подставлять вместо g++
CC=g++

# Флаги сборки (переменная)
LDFLAGS=-lglut -lGL -lGLU -lGLEW -ljpeg

# Цель "all", указывает, что чтобы собрать ее, нам требуется main.o
all: 
	gcc -c main.c 
	$(CC) include/jpeg/load.o main.o $(LDFLAGS) -o main
	rm *.o
	./main 

