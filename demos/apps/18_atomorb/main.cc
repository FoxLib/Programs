#include <qblib.c>

const float ppd      = 200;
const float ppd_near = 0.05;

enum FNTypes {
    fn_s = 0,
    fn_p = 1,
};

float zbuf[320][200];

vec3 cam;
vec3 rotcam;

// Удалить Z-буйфер
void clear() {

    for (int i = 0; i < 200; i++)
    for (int j = 0; j < 320; j++) {
        zbuf[j][i] = 1000.0;
        pset(j, i, 0);
    }
}

void atom(int fn, float radius, vec3 rot, int iter) {

    srand(1);

    float a = 0, b = 0, r = 0;
    vec3  el = {0, 0, 0};

    // Вращение объекта
    vec2  rt[3] =
    {
        {cos(rot.x), sin(rot.x)}, // X
        {cos(rot.y), sin(rot.y)}, // Y
        {cos(rot.z), sin(rot.z)}  // Z
    };

    // Вращение камеры
    vec2  cm[3] =
    {
        {cos(rotcam.x), sin(rotcam.x)}, // X
        {cos(rotcam.y), sin(rotcam.y)}, // Y
        {cos(rotcam.z), sin(rotcam.z)}  // Z
    };

    for (int i = 0; i < iter; i++) {

        // Случайные точки на орбитали
        a = (float)(rand()%1000) / 1000.0 * 2.0 * M_PI;
        b = (float)(rand()%1000) / 1000.0 * 2.0 * M_PI;

        // Сферическая
        if (fn == fn_s) {

            r  = radius;
            el = {sin(a) * cos(b) * r, sin(b) * r, cos(a) * cos(b) * r};
        }
        // Гантели
        else if (fn == fn_p) {

            r  = sin(2*a) / 3;
            el = {sin(b) * r, sin(a) * radius, cos(b) * r};
        }

        // Rotate Object
        el = {el.x, rt[0].x * el.y - rt[0].y * el.z, rt[0].x * el.z + rt[0].y * el.y}; // X
        el = {rt[1].x * el.x - rt[1].y * el.z, el.y, rt[1].x * el.z + rt[1].y * el.x}; // Y
        el = {rt[2].x * el.x - rt[2].y * el.y, rt[2].x * el.y + rt[2].y * el.x, el.z}; // Z

        // Rotate Camera
        el = {el.x, cm[0].x * el.y - cm[0].y * el.z, cm[0].x * el.z + cm[0].y * el.y}; // X
        el = {cm[1].x * el.x - cm[1].y * el.z, el.y, cm[1].x * el.z + cm[1].y * el.x}; // Y
        el = {cm[2].x * el.x - cm[2].y * el.y, cm[2].x * el.y + cm[2].y * el.x, el.z}; // Z

        float zp = el.z - cam.z;

        // Точку рисовать можно
        if (zp > ppd_near) {

            int xm = 160 + (el.x + cam.x)*ppd / zp;
            int ym = 100 - (el.y + cam.y)*ppd / zp;
            int cl = 255 - zp*64;

            // Рисовать только в обозначенной области
            if (xm >= 0 && ym >= 0 && xm < 320 && ym < 200) {

                if (cl < 0) cl = 0;
                if (cl > 255) cl = 255;

                if (zbuf[xm][ym] > zp) {
                    zbuf[xm][ym] = zp;
                    pset(xm, ym, cl);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {

    cam = {0, 0, -0.5};
    rotcam = {0, 0, 0};

    screen(13);

    for (int i = 0; i < 256; i++)
        palette(i, i, i, i);

    int type = 1;
    while (sdlevent(EVT_REDRAW)) {

        int key = keypress();

        clear();

        // Первый уровень
        if (type >= 1) atom(fn_s, 0.075, {0,0,0}, 4096);

        // Второй уровень
        if (type >= 2) atom(fn_s, 0.25, {0,0,0}, 2048);
        if (type >= 3) atom(fn_p, 1, {0, 0, 0}, 32768/2);
        if (type >= 4) atom(fn_p, 1, {3.1415/2, 0, 0}, 32768/2);
        if (type >= 5) atom(fn_p, 1, {0, 0, 3.1415/2}, 32768/2);

        // Вращение
        rotcam.x = (float)mouse.y/50;
        rotcam.y = (float)mouse.x/50;

        // Приблизить, отдалить
        if (mouse.st & 1) cam.z += 0.025;
        if (mouse.st & 2) cam.z -= 0.025;

        // Переключать 1..5
        if (key >= '1' && key <= '5') {
            type = key - '0';
        }

        if (key == 'P') { saveppm("shot.ppm"); }
    }

    return 0;
}
