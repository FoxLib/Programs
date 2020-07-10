float timer;
int program, texture;

#include "opengl.h"

// Окно ничего не делает, обязательный параметр
void event_display_idle() { timer -= 0.001; event_display_repaint(); }

// Обязательный вызов отрисовщика
void event_display_repaint() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -----
    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, "timer"), timer);

    // ---
    glBegin(GL_QUADS); // Начинаем рисование
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,  1.0, 1 + 1.4142);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0,  1.0, 1 + 1.4142);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0, -1.0, 1 + 1.4142);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0, 1 + 1.4142);
    glEnd();
    // ---

    glutSwapBuffers();
}

int main(int argc, char* argv[]) {

    timer = 0;

    opengl gl(argc, argv, 512, 512, "Hello, word!");
    image  tex(new jpeg("src/earth_sm.jpg"));

    program = gl.load_shader("src/test.vs", "src/test.fs");
    texture = gl.load_texture(&tex);

    glutMainLoop();
    return 0;
}
