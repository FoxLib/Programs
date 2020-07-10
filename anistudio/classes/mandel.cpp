class mandel {

public:

    studio* app;
    image   img;
    int     max_iter;           // Максимальное кол-во итерации (360)

    mandel(studio* _app) {

        app = _app;
        max_iter = 4*360;
        img.create(1920, 1080);
    }

    // -----------------------------------------------------------------
    // Zi^2+C
    // (a + bi)(a + bi) = a^2 + 2*a*b*i - b^2
    // a_ = a^2 - b^2
    // b  = 2*a*b
    // -----------------------------------------------------------------

    int bailout(double Ca, double Cb) {

        double
            a_, b_,
            a = 0.0, b = 0.0;

        for (int i = 0; i < max_iter; i++) {

            a_ = Ca + a*a - b*b;
            b_ = Cb + 2*a*b;
            a  = a_;
            b  = b_;

            if (a*a + b*b > 4.0)
                return i;
        }

        return -1;
    }

    // Перерисовка экрана
    void repaint(double tx, double ty, double scale) {

        int x, y;

        for (y = -540; y < 540; y++)
        for (x = -960; x < 960; x++) {

            // Нормализованное значение
            double nx = scale * ((double)x / 540.0) + tx;
            double ny = scale * ((double)y / 540.0) + ty;

            // Получение цвета
            int color = bailout(nx, ny);

            // Проверка на то, какой цвет пришел
            if (color < 0)
                color = BLACK;
            else
                color = img.hsv2rgb(180 + color, 100, 255);

            img.pset(960 + x, 540 + y, color);
        }
    }

    // Билдинг
    // x = -0.552177333525
    // y = -0.648264229945
    // desired = 0.0000001
    void build(int start, int end, int cnt, float startx, float starty, float desired) {

        jpeg  jpg(1920, 1080);

        // Куда будет приближение идти
        float endx   = startx,
              endy   = starty,
              factor = log(desired) / log(2.0);

        // Сгенерировать фреймы
        for (int i = start; i <= end; i++) {

            float k     = (float)i / (float)cnt;
            float scale = pow(2.0, factor * k);     // Квадратная интерполяция
            float x     = startx*(1 - k) + endx*k;  // Линейная интерполяция
            float y     = starty*(1 - k) + endy*k;

            repaint(x, y, scale);   // Перерисовка mandel

            app->savejpeg(&img, i); // Выгрузка в jpeg
            app->update(&img);
        }
    }
};
