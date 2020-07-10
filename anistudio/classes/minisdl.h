#include <SDL.h>

class MiniSDL {

protected:

    int width, height;

    SDL_Event    event;
    SDL_Surface* sdl_screen;

public:

    MiniSDL(int, int, const char*);

    void loop();
    void pset(int, int, uint);
    void flip();
};
