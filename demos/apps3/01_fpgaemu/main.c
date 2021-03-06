/* Эмулятор 240 LE */

#include "SDL.h"

// --------------------
#include "struct.h"
#include "fonts.h"
#include "graph.c"
#include "event.c"
// --------------------

int main(int argc, char* argv[]) {

    int keycode;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    init_graphics("Симулятор FPGA", 1280, 800);

    struct point2d pt[3] = { {200, -20}, {50, 150}, {1400, 800} };
    trif( pt, 0xffffff );
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
