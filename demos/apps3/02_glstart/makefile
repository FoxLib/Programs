# Этот файл отвечает за сборку проектов на GCC, G++ и т.д.
# apt install libjpeg-dev freeglut3-dev freeglut3 libgl1-mesa-dev mesa-common-dev libgl1-mesa-glx

# Переменная CC - ее используем, что подставлять вместо g++
CC=g++

# Флаги сборки (переменная)
LDFLAGS=-lglut -lGL -lGLU -ljpeg

# Цель "all", указывает, что чтобы собрать ее, нам требуется main.o
all: main.o jpegload.o 

# Выполним сборку (линковка)
	 $(CC) jpegload.o main.o $(LDFLAGS) -o main
	 
# Файлы от компиляции удалить
	 rm *.o
	 
# Запуск
	 ./main 

# Собираем main.o, используя файла main.c - если он меняется, файл пересобирается заново
main.o: main.c
	$(CC) -c main.c

jpegload.o: libs/jpegload.c
	$(CC) -c libs/jpegload.c

clean:
	rm *.o
