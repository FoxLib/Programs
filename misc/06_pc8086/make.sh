if (gcc `sdl-config --cflags --libs` main.c -Isdl -lSDL -Wall -o main)
then

    echo "OK"
    ./main

fi
