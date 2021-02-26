#include <sdl2framework.cc>

using namespace std;

int main(int argc, char* argv[]) {

    SDL2framework win(14);

    while (win.poll()) {

        if (win.timer()) {
            win.record(argc, argv);
        }
    }

    return 0;
}
