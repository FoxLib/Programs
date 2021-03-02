#include <qblib.c>

float buf[2][640][480];
float k;

void update() {

    for (int y = -100; y < 100; y++)
    for (int x = -100; x < 100; x++) {

        int rx = 160 + x,
            ry = 100 + y;

        // Расстояние
        float r = sqrt(x*x + y*y);

        // Скорость вращения
        float a = 0.05 / (0.05 + r*0.1) + sin(k)*0.15;

        // Притяжение
        float f = 1.05;

        // Целочисленный баг
        if (r < 16) f = 1.5;

        // Вращение
        int px = f*(cos(a) * (double)x - sin(a) * (double)y);
        int py = f*(cos(a) * (double)y + sin(a) * (double)x);

        // Перерисовка старой точки
        pset(rx, ry, buf[0][rx][ry]);

        // Новая точка
        buf[1][rx][ry] = buf[0][(160 + px)%320][(100 + py)%200];
    }

    // Действие
    k += 0.01;

    // Перемещение полей
    for (int y = 0; y < 200; y++) for (int x = 0; x < 320; x++) buf[0][x][y] = buf[1][x][y];
}

int main(int argc, char* argv[]) {

    k = 0;
    screen(13);

    // Палитра
    for (int i = 0; i < 255; i++) palette(i, i, i, i);

    while (sdlevent(EVT_REDRAW)) {

        // Случайные точки
        for (int i = 0; i < 256; i++) {
            buf[0][rand()%640][rand()%480] += rand()%128;
        }

        update();
        record(argc, argv, 3600);
    }

    return 0;
}
