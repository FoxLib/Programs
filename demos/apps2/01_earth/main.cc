#include <sdl2framework.cc>
#include "app.cc"

using namespace std;

int main(int argc, char* argv[]) {

    SDL2framework win(11);
    App app(& win);

    while (win.poll()) {

        if (win.timer()) {
            if (app.update())
                win.record(argc, argv);
        }
    }

    return 0;
}
