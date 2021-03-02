#include <GL/glew.h>
#include <GL/glut.h>
#include <unistd.h>
#include <stdio.h>

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

    GLuint texture_xor(int, int);

    // Событийная система событий для события событий
    static void func_reshape(int w, int h) { instance->viewport(w, h); }
    static void func_display() { }

    // Используется только для SLEEP
    static void func_idle() { usleep(1); }

    // Таймер надо всегда вызывать снова
    static void func_timer(int timer_id) {

        glmain(L_event_timer);
        glutTimerFunc(GLUT_milliseconds, GLUT::func_timer, 0);
    }
};
