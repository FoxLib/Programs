# ln -s ../libsdl
# Компиляция и линковка в `main`
if (gcc `sdl-config --cflags --libs` -I../lib main.c -lSDL -Wall -o main)
then

    echo "OK"
    ./main
fi
