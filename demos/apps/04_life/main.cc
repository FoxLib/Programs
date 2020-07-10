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

static const char marquee[] = "Игра жизнь была создана и разработана Джоном Конвеем в 1970 году. Можно прочесть Википедию, чтобы узнать подробнее. Суть игры в том, чтобы вычислить, какая клетка будет жить, а какая - нет. У клетки есть 8 соседей, которые и решают судьбу клетки в центре. Если соседей 3, то в пустой клетке рождается новая. Если соседей меньше 3 или более 4, то клетка погибает. Так происходит для всего поля, и получаются узоры, которые вы можете увидеть на анимации... Спасибо за внимание.";

int main(int argc, char* argv[]) {

    screen(13);

    qb_scroll_x = 0;
    init();

    int t = 0;
    int p = 0;
    // int obs = 0;

    while (int evt = sdlevent(1)) {

        // Задержка 2 сек. перед записью
        // if (obs++ < 100) continue;

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

        // Бегущая строка
        if ((t % 8) == 0) {

            linebf(0, 192, 319, 199, 0);

            // 39 символов на строке
            if (p <= 40) { locate(40-p, 24); print(mid(marquee, 0,    p));  }
            else         { locate(0,    24); print(mid(marquee, p-40, 40)); }

            p++;
        }
        else {

            for (int i = 192; i < 200; i++)
            for (int j = 0; j < 320; j++)
                pset(j, i, qb_screen[j + 1][i]);
        }

        t++;
    }

    return 0;
}
