#include <sdl2framework.cc>

using namespace std;
SDL2framework* win;

int main(int argc, char* argv[]) {

    win = new SDL2framework(14);
    while (win->poll()) {

        if (win->timer()) {
            win->record(argc, argv);
        }
    }

    return 0;
}
