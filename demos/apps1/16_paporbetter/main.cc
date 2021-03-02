#include <qblib.c>

// http://algolist.ru/graphics/fern.php
// http://grafika.me/node/184

#define MAXIT 20

void leaf() {

    double   x = 0, y = 0, newx, newy;
    int      rand_num;

    for (int i = 0; i < MAXIT; i++)
    {
        rand_num = rand();

        if (rand_num < 0.01 * RAND_MAX)
        {
            newx = 0;
            newy = 0.16 * y;
        }
        else if (rand_num < 0.86 * RAND_MAX)
        {
            newx = ( 0.85 * x) + (0.04 * y);
            newy = (-0.04 * x) + (0.85 * y) + 1.6;
        }
        else if (rand_num < 0.93 * RAND_MAX)
        {
            newx = (0.20 * x) - (0.26 * y);
            newy = (0.23 * x) + (0.22 * y) + 1.6;
        }
        else
        {
            newx = (-0.15 * x) + (0.28 * y);
            newy = ( 0.26 * x) + (0.24 * y) + 0.44;
        }

        x = newx;
        y = newy;
    }

    pset (320 + 45*x, 480 - 45*y, 10);
}

int main(int argc, char* argv[]) {

    screen(14);

    while (sdlevent(EVT_REDRAW)) {
        for (int i = 0; i < 64; i++)
            leaf();
    }
    return 0;
}
