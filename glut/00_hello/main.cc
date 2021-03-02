#include "glut.cc"

GLUT* glut;
uint  texture_id, program_id;

void glmain(int event) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,  1.0, -2.41);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0,  1.0, -2.41);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0, -1.0, -2.41);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0, -2.41);
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glut = new GLUT(argc, argv, 40);
    glut->window("Schnuffel Bunny", 640, 640);

    texture_id = glut->texture_xor(256, 256);
    program_id = glut->load_shader("main.vs", "main.fs");
    glUseProgram(program_id);

    glutMainLoop();
    return 0;
}
