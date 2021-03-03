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

// ---------------------------------------------------------------------
// Объявление класса
// ---------------------------------------------------------------------

class GLUT {
protected:

    int width, height, main_window;

public:

    static int GLUT_milliseconds;
    static GLUT* instance;

    GLUT(int, char**, int);
    void window(const char*, int, int);
    void viewport(int w, int h);
    void setcamera(int ortho);

    char*  load_text(const char* filename);
    GLuint load_shader(const char* vertex, const char* fragment);
    GLuint load_texture(GLuint* bgra, GLuint width, GLuint height);
    GLint  uniform(GLuint, const char*, float);

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
        if (utime < GLUT_milliseconds) utime = GLUT_milliseconds - utime;
        else utime = 1;

        // Новый вызов таймера
        glutTimerFunc(utime, GLUT::func_timer, 0);
    }
};
