#include <qblib.c>

#define PALNUMBER 6
#define MSGNUMBER 9
#define RECORDING 0

#define WIDTH  640
#define HEIGHT 360

// Исходная палитра
static const unsigned char pal[PALNUMBER][4] =
{
    {0,     0,   0,   0},
    {16,    0,   0,  64},
    {127, 255,   0,   0},
    {230, 255, 255,   0},
    {250, 255, 255, 192},
    {255, 255, 255, 255}
};

// Инициализация поля
float t;
int   msg_cnt;
int   msg_id;
float fld[ 2 ][ WIDTH ][ 500 ];
float qbs     [ WIDTH ][ 500 ];

// Инициализация поля
void init() {

    t = 0;

    for (int i = 0; i < HEIGHT; i++)
    for (int j = 0; j < WIDTH; j++)
        fld[0][j][i] = 0.0;
}

// Постоянное обновление поля
void update() {

    for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++) {

        if (qbs[x][y]) fld[0][x][y] = qbs[x][y];

        // Сбор суммарной энергии соседей снизу
        float s = fld[0][ (x+WIDTH-1)%WIDTH ][ (y+1) ] +
                  fld[0][ (x+0)%WIDTH ][ (y+1) ] +
                  fld[0][ (x+1)%WIDTH ][ (y+1) ] +
                  fld[0][ x ][ y ];

        s = s/4 * 0.997;
        fld[1][x][y] = s;
    }
}

// Перерисовка и перенос в следующее состояние
void redraw() {

    for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++) {

        fld[0][x][y] = fld[1][x][y];

        int s = 8*fld[0][x][y];
        if (s > 255) s = 255;
        pset(x, y, s);
    }
}

// Нарисовать предметы
void draw() {

    linebf(0, 0, WIDTH, HEIGHT, 0);
    circle(320 + sin(t)*200, 240 + cos(t)*80, 4, -128);
    circle(320 - cos(t)*200, 240 - sin(t)*80, 4, -128);

    for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
        qbs[x][y] = qb_screen[x][y];
}

void sidgen() {

    t += 0.01;

    // Генератор нового сида
    for (int x = 0; x < WIDTH; x++) fld[0][x][479] = 0;
    for (int x = 0; x < 128; x++) fld[0][rand() % WIDTH][HEIGHT-1] = rand()%256;

}

// 15 минут кодинга
int main(int argc, char* argv[]) {

    screen(15);

    init();
    generate_palette(PALNUMBER, pal);

    // Залипательный огонь...
    while (sdlevent(EVT_REDRAW)) {

        sidgen();
        draw();
        update();
        redraw();
        if (record(argc, argv, 1*3600)) break;
    }

    return 0;
}
