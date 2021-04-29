#include <sdl2framework.cc>

using namespace std;
SDL2framework* win;

int   mouse_active;
int   curr_x, curr_y;
int   last_x, last_y;
float speed_x, speed_y;

void update() {

    for (int y = 0; y < 640; y++)
    for (int x = 0; x < 640; x++) {
        uint8_t b = ((x - curr_x) ^ (y - curr_y));
        win->pset(x, y, -win->rgb(b, b, b));
    }

    if (mouse_active == 0) {

        curr_x += speed_x;
        curr_y += speed_y;

        speed_x *= 0.92;
        speed_y *= 0.92;
    }
}

int main(int argc, char* argv[]) {

    speed_x = 0;
    speed_y = 0;
    curr_x  = 0;
    curr_y  = 0;
    mouse_active = 0;

    win = new SDL2framework(11);
    while (win->poll()) {

        int mx = win->get_mouse_x(),
            my = win->get_mouse_y();

        if (win->timer()) {

            update();
            win->record(argc, argv);
        }

        // Мышка нажата
        if (win->mouse_down()) {

            last_x = win->get_mouse_x();
            last_y = win->get_mouse_y();
            mouse_active = 1;
        }

        // Движение мышью
        if (win->mouse_move()) {

            if (mouse_active) {

                // Последняя скорость
                speed_x = mx - last_x;
                speed_y = my - last_y;

                // Добавление к текущему положению
                curr_x += speed_x;
                curr_y += speed_y;

                last_x  = mx;
                last_y  = my;
            }
        }

        // Мышка отпущена
        if (win->mouse_up()) {
            mouse_active = 0;
        }

    }

    return 0;
}
