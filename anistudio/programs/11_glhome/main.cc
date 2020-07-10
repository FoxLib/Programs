#include "opengl.h"
#include "opengl.defaults.cc"

int main(int argc, char* argv[]) {

    opengl gl(argc, argv, 640, 480, "Hello, word!");

    // program = gl.load_shader("src/main.vs", "src/main.fs");
    // texture = gl.load_texture(&tex);

    glutMainLoop();
    return 0;
}
