#include <sdl2framework.cc>

using namespace std;

int main(int argc, char* argv[]) {

    SDL2framework win(640, 480);

    int k = 0;
    while (win.poll()) {


        for (int y = 0; y < 480; y++)
        for (int x = 0; x < 640; x++)
            win.pixel(x, y, x * y * k);

        k++;

    }

    return 0;
}
