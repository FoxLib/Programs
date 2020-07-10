# Настройки библиотек
LIBR="-ljpeg -lSDL -lm -lglut -lGL -lGLU -lGLEW"
WARN="-Wall -Wno-strict-overflow -Wno-unused-result"
INCL="-I../../classes"

# Компилируется через G++
if (g++ `sdl-config --cflags --libs` $INCL -Ofast main.cc $LIBR $WARN -o main) then
    ./main
fi
