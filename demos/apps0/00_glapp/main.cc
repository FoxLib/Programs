int program, texture;

#include "opengl.h"
#include "opengl.events.cc"

int main(int argc, char* argv[]) {

    opengl gl(argc, argv, 640, 480, "Hello, word!");
    image  tex(new jpeg("src/texture.jpg"));

    gl.events();

    program = gl.load_shader("src/main.vs", "src/main.fs");
    texture = gl.load_texture(&tex);

    glUseProgram(program);
    glutMainLoop();

    return 0;
}
