#include <qblib.c>

int ofield[ 320 ][ 200 ]; // Старое поле
int nfield[ 320 ][ 200 ]; // Новое поле

// Инициализация случайными точками
void init() {

    srand(3);
    for (int i = 0; i < 8192; i++) {

        int x = rand() % 320;
        int y = rand() % 200;
        ofield[x][y] = rand() % 16;
    }
}

// Выполнить шаг
void conway() {

    // Перебрать каждую клетку
    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++) {

        int n = 0, cl = 0;

        // 2-3 соседа - клетка живет
        for (int a = -1; a <= 1; a++)
        for (int b = -1; b <= 1; b++) {

            if (a == 0 && b == 0)
                continue;

            int cr = ofield[ (320 + x + a) % 320 ][ (200 + y + b) % 200 ];

            if (cr > 0) { n++; cl += cr; }
        }

        // Если 3 соседа, то зарождение
        if (n == 3) nfield[x][y] = (cl % 15) + 1;
        // Если <2 или >3, то погибает
        else if (n < 2 || n > 3) nfield[x][y] = 0;
    }

    // Копирование новой области в старую
    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++)
        if (y < 192)
            pset(x, y, (ofield[x][y] = nfield[x][y]) ? 10 : 0);
}

int main(int argc, char* argv[]) {

    screen(13);
    load_running_string("ticker.txt");

    qb_scroll_x = 0;
    init();

    int t = 0;

    while (int evt = sdlevent(1)) {

        // Если сработал REDRAW, то разрешить код далее
        if (!(evt & EVT_REDRAW)) continue;

        if ((t % 3) == 0)
            conway();

        // Клик мышкой по полю
        if (mouse.st & LF_CLICK) {

            for (int a = 0; a < 4; a++)
            for (int b = 0; b < 4; b++)
                if (rand() & 1)
                    ofield[ (mouse.x + a)%320 ][ (mouse.y + b)%320 ] = 1;
        }

        if (step_runstr(24)) record(argc, argv, 0);
        t++;
    }

    return 0;
}
