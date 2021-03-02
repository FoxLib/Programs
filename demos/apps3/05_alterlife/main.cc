
#include <GL/freeglut.h>
#include "SDL.h"

#include <stdio.h>
#include <vector>

/*
 * Файлы движка
 */

#include "common.h"

#include "class/TJpeg.cc"
#include "class/TObject.cc"
#include "class/TCamera.cc"
#include "class/TScene.cc"
#include "class/TEngine.cc"

int main(int argc, char* argv[]) {

    TEngine Main(&argc, argv);

    TScene* Scene = Main.CreateScene(); /* Главная сцена */
    (void)Scene->CreateCamera();        /* Центральная камера */

    Main.LoadTextureMap();  /* Загрузка текстур */

    glutMainLoop();
    return 0;
}
