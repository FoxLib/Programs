#include <qblib.c>

#define CHECKPOINT_NUMBER 7

static const double checkpoint[ CHECKPOINT_NUMBER ][ 2 ] = {

    {-0.373972789840348263101788006679271347820758819580,  0.659771146748583614538574693142436444759368896484}, // 0
    { 0.368223619982562921926216858992120251059532165527,  0.592964429003537407680823889677412807941436767578}, // 1
    {-1.942948951698288073686171628651209175586700439453,  0.000000000000000027818939071799278620520511283696}, // 2
    {-0.027116964259834751282962628238237812183797359467,  0.808631325719216142999812291236594319343566894531}, // 3
    { 0.250642464452859869972201067866990342736244201660, -0.000023159465316861627224177128114845913842145819}, // 4
    {-0.747957199489194812791481581371044740080833435059,  0.075173326380023136605856848291296046227216720581}, // 5
    {-1.942550688048265028484706817835103720426559448242,  0.000000441805744265319442252526547371127563224036}, // 6
};

// Расчет фрактала
int mandel(double cx, double cy) {

    double x = 0, y = 0;
    for (int i = 0; i < 256; i++) {

        double tx = x*x - y*y + cx,
               ty = 2*x*y     + cy;

        x = tx;
        y = ty;

        if (x*x + y*y >= 4) return i;
    }

    return 255;
}

int main(int argc, char* argv[]) {

    screen(13);

    // Палитра
    for (int i = 2; i < 256; i++) palette(i, i, i, 128 + (i>>1));
    palette(0, 0, 0,   0);
    palette(1, 0, 255, 0);
    qb_scroll_x = 0;

    int     ip = 0; // CHECKPOINT_NUMBER - 1;
    double  x0 = checkpoint[ip][0],
            y0 = checkpoint[ip][1],
            f  = ip ? 0.00000000000001 : 1;
    double  dx = 0, dy = 0;
    int     mx = -1, my = -1, st = -1;

    // Скорость приближения
    float   speed = 0,
            speed_prev = 0;
    int     checkpoint_id = 0;
    int     iter   = 0;
    int     record = 1;

    // Черная подложка
    linebf(0, 192, 319, 199, 0);

    load_running_string("ticker.txt");

    while (sdlevent(EVT_REDRAW)) {

        // Срабатывает событие
        if (mouse.st == st && record == 0)
            continue;

        mx = mouse.x;
        my = mouse.y;

        double xm = (float)(mx - 160) / 100.0;
        double ym = (float)(100 - my) / 100.0;

        // Режим записи видео
        if (record) {

            // Если дно не было достигнуто, постепенно увеличивать скорость
            if (f > 0.00000000000015)
                 speed += 0.01;
            // Достигнув дна, начинать разгон в обратную сторону с бОльшей скоростью для буста
            else speed -= 0.06;

            // Точка перегиба из отдаления к приближению
            if (speed_prev < 0 && speed > 0) {

                checkpoint_id++;

                // Следующий чекпоинт
                if (checkpoint_id < CHECKPOINT_NUMBER) {

                    iter = 100;
                    dx   = (checkpoint[ checkpoint_id ][0] - x0) / (double)iter;
                    dy   = (checkpoint[ checkpoint_id ][1] - y0) / (double)iter;;
                }
                else {
                    record = 0;
                }
            }

            // Линейное движение к другому чекпоинту
            if (iter) {

                x0 += dx;
                y0 += dy;

                // Достигло конечного значения
                if (--iter == 0) {
                    x0 = checkpoint[ checkpoint_id ][0];
                    y0 = checkpoint[ checkpoint_id ][1];
                }
            }

            f *= ((100.0 - speed)/100.0);
            speed_prev = speed;
        }
        // Приближение
        else if ((mouse.st & LF_CLICK) && !(st & LF_CLICK)) {

            x0  += f*xm;
            y0  += f*ym;
            f   /= 2.0;

            printf("{%.48f, %.48f}\n", x0, y0);
        }
        // Отдаление
        else if ((mouse.st & RT_CLICK) && !(st & RT_CLICK)) {
            f *= 2.0;
        }

        // Перерисовать фрактал
        for (int y = -91;  y <= 100; y++)
        for (int x = -160; x <  160; x++) {

            double vx = x0 + f*(double)x / 100.0;
            double vy = y0 + f*(double)y / 100.0;

            int cl = mandel(vx, vy);
            pset(160 + x, 100 - y, cl < 2 ? 2 : cl);
        }

        st = mouse.st;

        // HUD
        line(160, 98, 160, 102, 128);
        line(158, 100, 162, 100, 128);

        // Бегущая строка
        color(255, 0); step_runstr(24);

        // -------------------------------------------------------------
        if (record) record2ppm("record");

        // Отладочный
        color(1, -1); locate (0, 0); print("#"); print(itoa(checkpoint_id));
        color(1, -1); locate (0, 1); print(itoa(rec_frame_id));
    }

    return 0;
}
