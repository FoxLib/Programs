#define BIGNUM_SIZE 6
#define BIGNUM_FRAC 4

#include <qblib.c>
#include <bignum.cc>

// Расчет фрактала
int mandel(bignum cx, bignum cy) {

    bignum x(0), y(0);
    bignum a, b, c, d, e;
    bignum value2(2);
    bignum value4(4);

    for (int i = 0; i < 256; i++) {

        a = x*x;
        b = y*y;
        e = a+b;
        d = a-b+cx;
        y = x*y*2+cy;   // y' = 2*x*y + cy
        x = d;          // x' = x^2 - y^2 + cx

        // x^2 + y^2 >= 4, выход
        if (e.cmp(value4) >= 0) return i;
    }

    return 255;
}

// sudo lshw
int main() {

    screen(13);

    for (int i = 0; i < 256; i++) {

        int s = sin(2*i*M_PI/256 - M_PI/2)*127;
        int r = 128 + s;
        int g = 128 + s;
        int b = 192 + s*0.5;
        palette(i, r, g, b);
    }

    int iter = 0;
    int fillrate = 320;
    int mx = -1, my = -1, st = -1;
    int deep = 0;

    // Начальные значения
    bignum x0(-1), y0(0), f(1);

    while (sdlevent(EVT_REDRAW)) {

        double xm = (float)(mx - 160) / 100.0;
        double ym = (float)(100 - my) / 100.0;

        mx = mouse.x;
        my = mouse.y;

        // Заполнить динамически меняемый fillrate
        for (int i = 0; i < fillrate; i++) {

            if (iter < 64000) {

                int x = (iter % 320) - 160,
                    y = (iter / 320) - 100;

                // Смещения в координатах экрана
                bignum dx((double)x / 100);
                bignum dy((double)y / 100);

                int cl = mandel(dx*f + x0, dy*f + y0);

                pset(160 + x, 100 + y, cl & 255);
                iter++;
            }

            if (iter == 63999) {
                locate(0, 0); color(255, -1); print(itoa(deep));
            }
        }

        // Автоподстройка
        if (iter < 64000) {

            if (sdl_trigger_frames > 1)
                 fillrate = (2*fillrate)/3;
            else fillrate = (3*fillrate)/2;

            if (fillrate < 1)
                fillrate = 1;
            else if (fillrate > 64000)
                fillrate = 64000;
        }
        // Интерактивное управление
        else {

            // Приближение
            if ((mouse.st & LF_CLICK) && !(st & LF_CLICK)) {

                x0 = x0 + f*xm;
                y0 = y0 - f*ym;
                f  = f * 0.25;
                iter  = 0;
                deep += 4;

                // Текущее положение
                printf("x="); x0.print(); printf(" ");
                printf("y="); y0.print(); printf(" ");
                printf("f=");  f.print(); printf("\n\n");
            }
            // Отдаление
            else if ((mouse.st & RT_CLICK) && !(st & RT_CLICK)) {

                f     = f * 8.0;
                iter  = 0;
                deep -= 4;
            }
        }

        st = mouse.st;
    }

    return 0;
}
