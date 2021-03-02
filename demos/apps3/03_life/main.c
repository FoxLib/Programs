#include "SDL.h"

#include <stdio.h>
#include <stdlib.h>

// -----------------------
#include "struct.h"
#include "fonts.h"
#include "graph.c"
#include "event.c"
#include "random.c"
// -----------------------

// Должно быть кратно 2
#define FLDSIZE  256

int field [ FLDSIZE ][ FLDSIZE ]; // Старое поле
int fieldn[ FLDSIZE ][ FLDSIZE ]; // Новое поле

// Инициализация поля
void init_field() {

    int i, j, n1, n2;
    for (i = 0; i < FLDSIZE; i++) {
        for (j = 0; j < FLDSIZE; j++) {

            n1 = random_get();
            n2 = random_get();

            if (((n1 ^ n2) & 0x0F) == 0) {
                field[i][j] = 1;
            } else {
                field[i][j] = 0;
            }
        }
    }
}

void redraw() {

    int x, y, a, i, j;

    for (y = 0; y < FLDSIZE; y++) {
        for (x = 0; x < FLDSIZE; x++) {

            a = field[y][x];
            a = a == 1 ? 0xffffff : 0;

            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    pset(3*x + j, 3*y + i, a);
                }
            }
        }
    }
}

void conway() {

    int i, j, a, b, n;

    for (i = 0; i < FLDSIZE; i++) {
        for (j = 0; j < FLDSIZE; j++) {

            n = 0;

            // 2-3 соседа - клетка живет
            for (a = -1; a <= 1; a++) {
                for (b = -1; b <= 1; b++) {

                    if (a == 0 && b == 0) {
                        continue;
                    }

                    // 0 .. 255, 256 = 0, -1 = 255 ...
                    if (field[ (i + a) & (FLDSIZE - 1) ][ (j + b) & (FLDSIZE - 1) ]) {
                        n++;
                    }
                }
            }

            // Если 3 соседа, то зарождение
            if (n == 3) {
                fieldn[i][j] = 1;
            }
            // Если <2 или >3, то погибает
            else if ( n < 2 || n > 3 ) {
                fieldn[i][j] = 0;
            }
        }
    }

    // Копирование новой области в старую
    for (i = 0; i < FLDSIZE; i++) {
        for (j = 0; j < FLDSIZE; j++) {
            field[i][j] = fieldn[i][j];
        }
    }
}

int main(int argc, char* argv[]) {

    int keycode;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    init_graphics("Игра Жизнь", 768, 768);
    init_event(25);

    random_seed(1);
    init_field();

    while (1) {

        /* Снижение нагрузки на процессор */
        SDL_Delay(1);

        /* Опрос событий, произошедших за 1 мс */
        while (SDL_PollEvent(& event)) {

            switch (event.type) {

                /* Вызывается по таймеру */
                case SDL_USEREVENT:

                    conway();
                    redraw();
                    flip();

                    break;

                /* Если нажата на крестик, то приложение будет закрыто */
                case SDL_QUIT: {
                    return 0;
                }

                /* Нажата мышь */
                case SDL_MOUSEBUTTONDOWN:

                    break;

                /* Кнопка мыши отжата */
                case SDL_MOUSEBUTTONUP:

                    break;

                /* Мышь перемещена */
                case SDL_MOUSEMOTION:

                    break;

                /* Нажата какая-то клавиша */
                case SDL_KEYDOWN: {

                    keycode = get_key(event);

                    switch (keycode) {
                        /* ESC */ case 9: return 0;
                    }

                    break;
                }
            }
        }
    }

    return 0;
}
