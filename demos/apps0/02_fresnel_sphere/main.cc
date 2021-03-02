#include <main.h>

studio app(960, 540);

int main(int argc, char* argv[]) {

    int     i, j, k;
    image   img(2*960, 2*540);
    image   bg(new jpeg("src/bgoh.jpg"));

    context3d d3(& img);

    float factor        = 640.0;
    vec3f point_light   = {2.0, 2.0, 2.0};
    vec3f ambient_light = d3.ray->normal( {1.0, 1.0, -1.0} );

    for (k = 0; k < 3600; k++) {

        float fres = 2.0 * (float)k / 3600.0;

        // Универсальный шаблон для рисования
        for (i = d3.bottom; i < d3.top; i++) {
            for (j = d3.left; j < d3.right; j++) {

                uint color;
                float cz = 4.0 + sin(5.0*fres);

                // Вектор направления
                vec3f dir = d3.viewport(j, i);

                // Расчет дистанции до сферы
                TCollide r = d3.ray->cast_sphere(dir, { 0.0, 0.0, cz }, 1.0);

                int xi = (factor * dir.x) + (float)k/4.0;
                int yi = (factor * dir.y);

                float sp = 0.0;

                // Есть попадание на сферу
                if (r.cast) {

                    vec3f pl = d3.ray->fresnel(r, 0.0 + fres);

                    float am = d3.ray->dot( ambient_light, r.normal ); // Расчет силы света                                
                    vec3f reflect = d3.ray->reflect(r, point_light);   // Получение отраженного луча на сфере

                    sp = d3.ray->specular( reflect, r.direct, 20.0 );  // Вычисление отражения (specular)
                    sp = (sp + am) / 2.0;

                    xi = factor * pl.x + (float)k/4.0;
                    yi = factor * pl.y;
                } 

                xi = (int)(xi + d3.center.x);
                yi = (int)(yi + d3.center.y);

                // Точка
                color = bg.point(xi, yi, bg.width, bg.height);

                // Смешивание цветов
                vec3f vecolor = d3.ray->add( d3.ray->rgb(color), {sp, sp, sp} );

                d3.pset(j, i, d3.ray->rgb( vecolor ));
            }
        }

        // Строка для выдачи
        char str[256]; sprintf(str, "fresnel=%1.3f", fres);
        d3.canvas->font(17, 17, str, 0x0);
        d3.canvas->font(16, 16, str, 0xffffff);

        app.update(d3.canvas);
        app.savejpeg(d3.canvas, k);

    }
    app.loop();

    return 0;
}
