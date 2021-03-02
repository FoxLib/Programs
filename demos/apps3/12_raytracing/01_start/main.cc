#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ppm.h"

#define fov 240

struct vec3 {
    float x, y, z;
};

struct vec2 {
    float x, y;
};

// Проекция
struct vec2 projection(vec3 p) {

    vec2 t;
    t.x = p.x * fov / p.z;
    t.y = p.y * fov / p.z;
    return t;
}

int main(int argc, char* argv[]) {

    ppm image(640, 480);

    struct vec3 cam = {0, 0, 1.5};

    for (float beta = 0; beta <= 2.0*M_PI; beta += 0.05)
    for (float alpha = 0; alpha <= 2.0*M_PI; alpha += 0.05) {

        struct vec3 pn;

        pn.x = cos(alpha) * cos(beta)   + cam.x;
        pn.y = sin(beta)                + cam.y;
        pn.z = sin(alpha) * cos(beta)   + cam.z;

        struct vec2 pp = projection(pn);

        image.pset(320 + pp.x, 240 - pp.y, {255,255,255});
    }

    image.save("test.ppm");
    return 0;
} 