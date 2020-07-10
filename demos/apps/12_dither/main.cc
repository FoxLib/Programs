#include <qblib.c>

int main(int argc, char* argv[]) {

    screen(13);
    ditherpal();

    float t = 0;
    while (sdlevent(EVT_REDRAW)) {

        cls();
        useindex(0);

        for (int i = 0; i < 192; i++)
        for (int j = 0; j < 320; j++) {

            vec2 src_r = {160 + sin(t)*100, 100 + cos(t)*100};
            vec2 src_g = {160 - sin(t)*100, 100 + cos(t)*100};
            vec2 src_b = {160 + sin(t)*100, 100 - cos(t)*100};

            int r = 255 - sqrt(pow(j-src_r.x, 2) + pow(i-src_r.y, 2));
            int g = 255 - sqrt(pow(j-src_g.x, 2) + pow(i-src_g.y, 2));
            int b = 255 - sqrt(pow(j-src_b.x, 2) + pow(i-src_b.y, 2));

            pset(j, i, rgb(r, g, b));
        }

        t = t + 0.01;
        int middle = 160 + 160*sin(t);
        line(middle, 0, middle, 192, 0xffffff);

        // Сам алгоритм дизеринга
        ditherect(0, 0, middle, 192, 1);

        // Для записи видео
        color(255, 0);
        locate(0, 24);
        print(" 256 Дизериг       |   24-х битный цвет");
        record(argc, argv, 628);
    }

    return 0;
}
