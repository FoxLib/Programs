
# Компиляция главного файла
if (g++ -c `sdl-config --cflags --libs` main.cc -Wall -o main.o)
then

    # Сборка
    if (g++ main.o -ljpeg -lSDL -lglut -lGL -lGLU  -o main)
    then
        echo "OK"
        ./main
    fi
fi

