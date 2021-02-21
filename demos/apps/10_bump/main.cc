#include <qblib.c>

vec2 normal[320][200];
int  height[320][200];

void calcnormal(int x, int y) {

    normal[x][y].x = height[x][y] - height[(x+1)%320][y];
    normal[x][y].y = height[x][y] - height[x][(y+1)%200];
}

void init() {

    color(32, 0); cls();
    locate(12, 12); print("Эффект BumpMapping");
    circle(100, 50, 20, 16);
    circlef(220, 50, 20, 32);
    circlef(220, 50, 15, 8);
    lineb(140, 30, 180, 60, 32);

    linebf(50, 120, 250, 180, 32);
    color(16, -1);
    locate(8, 18); print("Расчет силы света");
    locate(8, 20); print("L=1-k*|2N-(L-P)|");
}

int main(int argc, char* argv[]) {

    screen(13);
    init();
    load_running_string("ticker.txt");

    for (int i = 1; i < 256; i++) palette(i, i, i, i);
    palette(255, 0, 255, 0);

    // Расчет карты нормалей
    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++)
        height[x][y] = qb_screen[x][y];

    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++)
        calcnormal(x, y);

    color(255, 0);
    cls();

    float t = 0;
    while (sdlevent(EVT_REDRAW)) {

        vec2 light = {160 + sin(t)*160, 100 + cos(3*t)*100};
        t += 0.01;

        srand(1);
        for (int y = 0; y < 192; y++)
        for (int x = 0; x < 320; x++) {

            vec2 h = {
                2*normal[x][y].x - (light.x - x),
                2*normal[x][y].y - (light.y - y),
            };

            // Дистанция наклона луча
            float l = (256 - sqrt(h.x*h.x + h.y*h.y));

            // Имитация текстуры
            l += rand()%7;

            // Границы
            if (l < 1) l = 1;
            else if (l > 254) l = 254;

            pset(x, y, l);
        }

        useindex(1);
        color(250, 0);
        if (step_runstr(24))
            record(argc, argv, 0);
    }

    return 0;
}
