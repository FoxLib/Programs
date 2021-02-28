#include <sdl2framework.cc>

using namespace std;
SDL2framework* win;

struct droplet {

    struct vec2 pos;
    struct vec2 spd;
    float color;
};

int timer;
const int maxdrop = 8192;

struct droplet droplets[16384];

void update() {

    win->cls(0);

    int w = win->w(),
        h = win->h();

    struct vec3i destcl;

    for (int x = 0; x < w; x++) {

        int y = 160 + win->fbm((float)x/75, 0)*64;
        win->line(x, y, x, h, -0x202020);
    }

    for (int i = 0; i < maxdrop; i++) {

        int cl = droplets[i].color;
        float x = droplets[i].pos.x;
        float y = droplets[i].pos.y;

        // Дождь или брызги
        if (cl < 256 && cl >= 0)
             destcl = {0, cl>>2, cl>>1};
        else destcl = {96, 96, 96};

        // Фонарный столб
        if (y > h - 100) {

            int m = x - 300;
            int n = ((h-120)-y)/2;

            if ((m < 0 && m > n) || (m > 0 && m < -n) )
                destcl = {cl, cl, cl};
        }

        // Рисование точки
        win->pset(x, y, -win->rgb(destcl.x, destcl.y, destcl.z));

        // Ускорение движения частиц
        if (droplets[i].spd.y > 2)       droplets[i].spd.y = 2;
        else if (droplets[i].spd.y <= 2) droplets[i].spd.y += 0.4;

        if (droplets[i].spd.x < -2) droplets[i].spd.x = -2;
        if (droplets[i].spd.x >  2) droplets[i].spd.x =  2;

        droplets[i].spd.x = sin((float)timer/360);

        // Скорость частиц
        x = x + droplets[i].spd.x;
        y = y + droplets[i].spd.y;

        // Циклическое перемещение
        if (x < 0) x += w;
        else if (x >= w) x -= w;

        if (y > h) {

            // Отскок
            if (droplets[i].color < 256 && droplets[i].color >= 0) {

                x += (rand()%8 - 4);
                droplets[i].spd.y = -droplets[i].spd.y;
                droplets[i].color = 256;

            } else {

                droplets[i].spd.y = ((rand() % 4) / 2 + 0.5);
                droplets[i].color = rand() & 255;
                y = 0;
            }
        }

        droplets[i].pos.x = x;
        droplets[i].pos.y = y;
    }

    // Фонарный столб
    for (int y = h-110; y < h; y++)
    for (int x = 300-6; x < 300+6; x++) {

        if (y > h-106 && (x < 300-2 || x >= 300+2))
            continue;

        int addc = (y == h-106 ? 192 : 32);
        struct vec3i m = win->i2rgb( win->point(x, y) );

        win->pset(x, y, -win->rgb(m.x + addc, m.y + addc, m.z + addc));
    }

    win->locate(1, 1);
    win->color(7, -1);
    win->print("Дождливая ночь");

    timer++;
}

int main(int argc, char* argv[]) {

    win = new SDL2framework(11);
    timer = 0;

    // Инициализация капель
    for (int i = 0; i < maxdrop; i++) {

        droplets[i].pos = { (float)(rand()%win->w()), (float)(rand()%win->h()) };
        droplets[i].spd = { 0, (float)((rand() % 4) / 2 + 0.5) };
        droplets[i].color = rand() & 255;
    }

    while (win->poll()) {

        if (win->timer()) {

            update();
            win->record(argc, argv);
        }
    }

    return 0;
}
