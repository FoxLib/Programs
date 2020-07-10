#include <qblib.c>

#define PALNUMBER 6
#define MSGNUMBER 9
#define RECORDING 0

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

// Список сообщений
static const char* messages[MSGNUMBER] = {
    "Пишу о том, что...",
    "У попа была собака",
    "Он собаку эту любил",
    "Но потом собака",
    "Собака совершила нечто ужасное",
    "Она съела кусок мяса!",
    "И поп с ней сделал кое-что",
    "Он ее похвалил!",
    "И в тетрадке так и записал:",
};

// Инициализация поля
float t;
int   msg_cnt;
int   msg_id;
float fld[ 2 ][ 320 ][ 200 ];
float qbs[ 320 ][ 200 ];

// Инициализация поля
void init() {

    t = 0;
    msg_id  = 0;
    msg_cnt = 0;
    qb_scroll_x = 0;

    for (int i = 0; i < 200; i++)
    for (int j = 0; j < 320; j++)
        fld[0][j][i] = 0.0;
}

// Постоянное обновление поля
void update() {

    for (int y = 0; y < 199; y++)
    for (int x = 0; x < 320; x++) {

        if (qbs[x][y]) fld[0][x][y] = qbs[x][y];

        // Сбор суммарной энергии соседей снизу
        float s = fld[0][ (x+319)%320 ][ (y+1) ] +
                  fld[0][ (x+0)%320 ][ (y+1) ] +
                  fld[0][ (x+1)%320 ][ (y+1) ] +
                  fld[0][ x ][ y ];

        s = s/4 * 0.99;
        fld[1][x][y] = s;
    }
}

// Перерисовка и перенос в следующее состояние
void redraw() {

    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++) {

        fld[0][x][y] = fld[1][x][y];

        int s = 8*fld[0][x][y];
        if (s > 255) s = 255;
        if (y < 192) pset(x, y, s);
    }
}

// Нарисовать предметы
void draw() {

    linebf(0, 0, 319, 191, 0);
    circle(160 + sin(t)*100, 100 + cos(t)*40, 4, -128);
    circle(160 - cos(t)*100, 100 - sin(t)*40, 4, -128);

    if (msg_cnt < 200) {

        locate((40 - len(messages[msg_id]))/2, 17);
        color(32 + (8*msg_cnt/200), -1);
        print(messages[msg_id]);
    }
    else if (msg_cnt > 250) {

        msg_cnt = 0;
        msg_id = (msg_id + 1) % MSGNUMBER;
    }

    msg_cnt++;

    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++)
        qbs[x][y] = qb_screen[x][y];
}

void sidgen() {

    t += 0.01;

    // Генератор нового сида
    for (int x = 0; x < 320; x++) fld[0][x][194] = 0;
    for (int x = 0; x < 128; x++) fld[0][rand() % 320][194] = rand()%256;

}

// 15 минут кодинга
int main(int argc, char* argv[]) {

    screen(13);
    init();
    generate_palette(PALNUMBER, pal);
    load_running_string("ticker.txt");

    while (sdlevent(EVT_REDRAW)) {

        sidgen();
        draw();
        update();
        redraw();

        // Бегущая строка
        color(255, 0); step_runstr(24);
        if (RECORDING) record2ppm("record");
    }

    return 0;
}
