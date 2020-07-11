# ln -s .. lib
# Компиляция и линковка в `main`
if (g++ `sdl-config --cflags --libs` -I../lib main.c -lSDL -lglut -lGL -lGLU -Wall -o main)
then

    echo "OK"
    ./main
fi
