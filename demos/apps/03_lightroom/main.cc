#include <qblib.c>

int     objbuf[320][200];
float   qray[320][200];
float   sqrtval[175000]; // sqrt[0..n]

// Возвращается количество сумму пересечений с объектами
int bzline(int x1, int y1, int x2, int y2) {

    // Инициализация смещений
    int signx  = x1 < x2 ? 1 : -1;
    int signy  = y1 < y2 ? 1 : -1;
    int deltax = x2 > x1 ? x2 - x1 : x1 - x2;
    int deltay = y2 > y1 ? y2 - y1 : y1 - y2;
    int error  = deltax - deltay;
    int error2;
    int match = 0;

    // Перебирать до конца
    while ((x1 != x2) || (y1 != y2)) {

        // Количество пересечений
        match += objbuf[x1][y1];
        error2 = 2 * error;

        // Коррекция по X
        if (error2 > -deltay) {
            error -= deltay;
            x1 += signx;
        }

        // Коррекция по Y
        if (error2 < deltax) {
            error += deltax;
            y1 += signy;
        }
    }

    return match;
}

// Нарисовать сцену и перенести в буфер
void draw_scene() {

    for (int i = 0; i < 255; i++)
        palette(i, i, i, i);

    palette(255, 255, 0, 0);

    linebf(50, 50, 100, 150, -64);
    linebf(60, 40, 90, 120, 0);

    linebf(150, 25, 200, 30, -64);

    circlef(270, 125, 15, -64);
    pset(250, 100, -64);

    circlef(180, 160, 32, -128);
    circlef(180, 160, 28, 0);
    circlef(180, 160, 10, -64);

    // Перенести нарисованные объекты в буфер
    for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++)
        objbuf[x][y] = qb_screen[x][y];
}

int max = -1;

// Вычисление света
void calclight(int lx, int ly) {

    for (int i = 0; i < 200; i++)
    for (int j = 0; j < 320; j++) {

        qray[j][i] = 0;

        // Количество
        int density = bzline(j, i, lx, ly);
            density = density ? (density>>4) : 1;

        // Сколько раз пересеклось с объектами
        int length = (lx-j)*(lx-j) + (ly-i)*(ly-i);
        qray[j][i] = sqrtval[length] * sqrtval[density];

        if (density > max) max = density;
    }

    // Перерисовка экрана
    for (int i = 0; i < 200; i++)
    for (int j = 0; j < 320; j++) {

        // Уровень освещенности
        int lv = 255 - (int) qray[j][i];

        // Освещенность объекта
        int cl = objbuf[j][i] + (lv < 0 ? 0 : lv);

        // Поставить точку
        pset(j, i, (cl < 0 ? 0 : (cl > 254 ? 254 : cl)));
    }

    // Курсор
    pset(lx, ly, 255);
}

// Рисовать квадрат в буфере
void drawbox(int x, int y, int radius, int cl) {

    for (int i = -radius; i <= radius; i++)
    for (int j = -radius; j <= radius; j++) {

        int xo = x + j, yo = y + i;
        if (xo >= 0 && xo < 320 && yo >= 0 && yo < 200) {
            objbuf[xo][yo] = cl;
        }
    }
}

int main(int argc, char* argv[]) {

    screen(13);
    draw_scene();

    int px = -1, py = -1, pst = 0;

    // Предвычисление
    for (int i = 0; i < 175000; i++) sqrtval[i] = sqrt(i);

    // Бесконечный цикл проверки событий
    while (sdlevent(EVT_REDRAW)) {

        // Срабатывает только изменились координаты мыши
        if (mouse.x == px && mouse.y == py && mouse.st == pst) continue;

        // Нарисовать новый объект
        if (mouse.st & LF_CLICK) drawbox(mouse.x, mouse.y, 1, 128);
        if (mouse.st & RT_CLICK) drawbox(mouse.x, mouse.y, 1, 0);

        pst = mouse.st;
        calclight(px = mouse.x, py = mouse.y);

        pset(mouse.x, mouse.y, 255);
    }

    return 0;
}
