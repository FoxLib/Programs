#include <GL/glew.h>
#include <GL/glut.h>

#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

// Данная функция должна быть реализована!
void glmain(int);

// Типы событий
enum GLUT_events {
    L_event_timer = 1
};

// Вершинный шейдер по умолчанию
static char glut_vertex_shader[] = "#version 120\n\
attribute vec4 position;\n\
void main() {\
    gl_TexCoord[0] = gl_MultiTexCoord0;\
    gl_Position    = gl_ProjectionMatrix * gl_ModelViewMatrix * position;\
}";

// ---------------------------------------------------------------------
// Объявление класса
// ---------------------------------------------------------------------

class GLUT {
protected:

public:

    long    start_time;
    int     width, height, main_window;
    GLuint* fb;

    static int GLUT_milliseconds;
    static GLUT* instance;

    GLUT(int, char**, int);
    ~GLUT();

    void readfb();
    void drawfb();
    void window(const char*, int, int);
    void viewport(int w, int h);
    void setcamera(int ortho);

    void record();
    void recordout();
    void record(const char* file);
    void record(FILE*);

    char*  load_text(const char* filename);
    GLuint load_shader(const char* vertex, const char* fragment);
    GLuint load_shader(const char* fragment);
    GLuint load_texture(GLuint* bgra, GLuint width, GLuint height);
    GLint  uniform(GLuint, const char*, float);
    long   microtime();
    long   clocks();

    GLuint texture_xor(int, int);
    void   cls() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    // -----------------------------------------------------------------

    // Событийная система событий для события событий
    static void func_reshape(int w, int h) { instance->viewport(w, h); }
    static void func_display() { }

    // Используется только для SLEEP
    static void func_idle() { usleep(1); }

    // Таймер надо всегда вызывать снова
    static void func_timer(int timer_id) {

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        glmain(L_event_timer);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> time_span = t2 - t1;

        // Расчет следующего кадра
        int utime = time_span.count();

        if (utime < GLUT_milliseconds)
             utime = GLUT_milliseconds - utime;
        else utime = 1;

        // Новый вызов таймера
        glutTimerFunc(utime, GLUT::func_timer, 0);
    }
};
