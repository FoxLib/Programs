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
#include "raytrace.cpp"
#include "context3d.cpp"
#include "studio.cpp"

// Компоненты
// ----------------------------
#include "com/mandel.cpp"
#include "com/figures.cpp"

// Команда для сборки видео

// ffmpeg -r 60 -i build/%06d.jpeg -c:v libx264 -s 1920x1080 -pix_fmt yuv420p test.mp4
// ffmpeg -r 60 -i build/%06d.jpeg -i audio.mp3 -c:v libx264 -c:a libvorbis -shortest -s 1920x1080 -pix_fmt yuv420p output.mkv

// ----------------------------
studio app(960, 540);
// ----------------------------

int main(int argc, char* argv[]) {

    int     i, j, k;
    image   img(2*960, 2*540);

    context3d   d3(& img);

    image   earth( new jpeg("rsrc/earth.jpg") );
    image   milky( new jpeg("rsrc/milky.jpg") );

    // Общее освещение подчиняется точке (0,0,0)
    vec3f point_light   = {1.0, 1.0, 3.0};
    vec3f ambient_light = d3.ray->normal( {1.0, 1.0, -1.0} );

    // 10 минут
    for (k = 0; k < 36000; k++) {

        // Универсальный шаблон для рисования
        for (i = d3.bottom; i < d3.top; i++) {
            for (j = d3.left; j < d3.right; j++) {

                uint  color;
                float edist = 6.0 - (k/36000.0) * 4.0;

                // Вектор направления
                vec3f dir = d3.viewport(j, i);

                // Расчет дистанции до сферы
                TCollide r = d3.ray->cast_sphere(dir, { 0.0, 0.0, edist }, 1.00);

                // Есть попадание на сферу
                if (r.cast) {

                    vec3f reflect = d3.ray->reflect(r, point_light);        // Получение отраженного луча на сфере
                    float sp = d3.ray->specular( reflect, r.direct, 20.0 ); // Вычисление отражения (specular)
                    float am = d3.ray->dot( ambient_light, r.normal );      // Расчет силы света                

                    // Расчет координат на сфере
                    vec2f uv = d3.ray->uv_sphere(r.normal);

                    // Точка на Земле
                    int u = ((int)(k  + (uv.x * earth.width / 2.0)) % earth.width); 
                    int v = ((int)(    (uv.y * earth.height)) % earth.height); 

                    // Цвет земли, умноженный на ambient light
                    vec3f cl1 = d3.ray->rgb( earth.point(u, v) );
                    vec3f cl2 = d3.ray->dot( cl1, 2.0*am );

                    // Добавление specular
                    sp *= 0.75;
                
                    color = d3.ray->rgb( d3.ray->add(cl2, d3.ray->vec3(sp, sp, sp)) ); // Конечный цвет
                    
                } else {

                    color = milky.point(1.0*(float)j + img.width*0.5, 1.0*(float)i + img.height*0.5);
                }

                // Рисование точки
                d3.pset(j, i, color);
            }
        }

        app.savejpeg(d3.canvas, k);

        char db[256]; sprintf(db, "image: %d", k); d3.canvas->font(FONT8x16, 8, 8, db, 0xffffff);
        app.update(d3.canvas);
        
    }
    
    app.loop();

    return 0;
}
