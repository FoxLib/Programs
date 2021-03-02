#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ppm.h"
#include "scene.h"

int main(int argc, char* argv[]) {

    ppm   image(640, 480);
    scene scene3d(640, 480);

    raycast ray;

    // Добавление сферы на сцену
    scene3d.add(new object({1, 0, 2}, 1.0));
    scene3d.add(new object({-0.5, 0, 2}, 1.0));
    scene3d.add(new object({0, 0, 1}, 0.25));

    vec3 center = {0, 0, 2};
    vec3 camera = {0, 0, 0};

    // Прямое освещение
    vec3 light  = ray.normalize({0, 1, -1});

    for (int i = -240; i < 240; i++)
    for (int j = -320; j < 320; j++) {

        vec3 dir = {(float)j / (float)320.0, (float)i / (float)320.0, 1.0};

        // Если пересечение с объектом есть, то тогда рисует точку
        if (scene3d.intersect(camera, dir)) {

            vec3 norm = scene3d.get_normal();

            // Модель освещенния диффузного
            float l = ray.dot(norm, light); if (l < 0.0) l = 0.0;

            image.pset(320 + j, 240 - i, {l*255, l*255, l*255});
        }
    }

    image.save("test.ppm");
    return 0;
} 
