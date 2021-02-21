#include <qblib.c>
#include <engi3.cc>

int main(int argc, char* argv[]) {

    screen(13);
    load_running_string("ticker.txt");

    math3 m3;

    // Расчет параметров плоскости
    vec3 origin = {0, 0, 0};
    vec3 light = {1, 2, 3};

    m3image img("src/parket.ppm");
    m3image img2("src/parket2.ppm");

    m3.add_plane(M3_OBJECT_PLANE, {-2, 0, 4}, { 2, 0, 4}, {-2, 0, 0} );
    m3.add_plane(M3_OBJECT_SQUARE, { 3, 2, 4}, { 3, 2, 0}, { 3, 0, 4} );

    // Левая стена
    m3.add_plane(M3_OBJECT_SQUARE, {-4, 2, 4}, {-4, 2, 0}, {-3, 0, 4} );

    m3.add_sphere({0, 0, 2}, 1);
    m3.add_sphere({2, 2, 2}, 0.5);
    m3.add_sphere({0, 1, 1}, 0.25);

    float t = 0;
    while (sdlevent(EVT_REDRAW)) {

        useindex(0);

        // Положение камеры
        origin.x = 3*sin(t);
        origin.y = 0.6 + 0.5*sin(0.5*t);
        origin.z = 2 - 3*cos(t);

        // Положение точки света
        light.x = 1.5*sin(2*t);
        light.z = 2 + 3*cos(3*t);

        // Обновление экрана
        for (int y = -100; y < 100; y++)
        for (int x = -160; x < 160; x++) {

            RGB cl = {0, 0, 0};

            // Бросить луч от камеры в сторону пикселя на экране
            castpoint m = m3.raycast(origin, m3.roty( m3.cam(x, y), t ));

            if (m.cast) {

                // Бросание луча по направлению к свету
                vec3  lp = m3.sub(light, m.coord);
                float ln = m3.len(lp);

                if (ln > 0) {

                    // Вычисление тени
                    castpoint s = m3.raycast(m.coord, m3.norm(lp));

                    // Расстояние до освещения
                    float l  = 1 / sqrt(ln);
                    if (l > 1) l = 1;

                    // Луч находится в тени
                    if (s.cast) l *= 0.5;

                    // Для сферы одна текстура, для плоскости другая
                    if (m.type == M3_OBJECT_SPHERE)
                         cl = img.point(m.uv.x, m.uv.y);
                    else cl = img2.point(m.uv.x, m.uv.y);

                    cl.r *= l;
                    cl.g *= l;
                    cl.b *= l;
                }
            }

            pset(160 + x, 99 - y, rgb(cl.r, cl.g, cl.b));
        }

        t += 0.0025;

        // Запись экрана
        useindex(1); color(15, 0); 
        // if (step_runstr(24)) 
        record(argc, argv, 0);
    }

    return 0;
}
