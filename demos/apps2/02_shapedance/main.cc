#include <sdl2framework.cc>

using namespace std;

SDL2framework* win;

float t = 0;
const float PI = 3.141592;
const float tri[3] = {0, 2*PI/3, 4*PI/3};
const float square[4] = {0, 2*PI/4, 2*2*PI/4, 3*2*PI/4};

// Рисование выпуклой фигуры
void closeup(int k, float t, struct vec3i ap, const float shape[], int cl) {

    int x = ap.x, y = ap.y, r = ap.z;

    for (int i = 0; i < k; i++) {
        win->line(
            x+sin(t+shape[i])*r,       y+cos(t+shape[i])*r,
            x+sin(t+shape[(i+1)%k])*r, y+cos(t+shape[(i+1)%k])*r,
            cl
        );
    }
}

void update() {

    int w = win->w(), h = win->h();

    // Ослабление цветов
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {

        struct vec3i cl = win->i2rgb( win->point(x, y) );

        cl.x *= 0.92;
        cl.y *= 0.92;
        cl.z *= 0.92;

        // Фоновые облака
        float b =  win->fbm((float)x/100 + t, (float)y/100 + sin(t))*16;

        cl.x += b*0.5;
        cl.y += b*0.75;
        cl.z += b;

        win->pset(x, y, -win->rgb(cl.x, cl.y, cl.z) );
    }

    // Рисование фигур
    win->circle(160, h/2, 10 + abs(sin(t))*64.0, 10);
    win->circle(160, h/2, 10 + abs(cos(t))*32.0, 12);
    win->circle(160, h/2, 10 + abs(sin(t)*cos(t))*48.0, 14);

    // Вращение фигур
    closeup(3, t, {50, h/2, (int)(50 + sin(t)*25) }, tri, 13);
    closeup(4, t, {270, h/2, (int)(50 + cos(t)*25) }, square, 11);

    t += 0.05;
}

int main(int argc, char* argv[]) {

    win = new SDL2framework(14);
    while (win->poll()) {

        if (win->timer()) {

            update();
            win->record(argc, argv);
        }
    }

    return 0;
}
