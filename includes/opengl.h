
// Стандартная библиотека
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// SDL
#include <SDL/SDL.h>

// OpenGL
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Объявляем структуры и заголовки
#include "vector.h"
#include "font8x16.h"
#include "ppm.h"
#include "image.h"

// Укоротить типы
#define uchar   unsigned char

// ---------------------------------------------------------------------
struct face3 {

    uint    v[3];       // Номера вершин (1..n)
    uint    vt[3];      // Текстурные координаты (1..n)
    uint    vn[3];      // Нормали (1..n)
};

struct mtl {

    char    name[256];  // Имя материала
    uint    texture_id; // Номер текстуры
    uint    color;

};
// ---------------------------------------------------------------------

#include "object3d.h"
#include "opengl.class.h"

// Подключаем сами файлы
#include "image.cc"
#include "jpeg.cc"
#include "ppm.cc"
#include "opengl.cc"
#include "object3d.cc"

