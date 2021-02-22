#include <sdl2framework.cc>

using namespace std;

int main(int argc, char* argv[]) {

    SDL2framework win(13);

    int k = 0;
    while (win.poll()) {

        if (win.timer()) {

            k++;
            for (int y = 0; y < 240; y++)
            for (int x = 0; x < 320; x++)
                win.pset(x, y, sin(x)*cos(y)*sin((float)k/256)*256);
        }
    }

    return 0;
}
