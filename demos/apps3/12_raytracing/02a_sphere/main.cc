#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ppm.h"
#include "raycast.h"

int main(int argc, char* argv[]) {

    ppm image(640, 480);
    raycast ray(640, 480);

    vec3 dirlt  = ray.norm({1, 1, -1});
    vec3 cam    = {0, 0, 0};
    vec3 center = {0, 0, 2};

    for (int i = -240; i < 240; i++)
    for (int j = -320; j < 320; j++) {

        vec3 dir  = {(float)j / (float)320.0, (float)i / (float)320.0, 1.0};
        vec3 cast = ray.intersect_sphere(center, 1, cam, dir);

        if (cast.z > 0) {

            vec3 point  = ray.add(cam, ray.mul(dir, (cast.x < cast.y ? cast.x : cast.y)));
            vec3 normal = ray.norm(ray.sub(point, center));

            float cosa = ray.dot(dirlt, normal);
            if (cosa < 0) cosa = 0;

            image.pset(320 + j, 240 - i, {255*cosa, 255*cosa, 255*cosa});
        }
    }

    image.save("test.ppm");
    return 0;
} 