# ln -s .. lib

# g++ -c object3d.cpp -o object3d.o

# Компиляция и линковка в `main`
if (g++ `sdl-config --cflags --libs` -I../lib main.c -lSDL -lglut -lGL -lGLU -Wall -o main)
then

    echo "OK"
    ./main
fi
