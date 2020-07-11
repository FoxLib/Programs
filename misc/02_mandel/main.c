/* Эмулятор 240 LE */

#include "SDL.h"

// -----------
#include "struct.h"
#include "fonts.h"
#include "graph.c"
#include "event.c"
// -----------

#include "graphics.c"

double x = 0, y = 0,  s = 2;

void redraw() {
    
    draw(x, y, s);
    line(320, 0, 320, 640, 0x808080);
    line(0, 320, 640, 320, 0x808080);        
    flip();
}

int main(int argc, char* argv[]) {

    int keycode;
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    init_graphics("Фрактал Мандельброта", 640, 640);
    
    //draw(-1, 0, 1);
    //draw(-0.73, -0.25, 0.01);
    redraw();

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
                        
                        /* U */ case 111: y -= s / 16; redraw(); break;
                        /* L */ case 113: x -= s / 16; redraw(); break;
                        /* R */ case 114: x += s / 16; redraw(); break;
                        /* D */ case 116: y += s / 16; redraw(); break;
                        
                        /* E */ case 65: s *= 0.5;  redraw(); break;                                            
                        /* S */ case 22: s *= 2;  redraw(); break;
                        
                    }

                    break;
                }
            }
        }
    }

    return 0;
}
