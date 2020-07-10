#include <SDL/SDL.h>
#include <math.h>

// Объявления
// ----------------------------
#include "main.h"
#include "font/font8x8_basic.h"
#include "font/font8x16.h"

// Библиотеки
// ----------------------------
#include "jpeg.cpp"
#include "ppm.cpp"
#include "image.cpp"
#include "audio.cpp"
#include "context3d.cpp"
#include "raytrace.cpp"
#include "studio.cpp"

// Компоненты
// ----------------------------
#include "com/mandel.cpp"
#include "com/figures.cpp"

// Команда для сборки видео
// ffmpeg -r 60 -i build/%06d.jpeg -c:v libx264 -s 1920x1080 -pix_fmt yuv420p test.mp4

// ----------------------------
studio app(960, 540);
// ----------------------------

int main(int argc, char* argv[]) {

    image img(960, 540);

    context3d d3(& img);
    raytrace  ray;

    int  i, j, k;
    uint color;
        
    vec3f light = {0.0, 2.0, -2.0};
    vec3f pointlight = {0.0, 2.0, 2.0};
    vec3f center = { 0.0, 0.0, 4.0 };

    for (k = 0; k < 3600; k++) {

        float phase = (float)k * 0.01;

        center.x = sin(phase) * 1.0;
        center.z = cos(phase) * 1.0 + 5.0;

        for (i = -269; i < 270; i++)
        for (j = -479; j < 480; j++) {

            vec3f dir = d3.viewport(j, i);

            // Расчет дистанции до сферы
            float dist = ray.closest_sphere(dir, center, 1.00);

            if (dist > 1.0) {

                vec3f coord   = ray.dot(dir, dist);         // Точка на сфере
                vec3f Ncoord  = ray.normal( light );        // Нормаль к общему освещению
                vec3f Nsphere = ray.normal( ray.sub(coord, center) ); // Получение нормали к сфере

                // Расчет отражения по лучу и нормали сферы
                vec3f reflect = ray.reflect( Nsphere, ray.sub(coord, pointlight) );
                
                // Сила освещения
                float am = ray.dot(Ncoord, Nsphere);
                float sp = ray.specular( reflect, dir, 20.0 );

                if (am > 0.0) {

                    vec3f clr = ray.add( ray.vec3(am, 0, 0), ray.vec3(0, sp, sp) );                
                    color = ray.rgb( clr );
                }
                else {
                    color = 0;
                }
                
            } else {
                color = 0x80;
            }

            d3.canvas->pset(j + 480, 270 - i, color);
        }

        app.update(d3.canvas);
        app.events();

        app.savejpeg(d3.canvas, k);
    }
    
    app.loop();
    
    return 0;
}
