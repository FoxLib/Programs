#include "SDL.h"

// -----------------------
#include "struct.h"
#include "fonts.h"
#include "graph.c"
#include "event.c"
// -----------------------

int main(int argc, char* argv[]) {

    int keycode;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    init_event(1000); // 1s timer
    init_graphics("Own Window", 640, 480);
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

                    /* stub */
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
