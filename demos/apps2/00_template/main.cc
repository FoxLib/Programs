#include <sdl2framework.cc>

using namespace std;

int main(int argc, char* argv[]) {

    SDL2framework win(13);

    int k = 0;
    while (win.poll()) {

        if (win.timer()) {

            k++;
            for (int y = 0; y < win.h(); y++)
            for (int x = 0; x < win.w(); x++)
                win.pset(x, y, x*y/256 - k);

            win.record(argc, argv);
        }
    }

    return 0;
}
