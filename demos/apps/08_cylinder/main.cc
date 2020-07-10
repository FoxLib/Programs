#include <qblib.c>

// Статичное небо
void starfeld() {

    cls();
    srand(1);
    for (int i = 0; i < 255; i++) pset(rand()%320, rand()%200, rand()%256);
    circlef(240, 100, 16, 240);
}

int main(int argc, char* argv[]) {

    screen(13);

    float x0 = 0, y0 = 0, r = 1;
    float al = 0, rx = 0, ry = 0, rz = 0;

    int cl;
    for (int i = 0; i < 256; i++) palette(i, i, i, i);

    while (sdlevent(EVT_REDRAW)) {

        starfeld();

        // Движение вперед, шаги
        al += 0.01;
        y0 = -0.92 + abs(sin(10*al) * 0.02);
        rz = sin(10*al) * 0.01;

        float tx, ty, tz;
        for (int y = -99; y <= 100; y++)
        for (int x = -160; x < 160; x++) {

            float   dx = (float)x / 200,
                    dy = (float)y / 200,
                    dz = 1;

            // Поворот Y
            tx = dx*cos(ry) - dz*sin(ry);
            tz = dz*cos(ry) + dx*sin(ry);
            dx = tx; dz = tz;

            // Поворот X
            tz = dz*cos(rx) - dy*sin(rx);
            ty = dy*cos(rx) + dz*sin(rx);
            dz = tz; dy = ty;

            // Поворот Z
            tx = dx*cos(rz) - dy*sin(rz);
            ty = dy*cos(rz) + dx*sin(rz);
            dx = tx; dy = ty;

            // Точка пересечения с цилиндром
            float a =  dx*dx + dy*dy;
            float b = (dx*x0 + dy*y0)*2;
            float c = x0*x0 + y0*y0 - r*r;
            float d = b*b - 4*a*c;

            if (d >= 0) {

                d = sqrt(d);

                // Корни уравнения
                float t1 = (-b + d) / (2*a);
                float t2 = (-b - d) / (2*a);
                float t;

                // Зависит от того, находимся ли внутри цилиндра
                if (c > 0)
                     t  = (t1 < t2) ? t1 : t2;
                else t  = (t2 < t1) ? t1 : t2;

                // Ближайший перед камерой
                if (t1 < 0 && t2 > 0)
                    t = t2;

                // Нет точки пересечения, пропуск
                if (t1 < 0 && t2 < 0) {
                    cl = 0;
                }
                else {

                    // Расчет координат пересечения
                    float _x = x0 + dx*t,
                          _y = y0 + dy*t,
                          _z =      dz*t;
                    float _a = atan2(_y, _x);

                    // Коррекция текстуры
                    if (_a < 0) _a = -_a;
                    if (_z < 0) _z = -_z;

                    // Расчет текстуры
                    float tex =  (((int)(_a*r*256) ^ (int)(r*128*(_z + al)))%256);

                    // Непрозрачность или прозрачные области
                    if (tex < 64 && (_x < -0.2 || _x > 0.2)) continue;

                    // Уровень освещенности
                    float lt = 2/_z;
                          lt = lt > 1 ? 1 : lt;

                    cl = tex*lt;
                    if (cl > 255) cl = 255;
                }


            } else {
                cl = 2;
            }

            pset(x + 160, 100 - y, cl);
        }
    }

    return 0;
}
