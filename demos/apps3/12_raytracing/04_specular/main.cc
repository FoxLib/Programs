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

    // Прямое освещение
    vec3 light  = ray.normalize({0, 1, -1});

    // Точечный свет
    vec3 pointlight = {1, -1, -1};

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

            // Вектор луча освещение к найденной точке пересечения
            vec3 pveclight = ray.normalize({pointlight.x - point.x, pointlight.y - point.y, pointlight.z - point.z});

            // Вектор луча к камере
            vec3 pcam   = ray.normalize({camera.x - point.x, camera.y - point.y, camera.z - point.z});

            // Находим перпендикуляр (он не нормализован)
            vec3 normal = ray.normalize({point.x - center.x, point.y - center.y, point.z - center.z});

            // Косинус угла между камерой и нормалью
            float cosnc = ray.dot(normal, pcam);

            // = 2*N*<N,L> - L
            vec3 reflect = {2.0*normal.x*cosnc - pcam.x, 
                            2.0*normal.y*cosnc - pcam.y, 
                            2.0*normal.z*cosnc - pcam.z};

            // Интенсивность освещения
            float ints = ray.dot(pveclight, reflect);
            if (ints < 0.0) ints = 0.0;
            ints = pow(ints, 30.0);

            // Вычисляем интенсивность света
            float intl = ray.dot(normal, light); 
            if (intl < 0.0) intl = 0.0;
         
            // Смешиваем - аддитивно
            intl = intl * 0.5 + ints * 0.5;
            if (intl > 1.0) intl = 1.0;

            image.pset(320 + j, 240 - i, {intl*255, intl*255, intl*255});
        }
    }

    image.save("test.ppm");
    return 0;
} 