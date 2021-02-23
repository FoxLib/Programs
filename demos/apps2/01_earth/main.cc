#include <sdl2framework.cc>
#include "app.cc"

using namespace std;

int main(int argc, char* argv[]) {

    SDL2framework win(13);
    App app(& win);

    while (win.poll()) {

        if (win.timer()) {
            app.update();
        }
    }

    return 0;
}
