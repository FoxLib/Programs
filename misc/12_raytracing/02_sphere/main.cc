#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ppm.h"
#include "raycast.h"

int main(int argc, char* argv[]) {

    ppm image(640, 480);
    raycast ray(640, 480);

    for (int i = -240; i < 240; i++)
    for (int j = -320; j < 320; j++) {

        vec3 dir = {(float)j / (float)320.0, (float)i / (float)320.0, 1.0};
        vec3 cast = ray.intersect_sphere({1, 0, 2}, 1, {0, 0, 0}, dir);

        if (cast.z > 0) {
            image.pset(320 + j, 240 - i, {255, 255, 255});
        }
    }

    // Перепроверка
    for (float beta = 0; beta <= 2.0*M_PI; beta += 0.05)
    for (float alpha = 0; alpha <= 2.0*M_PI; alpha += 0.05) {

        struct vec3 pn;

        pn.x = cos(alpha) * cos(beta)   + 1.0;
        pn.y = sin(beta)                + 0.0;
        pn.z = sin(alpha) * cos(beta)   + 2.0;

        struct vec2 pp = ray.proj2d(pn);
        image.pset(pp.x, pp.y, {255,0,0});
    }
    
    image.save("test.ppm");
    return 0;
} 