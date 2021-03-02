#include "SDL.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

SDL_Surface*    _sdl_screen;
GLint           _glut_window, _glut_window_width, _glut_window_height;

enum EventList {
    EV_FPS     = 1             // Событие Frame Per Seconds
};

void    startup(const char* caption, int w, int h);
void    glutwin(const char* name, int w, int h, int argc, char* argv[]);
uint    fwogl_func_sdl_timer(uint interval, void *param);
int     getevent();
void    lambda_glut_resize(int width, int height);
void    glprojectmatrix();
void    glviewport3d(int w, int h);
char*   load_text(const char* filename);
GLuint  load_shader(const char* vertex, const char* fragment);
