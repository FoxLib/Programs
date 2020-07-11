#include "SDL.h"

#define MX 810 // 640
#define MY 500

// -----------------------
#include "struct.h"
#include "fonts.h"
#include "graph.c"
#include "event.c"
// -----------------------

float Fp[ MY ][ MX ];
float Fn[ MY ][ MX ];

void init_flat() {

    int i, j;

    for (i = 0; i < MY; i++)
    for (j = 0; j < MX; j++)
        Fp[i][j] = 0;

}

void redraw() {

    int x, y, a, c;

    for (y = 0; y < MY; y++) {
        for (x = 0; x < MX; x++) {

            a = - (int)(8*Fn[ y ][ x ]);
            a = a < 0 ? 0 : (a > 255 ? 255 : a);
            
            // Цвет
            c = (128 + (a >> 1)) + a*256 + a*65536;
            
            pset(2*x+0, 2*y+0, c);
            pset(2*x+0, 2*y+1, c);
            pset(2*x+1, 2*y+0, c);
            pset(2*x+1, 2*y+1, c);
        }
    }
}

// Получение значения
float xy(int i, int j) {
    
    // Волны "отбиваются"

    // Левая/правая
    if (i < 0) i = -i; else if (i >= MY) i = 2*MY - i;

    // Верх/низ
    if (j < 0) j = -j; else if (j >= MX) j = 2*MX - j;

    return Fn[ i ][ j ];    
    
    // либо смешиваются циклически
    // return Fn[ (i + MY) % MY ][ (j + MX) % MX ];    
}

void waves() {

    int i, j;

    // Генераторы
    for (j = 0; j < 8; j++)
        Fp[ rand() % MY ][ rand() % MX ] = (rand() % 512) - 256;
    
    float omega = 0.995; // Релаксация, выше 1.0f не делать
    float laplas;

    /* Основная функция */
    for (i = 0; i < MY; i++) {
        for (j = 0; j < MX; j++) {

            // Расчет лапласиана
            laplas  = xy(i-1, j);
            laplas += xy(i+1, j);
            laplas += xy(i+0, j-1);
            laplas += xy(i+0, j+1);

            // Дополнительная точность
            laplas += xy(i-1, j-1);
            laplas += xy(i+1, j+1);
            laplas += xy(i-1, j+1);
            laplas += xy(i+1, j-1);
            laplas = (laplas / 8.0f);

            // Итоговое значение
            Fp[i][j] = laplas + omega*(Fn[i][j] - Fp[i][j]);
            
            // Релаксация, чтобы волны успокаивались
            Fp[i][j] *= 0.999f;
        }
    }

    /* Обмен "полей" -- вообще надо иначе это сделать, через указатели */
    float sw;
    for (i = 0; i < MY; i++)
    for (j = 0; j < MX; j++) {
        sw = Fp[i][j];
        Fp[i][j] = Fn[i][j];
        Fn[i][j] = sw;
    }

}

// -------------------------------------------------------------------
int main(int argc, char* argv[]) {

    int keycode;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    init_flat();

    init_event(1000/50); // 1s timer
    init_graphics("Эффект воды", 2*MX, 2*MY);
    flip();

    while (1) {

        /* Снижение нагрузки на процессор */
        SDL_Delay(1);

        /* Опрос событий, произошедших за 1 мс */
        while (SDL_PollEvent(& event)) {

            switch (event.type) {

                /* Если нажата на крестик, то приложение будет закрыто */
                case SDL_QUIT: {
                    return 0;
                }

                /* Вызывается по таймеру */
                case SDL_USEREVENT:

                    waves();
                    redraw();
                    flip();
                    break;

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
