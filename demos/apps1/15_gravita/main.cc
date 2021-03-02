#include <qblib.c>

// Расчет силы гравитации
// src - точка, которая притягивается
// dst - точку, к которой идет притяжение
// w   - вес dst
vec3 velocity(vec3 src, vec3 dst, float w) {

    const float G = 6.6743E-11;

    vec3  d = {dst.x - src.x, dst.y - src.y, dst.z - src.z};
    float r = d.x*d.x + d.y*d.y + d.z*d.z;
    float F = G*w/r;
    float n = F/sqrt(r);

    return {n*d.x, n*d.y, n*d.z};
}

int main(int argc, char* argv[]) {

    screen(13);

    float scale = 10;

    // Планета Земля
    circlef(160, 100, scale, 1);
    circle(160,  100, scale, 32);

    const float earth_mass   = 5.972E+24;
    const float earth_radius = 6371000;

    // Начальные позиции
    vec3 p = {-2*earth_radius, 0, 0};
    vec3 v = {4000, 6000, 0};

    int stop  = 0;
    int timer = 0;

    while (sdlevent(EVT_REDRAW)) {

        if (stop) continue;

        // Время в минутах
        char tmp[200]; locate(1, 1); sprintf(tmp, "%d m.", timer/60); print(tmp);

        // x N вычисление скорости
        for (int i = 0; i < 16; i++) {

            timer++;

            vec3 a = velocity(p, {0, 0, 0}, earth_mass);

            // Добавить скорости к точке
            v.x += a.x;
            v.y += a.y;
            v.z += a.z;

            // Добавить скорость к позиции
            p.x += v.x;
            p.y += v.y;
            p.z += v.z;
        }

        pset(160 + scale*p.x/earth_radius, 100 - scale*p.y/earth_radius, 2);

        if (p.x*p.x + p.y*p.y + p.z*p.z < earth_radius*earth_radius) stop = 1;
    }

    return 0;
}
