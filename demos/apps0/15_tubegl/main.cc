#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "main.oglite.h"

uint   earth_tex, metal_tex, stars_tex,
       prg_earth, prg_lift, prg_stars;
       
float  timer;
vec3f  cam;
uint*  canvas;
float  start;
int    running;
float  velocity;
float  alt;

int    click_pressed;
vec2f  click_at;

oglite app(800, 800, "OpenGL Demo");

// Изменение размера окна
void glut_reshape(int w, int h) { app.viewport(w, h); }

// Рисование квадрата
void draw_quad(float depth) {

    glBegin(GL_QUADS);    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,  1.0, depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0,  1.0, depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0, -1.0, depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0, depth);       
    glEnd();
}

// Записать шрифт в видеопамять
void write_font(int x, int y, const char* s, uint color) {

    int i, j, height = 16;

    while (*s) {

        unsigned char ch = *s; s++;

        for (i = 0; i < height; i++) {
            for (j = 0; j < 8; j++) {

                if (font8x16[ch*16 + i] & (1<<(7-j))) {
                    canvas[ app.width * (app.width - i - y) + j + x ] = color;
                }
            }
        }

        x += 8;
    }
}

// Перерисовка окна
void glut_repaint() {

    // Определить фоновый цвет
    glClearColor(0.0f, 0.5f, 0.8f, 0.0f);   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 6,3m
    if (running) {
        
        timer = (float)(clock() - start) / (float)CLOCKS_PER_SEC * 3.0;

        float speed = 9.8;
        velocity = timer * speed;
        alt = speed * timer * timer / 2.0;
    }
    else {    
        alt = 0;
    }
    
    //cam.x = 1.5 - 0.01001*alt;
    //cam.y = 0; // 0.01001*alt;
    //cam.z = -0.01001*alt;

    if (cam.x < 0) cam.x = 0.0;
    if (cam.x > 3.14) cam.x = 3.14;

    // -------------------------------------
    glUseProgram(prg_stars);
    glBindTexture(GL_TEXTURE_2D, stars_tex);
    glUniform1f(glGetUniformLocation(prg_stars, "time"), alt);
    glUniform3f(glGetUniformLocation(prg_stars, "cam"), cam.x, cam.y, cam.z);
    draw_quad(3.0);

    // -------------------------------------
    glUseProgram(prg_earth);
    glBindTexture(GL_TEXTURE_2D, earth_tex);
    glUniform1f(glGetUniformLocation(prg_earth, "time"), alt);
    glUniform3f(glGetUniformLocation(prg_earth, "cam"), cam.x, cam.y, cam.z);
    draw_quad(2.0);

    // -------------------------------------
    glUseProgram(prg_lift);
    glBindTexture(GL_TEXTURE_2D, metal_tex);
    glUniform1f(glGetUniformLocation(prg_lift, "time"), alt);
    glUniform3f(glGetUniformLocation(prg_lift, "cam"),  cam.x, cam.y, cam.z);
    draw_quad(1.0);

    // Прочесть пиксели
    // --------------------------

    char sp[256]; 
    
    glRasterPos2d(-1,-1);   
    glReadPixels(0, 0, app.width, app.height, GL_BGRA, GL_UNSIGNED_BYTE, canvas);
    
    sprintf(sp, "time: %d s.", (int) timer); write_font(16, 16, sp, 0xffffffff);
    sprintf(sp, "altitude: %.3f km", alt / 1000); write_font(16, 32, sp, 0xffffffff);
    sprintf(sp, "velocity: %.1f m/s", velocity); write_font(16, 48, sp, 0xffffffff);
    
    glDrawPixels(app.width, app.height, GL_BGRA, GL_UNSIGNED_BYTE, canvas);
    
    glutSwapBuffers();
}

void glut_kb(unsigned char k, int x, int y) {

    // Цифра 1
    if (k == 49) {

        start = clock();
        running = 1;
    }

}

void glut_click(int button, int state, int x, int y) {

    click_pressed = (state == 0 ? 1 : 0);
    click_at      = {(float)x, (float)y};
}

void glut_move(int x, int y) {

    const float factor = 1.0 / 200.0;

    if (click_pressed) {

        cam.x -= factor * (GLfloat)(y - click_at.y);  // Вращение вокруг оси X (вверх-вниз)
        cam.z += factor * (GLfloat)(x - click_at.x);  // Вращение вокруг оси Y (влево-вправо)

        click_at = {(float)x, (float)y};
    }
}

// Основная программа
int main(int argc, char* argv[]) {

    timer = 0;
    cam = {1.5, 0, 0};
    canvas = (uint*)malloc(4*2048*2048);

    alt = 0;
    running = 0;
    velocity = 0;
    click_pressed = 0;

    // ------------------
    app.init(argc, argv);

    glutReshapeFunc (glut_reshape);
    glutDisplayFunc (glut_repaint);
    glutIdleFunc    (glut_repaint);
    glutKeyboardFunc(glut_kb);
    glutMouseFunc   (glut_click);
    glutMotionFunc  (glut_move); 

    // ------------------

    image earth(new jpeg("src/earth.jpg"));    
    image metal(new jpeg("src/metal.jpg"));    
    image stars(new jpeg("src/star.jpg"));    

    earth_tex = app.load_texture(earth.canvas, earth.width, earth.height);
    metal_tex = app.load_texture(metal.canvas, metal.width, metal.height);
    stars_tex = app.load_texture(stars.canvas, stars.width, stars.height);
    
    prg_lift  = app.load_shader("src/main.vs", "src/main.fs");
    prg_earth = app.load_shader("src/main.vs", "src/earth.fs");
    prg_stars = app.load_shader("src/main.vs", "src/stars.fs");
    
    glutMainLoop();
    return 0;
}
