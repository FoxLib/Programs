#include <sdl2framework.cc>

using namespace std;
SDL2framework* win;

#define CYLCNT 9

struct TCylinder {

    float width, color, freq, shift;
};

const struct TCylinder cyls[ CYLCNT ] = {
    { 4,  0xf0fff0, 2,  0   },
    { 5,  0x004000, -6, 0   },
    { 7,  0x00c0c0, 8,  0   },
    { 7,  0x008080, 16, 0   },
    { 10, 0x004000, 24, 0   },
    { 10, 0x00a000, 32, 0   },
    { 10, 0x006000, 32, 3.1 },
    { 2,  0xf0fff0, -4, 0   },
    { 8,  0x808080, 6,  0   },
};

float timer = 0;

void update() {

    win->cls(0);
    for (int y = -180; y < 180; y++)
    for (int x = -320; x < 320; x++) {

        int   cl = 0;
        float dist  = sqrt(x*x + y*y);
        float u1;
        float angle = win->atn(x, y);
        float start = 50; // + sin(timer*0.5)*cos(timer*1.2)*10;

        for (int i = 0; i < CYLCNT; i++) {

            // Дистанция попала в диапазон
            if (start <= dist && dist < start + cyls[i].width) {

                u1 = sin(timer + cyls[i].shift + cyls[i].freq*M_PI*angle) > 0;
                cl = u1 ? cyls[i].color : 0;
            }

            start += cyls[i].width;
        }

        // Внешнее кольцо
        if (cl == 0 && dist > start) {

            float u2 = sin(timer + 16*M_PI*angle);
            if (u2 > 0) { cl = (int)(dist*0.6); cl = cl * 256; }
        }
        else {

            // Глаз
            if (dist < 40) {

                cl = 128 + 12*(dist-40);
                cl = win->rgb(2*cl, cl, 255-cl);

                int dist2 = sqrt(6*x*x + y*y);
                if (dist2 < 20) {
                    cl = 32 - 12*(dist-20);
                    cl = win->rgb(0, cl, 255);
                }
            }

            if (y < 0 && dist > 40) {
                int r = cl>>16, g = (cl>>8)&255, b = cl&255;
                cl = win->rgb(r*1.5, g*1.5, b*1.5);
            }
        }

        win->pset(320 + x, 180 + y, -cl);
    }

    timer += 0.1;
}

int main(int argc, char* argv[]) {

    win = new SDL2framework(11);
    while (win->poll()) {

        if (win->timer()) {

            update();
            win->record(argc, argv);
        }
    }

    return 0;
}
