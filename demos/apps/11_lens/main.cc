#include <qblib.c>

// Солнечный свет
void draw_sun(int x0, int y0) {

    float al = atan2(x0 - 160, y0 - 100);

    for (int y = -100; y < 100; y++)
    for (int x = -100; x < 100; x++) {

        int rx = (x + x0), ry = (y + y0);
        if (rx < 0 || ry < 0 || rx >= 320 || ry >= 200)
            continue;

        float r = sqrt(x*x + y*y);
        if (r < 100) {

            // Есть засветка яркости
            int cl = (400 - r*6);
            if (cl < 0) cl = 0;

            // Гало вокруг солнца
            if (r > 40 && r < 48) cl += 32/(r-39);

            // Добавление предыдущего цвета
            cl += point(rx, ry);
            if (cl > 255) cl = 255;
            if (cl < 0) cl = 0;
            pset(rx, ry, cl);
        }
    }

    // Лучи
    for (int i = 0; i < 48; i++) {

        float an = (float)(rand()%6280) / 1000;
        float ln = rand()%256 + 64;
        for (int j = 0; j < 64; j++) {

            float x = x0 + sin(an + al)*ln*((float)j / 128);
            float y = y0 + cos(an + al)*ln*((float)j / 128);

            int cl = point(x, y) + 8;
            if (cl > 255) cl = 255;
            pset(x, y, cl);
        }
    }
}

// Линза
void lens(int x0, int y0, int rad) {

    for (int y = -rad; y < rad; y++)
    for (int x = -rad; x < rad; x++) {

        float r = sqrt(x*x + y*y);

        // -2 Чтобы края были не такими четкими
        if (r < rad - 2) {

            int cl = 192 / (rad - r);
            int pc = point(x0 + x, y0 + y);

            cl = pc + cl;
            if (cl > 255) cl = 255;
            pset(x0 + x, y0 + y, cl);
        }
    }
}

void update_scene(int x, int y) {

    srand(1);

    for (int i = 0; i < 255; i++)
        pset((rand()%640) - 320 + x, (rand()%400) - 200 + y, rand()%128);

    draw_sun(x, y);
    vec2i ray = {160 - x, 100 - y};

    lens(160 + ray.x*0.1, 100 + ray.y*0.1, 12);
    lens(160 + ray.x*0.3, 100 + ray.y*0.4, 32);
    lens(160 + ray.x*0.7, 100 + ray.y*0.7, 52);

    color(255, -1);
    locate(1, 22); print("ВЕРСИЯ 2");
    locate(1, 23); print("Изменение движения лучей");
    color(255, 0);
}

int main(int argc, char* argv[]) {

    screen(13);
    for (int i = 0; i < 256; i++) palette(i, i, i, i);

    float t = 0;
    while (sdlevent(EVT_REDRAW)) {

        cls();

        int x = cos(t)*160 + 160;
        int y = sin(3*t)*80 + 100;

        update_scene(x, y);
        record(argc, argv, 60);

        t += 0.002;
    }

    return 0;
}
