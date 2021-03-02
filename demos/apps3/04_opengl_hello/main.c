#include <GL/freeglut.h>
#include "SDL.h"

#include <stdio.h>

// -----------------------
#include <vector>

// -----------------------
#include "struct.h"
#include "fonts.h"
#include "graph.c"
#include "event.c"
// -----------------------

#include "functions.h"
#include "object3d.cpp"
#include "scene3d.cpp"
#include "renderer.cpp"

object3d mtemp;

#include "functions.cpp"

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);    
    init();

    renderer Render; 
    scene3d  Scene;
    
    mtemp.loadFile("objects/cube.vb");    

    glutMainLoop();
    return 0;
}
