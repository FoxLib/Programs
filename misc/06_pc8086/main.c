#include "SDL.h"
#include "basic.c"

int main(int argc, char* argv[]) {

    init("Базовое окно SDL", 640, 480, 10);

    while (1) {

        /* Снижение нагрузки на процессор */
        SDL_Delay(1);

        /* Опрос событий, произошедших за 1 мс */
        while (SDL_PollEvent(& event)) {

            switch (event.type) {

                /* Вызывается по таймеру */
                case SDL_USEREVENT: {
                    break;
                }

                /* Приложение будет закрыто */
                case SDL_QUIT: {
                    return 0;
                }

                /* Нажата мышь */
                case SDL_MOUSEBUTTONDOWN: {
                    break;
                }

                /* Кнопка мыши отжата */
                case SDL_MOUSEBUTTONUP: {
                    break;
                }

                /* Мышь перемещена */
                case SDL_MOUSEMOTION: {
                    break;
                }

                /* Нажата клавиша */
                case SDL_KEYDOWN: {

                    switch (keycode = get_key(event)) {
                        /* ESC */ case 9: return 0;
                    }

                    break;
                }

                /* Отжата клавиша */
                case SDL_KEYUP: {

                    switch (keycode = get_key(event)) {
                        /* stub */
                    }

                    break;
                }
            }
        }

    }

    return 0;
}
