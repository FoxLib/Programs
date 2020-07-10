#include <qblib.c>

#define PALETTENUM 5

// Исходная палитра
static const unsigned char pal[PALETTENUM][4] =
{
    {1,     0,   0,  32},
    {64,    0,   0,  64},
    {128,   0,   0, 128},
    {192,   0, 192, 192},
    {255, 255, 255, 255},
};

float t;
float Fp[ 320 ][ 200 ]; // Новое поле
float Fn[ 320 ][ 200 ]; // Предыдущее поле

void init() {

    srand(3);
    for (int i = 0; i < 200; i++)
    for (int j = 0; j < 320; j++)
        Fp[j][i] = 0;

    generate_palette(PALETTENUM, pal);
    load_running_string("ticker.txt");
}

float xy(int x, int y) {

    if (x < 0) x = -x;
    if (x >= 320) x = 639 - x;
    if (y < 0) y = -y;
    if (y >= 200) y = 399 - y;
    return Fn[x][y];
}

// Генератор волн
void waves() {

    int i, j;

    // Генераторы
    if (mouse.st & LF_CLICK) Fn[mouse.x][mouse.y] = (rand()%256) - 16;

    // Спираль
    Fn[ (int)(160 + 30*sin(t)) ][ (int)(100 + 30*cos(t)) ] = 32 + (rand()%16) * sin(0.5*t);

    // Случайные капли
    for (j = 0; j < 4; j++) Fn[ rand() % 320 ][ rand() % 200 ] = (rand() % 32);

    float omega = 0.99; // Релаксация, выше 1.0f не делать
    float laplas;

    /* Основная функция */
    for (i = 0; i < 200; i++) {
        for (j = 0; j < 320; j++) {

            // Расчет лапласиана
            laplas  = xy(j-1, i+0);
            laplas += xy(j+1, i+0);
            laplas += xy(j+0, i-1);
            laplas += xy(j+0, i+1);

            // Дополнительная точность
            laplas += xy(j-1, i-1);
            laplas += xy(j+1, i+1);
            laplas += xy(j-1, i+1);
            laplas += xy(j+1, i-1);
            laplas = (laplas / 8.0f);

            // Итоговое значение
            Fp[j][i] = laplas + omega*(Fn[j][i] - Fp[j][i]);

            // Релаксация, чтобы волны успокаивались
            Fp[j][i] *= 0.999f;
        }
    }

    // Обмен "полей"
    for (i = 0; i < 200; i++)
    for (j = 0; j < 320; j++) {
        float sw = Fp[j][i];
        Fp[j][i] = Fn[j][i];
        Fn[j][i] = sw;
    }

    t = t + 0.04;
}

// Перерисовать
void redraw() {

    for (int y = 0; y < 192; y++) {
        for (int x = 0; x < 320; x++) {

            int a = 128 + 12*(Fn[x][y]);
            a = a < 1 ? 1 : (a > 255 ? 255 : a);
            pset(x, y, a);
        }
    }
}

int main(int argc, char* argv[]) {

    screen(13);
    init();

    while (sdlevent(EVT_REDRAW)) {

        waves();
        redraw();

        color(255, 0);
        if (step_runstr(24)) record(argc, argv);
    }

    return 0;
}
