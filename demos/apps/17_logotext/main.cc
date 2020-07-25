#include <qblib.c>

void typeprint(int x, int y, int ch, int cl) {

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            for (int m = 0; m < 4; m++)
                pset(x + 2*j + (m>>1), y + 2*i + (m%2), QB_FONT_8x16[ch][i] & (1<<(7-j)) ? cl : 0);
        }
    }
}

const char helloworld[] = "Press ANY Key To Start...";

int main(int argc, char* argv[]) {

    screen(15);

    color(15, 0); cls();

    int id = 0;
    int x = 120, y = 164, fps = 0, next = 7, blink = 0;

    while (sdlevent(EVT_REDRAW)) {

        if (++fps == next) {

            fps = 0;

            // Пропечатать букву
            if (helloworld[id]) {

                typeprint(x,    y, helloworld[id++], 10);
                typeprint(x+16, y, '_', 15);

                x += 16;
                next = (rand() % 2) + 1;
            }
            else {

                typeprint(x, y, (blink++)%2 ? ' ' : '_', 15);
                next = 12;
            }
        }

        if (record(argc, argv, 2*60)) break;

    }

    return 0;
}
