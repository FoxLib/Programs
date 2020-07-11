#include "SDL.h"

class SDLClass {

protected:

    int width, height;

    SDL_Event    event;
    SDL_Surface* sdl_screen;

public:

    SDLClass(int, int, const char*);

    void loop();
    void pset(int, int, uint);
    void flip();
};
