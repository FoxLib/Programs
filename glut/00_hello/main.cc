#include "glut.cc"

GLUT* glut;
uint  texture_id, program_id;
int   clocked;

void glmain(int event) {

    glut->cls();
    glut->uniform(program_id, "utime", (float)(clocked++) / 50);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.85,  1.0, -2.41);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.85,  1.0, -2.41);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.85, -1.0, -2.41);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.85, -1.0, -2.41);
    glEnd();
    glutSwapBuffers();

    // Запись серии ppm-скриншотов в файлы
    glut->record();
}

int main(int argc, char *argv[])
{
    // Инициализация
    glut = new GLUT(argc, argv, 33);
    glut->window("Schnuffel Bunny", 640, 360);

    // Загрузка ресурсов
    texture_id = glut->texture_xor(256, 256);
    program_id = glut->load_shader("main.fs");

    // Запуск программы
    glUseProgram(program_id);
    glutMainLoop();

    return 0;
}
