#include <GL/freeglut.h>
#include "SDL.h"

// -----------------------
#include "struct.h"
#include "fonts.h"
#include "graph.c"
#include "event.c"
// -----------------------

#include "functions.cpp"

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    // Инициализирует GL User Interface
    glutInit(&argc, argv);
    
    // Режим дисплея RGBA, есть буфер глубины и двойная буферизация
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowSize(1024, 768);
    glutCreateWindow("OpenGL Hello World");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Смешивание через вычитание Alpha-канала
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    glutIdleFunc(& MainIdleFunc);
    glutDisplayFunc(& MainDisplay);
    glutReshapeFunc(& ReshapeWindow);

    // Старт таймера 
    EventFPS(0);

    glutMainLoop();
    return 0;
}
