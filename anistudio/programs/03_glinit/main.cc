#include "opengl.h"

// Окно ничего не делает, обязательный параметр
void event_display_idle() { }

// Обязательный вызов отрисовщика
void event_display_repaint() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ---
    glBegin(GL_QUADS); // Начинаем рисование
    glColor3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,  1.0, 2 + 1.4142);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0,  1.0, 2 + 1.4142);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0, -1.0, 2 + 1.4142);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0, 2 + 1.4142);
    glEnd();
    // ---

    glutSwapBuffers();
}

// Возвращается функция для левого и правого
vec2f event_audio(float time) {
    vec2f v; v.x = 0; v.y = 0; return v;
}

int main(int argc, char* argv[]) {

    audio  au(44100);    
    opengl gl(argc, argv, 768, 768, "Hello, word!");

    glutMainLoop();
    return 0;
}
