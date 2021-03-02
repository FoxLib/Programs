// require: data/landscape/
// require: include/jpeg

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <time.h>   

// Библиотека для работы с JPEG
#include "include/jpeg/load.h"

// Основное окно
int window;

// Заголовочные файлы
#include "engine/algebra.h"
#include "engine/shaders.h"
#include "engine/vbo.h"
#include "engine/landscape.h"
#include "engine/skybox.h"
#include "engine/water.h"
#include "engine/camera.h"
#include "engine/controls.h"
 
// Функции отображения
#include "engine/camera.c"
#include "engine/controls.c"
#include "engine/textures.c"
#include "engine/skybox.c"
#include "engine/water.c"
#include "engine/landscape.c"
#include "engine/shaders.c"
#include "engine/vbo.c"
#include "engine/display.c"
#include "engine/init.c"

// ---------------------------------------------------------------------
// Запускной файл
// ---------------------------------------------------------------------

int main(int argc, char* argv[]) {
    
    glutInit(& argc, argv);

    init();    
    
    // Тестовая текстура
    T_grass_id = jpeg_texture_load( loadjpeg("data/basictex/grass.jpg") );    

    // Стандартный шейдер
    shader_load( SHADER_STANDART, "data/shaders/default.vs", "data/shaders/default.fs");
    
    // Шейдер ландшафта
    shader_load( SHADER_LANDSCAPE, "data/shaders/land.vs", "data/shaders/land.fs");

    // Шейдер воды
    shader_load( SHADER_WATER,    "data/shaders/water.vs", "data/shaders/water.fs");
    shader_load( SHADER_SUBWATER, "data/shaders/subwater.vs", "data/shaders/subwater.fs");
    
    // Генерация ландшафта
    land_load();
    land_generator();  

    // Генерировать skybox 
    skybox_generator(); 
    
    // Генерация воды
    water_generator();
    
    glutMainLoop();
    return 0;    
}
