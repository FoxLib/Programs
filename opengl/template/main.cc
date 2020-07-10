#include <fwogl.cc>

int main(int argc, char* argv[]) {

    startup("Maincraft", 320, 200);                         // Инициализация SDL
    glutwin("GLUTWindow", 640, 400, argc, argv);            // Инициализация GLUT
    texture2gpu(GTexture("test.jpg"));                      // Загрузка текстуры
    glUseProgram(load_shader("shader.vs", "shader.fs"));    // Загрузка шейдера

    // Главный цикл
    while (getevent()) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,  1.0, 5.4142);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0,  1.0, 2.4142);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0, -1.0, 0.4142);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0, 2.4142);
        glEnd();
        glutSwapBuffers();
    }

    return 0;
}
