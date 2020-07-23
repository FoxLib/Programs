#include <qblib.c>

#define SAND(x,y,dx,dy) buf[1][x+(dx)][y+(dy)] = buf[0][x][y]; buf[1][x][y] = 0

float timer;
int buf[2][640][360];

// Инициализация буфера, рисование препятствий и начального состояния
void init() {

    // Инициализация
    for (int i = 0; i < 2; i++)
    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++)
        buf[i][x][y] = 0;

    // Генератор блока
    for (int y = 150; y < 200; y++) for (int x = 40; x < 100; x++) buf[1][x][y] = 7;
    for (int y = 100; y < 108; y++) for (int x = 190; x < 250; x++) buf[1][x][y] = 7;

    // Воронка
    for (int x = 0; x < 74; x++) {

        buf[1][75 +  x][40] = 7;
        buf[1][225 - x][40] = 7;
        buf[1][110 + x][50] = 7;
        buf[1][75 + x][40 + x] = 7;
        buf[1][225 - x][40 + x] = 7;
    }

    // Заполняем песком
    for (int y = 52; y < 120; y++)
    for (int x = 75; x < 225; x++)
        if ((x - y - 35 > 0) && (265 - x - y > 0))
            buf[1][x][y] = rand()&1 ? 14 : 6;

}

// Генерация нового песка
void sand() {

    for (int k = 0; k < 8; k++) {

        int cx = (int)(mouse.x) + rand()%4 - 2;
        int cl = 0;

        if (cx < 0 || cx > 319) continue;

        switch (rand() % 2) {
            case 0: cl = 6; break;
            case 1: cl = 14; break;
        }

        buf[1][cx][mouse.y] = cl;
    }

    timer += 0.1;
}

// Клеточный автомат
void step() {

    for (int y = 0; y < 199; y++)
    for (int x = 1; x < 319; x++) {

        int p = buf[0][x][y];

        // Информация о клетках снизу
        int a = buf[0][x-1][y+1];
        int b = buf[0][x  ][y+1];
        int c = buf[0][x+1][y+1];

        // Движение песка
        if (p == 6 || p == 14) {

            // Под песчинкой ничего нет
            if (b == 0) { SAND(x,y,0,1); }
            // Нет слева или справа (и не мешают стены)
            else {

                int can_lf = 0, can_rt = 0, can_move = 0;

                if (a == 0 && buf[0][x-1][y] == 0) can_lf = 1;
                if (c == 0 && buf[0][x+1][y] == 0) can_rt = 1;

                // Если песок может двинуться влево и вправо
                if (can_lf && can_rt) can_move = rand()&2 ? 1 : -1;
                else if (can_lf)      can_move = -1;
                else if (can_rt)      can_move = 1;

                // Если песок может двинуться. Может и не двинуться с вероятность 25%
                if (can_move != 0 && (rand()%4 != 0) ) { SAND(x,y,can_move,1); }
            }

        }
    }

    // Перенести
    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++) {
        buf[0][x][y] = buf[1][x][y];
        pset(x, y, buf[0][x][y]);
    }
}

int main(int argc, char* argv[]) {

    screen(13);
    init();

    while (sdlevent(EVT_REDRAW)) {

        sand();
        step();
    }

    return 0;
}
