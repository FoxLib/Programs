#include <qblib.c>

#define PALETTENUM 5

#define WIDTH  640
#define HEIGHT 360

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
float Fp[ WIDTH ][ HEIGHT ]; // Новое поле
float Fn[ WIDTH ][ HEIGHT ]; // Предыдущее поле

void init() {

    srand(3);
    for (int i = 0; i < HEIGHT; i++)
    for (int j = 0; j < WIDTH; j++)
        Fp[j][i] = 0;

    generate_palette(PALETTENUM, pal);
    load_running_string("ticker.txt");
}

float xy(int x, int y) {

    if (x < 0) x = -x;
    if (x >= WIDTH) x = 2*WIDTH - x - 1;
    if (y < 0) y = -y;
    if (y >= HEIGHT) y = 2*HEIGHT - y - 1;
    return Fn[x][y];
}

// Генератор волн
void waves() {

    int i, j;

    // Генераторы
    if (mouse.st & LF_CLICK) Fn[mouse.x][mouse.y] = (rand()%256) - 16;

    // Спираль
    Fn[ (int)(WIDTH/2 + 90*sin(t)) ][ (int)(HEIGHT/2 + 90*cos(t)) ] = 32 + (rand()%16) * sin(0.5*t);

    // Случайные капли
    for (j = 0; j < 4; j++) Fn[ rand() % WIDTH ][ rand() % HEIGHT ] = (rand() % 32);

    float omega = 0.99; // Релаксация, выше 1.0f не делать
    float laplas;

    /* Основная функция */
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {

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
    for (i = 0; i < HEIGHT; i++)
    for (j = 0; j < WIDTH; j++) {
        float sw = Fp[j][i];
        Fp[j][i] = Fn[j][i];
        Fn[j][i] = sw;
    }

    t = t + 0.04;
}

// Перерисовать
void redraw() {

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            int a = 128 + 12*(Fn[x][y]);
            a = a < 1 ? 1 : (a > 255 ? 255 : a);
            pset(x, y, a);
        }
    }
}

int main(int argc, char* argv[]) {

    screen(15);
    init();

    while (sdlevent(EVT_REDRAW)) {

        waves();
        redraw();

        record(argc, argv, 3600);
    }

    return 0;
}
