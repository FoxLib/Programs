#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ppm.h"
#include "raycast.h"

int main(int argc, char* argv[]) {

    ppm image(640, 480);
    raycast ray(640, 480);

    vec3 center = {0, 0, 2};
    vec3 camera = {0, 0, 0};
    vec3 light  = ray.normalize({0, 1, -1});

    for (int i = -240; i < 240; i++)
    for (int j = -320; j < 320; j++) {

        vec3 dir = {(float)j / (float)320.0, (float)i / (float)320.0, 1.0};
        vec3 cast = ray.intersect_sphere(center, 1, camera, dir);

        if (cast.z > 0) {

            // Ближайшая точка пересечения со сферой
            float nearest = cast.x < cast.y ? cast.x : cast.y;

            // Где находится эта точка
            vec3 point = {camera.x + nearest*dir.x, 
                          camera.y + nearest*dir.y,
                          camera.z + nearest*dir.z};

            // Находим перпендикуляр (он не нормализован)
            vec3 normal = ray.normalize({point.x - center.x, point.y - center.y, point.z - center.z});

            // Вычисляем интенсивность света
            float intl = ray.dot(normal, light); 
            if (intl < 0.0) intl = 0.0;

            image.pset(320 + j, 240 - i, {intl*255, intl*255, intl*255});
        }
    }

    image.save("test.ppm");
    return 0;
} 