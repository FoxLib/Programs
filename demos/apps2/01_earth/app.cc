#define SELFNAME "app.cc"

class App {
protected:

    SDL2framework* win;
    struct vec3 org;
    float timer;

    char  textbuf[960][480];
    int   loc_x, loc_y;
    int   filesize, loc_file;

public:

    App(SDL2framework* _win) {

        win = _win;
        timer = 0;

        // Очистка области рисования
        loc_x = loc_y = loc_file = 0;
        for (int y = 0; y < 480; y++)
        for (int x = 0; x < 480; x++)
            textbuf[x][y] = 0;

        // Чтение файла
        FILE* fp = fopen(SELFNAME, "rb");
        fseek(fp, 0, SEEK_END);
        filesize = ftell(fp);
        fclose(fp);
    }

    // Чтение следующего символа из файла
    int readnext() {

        unsigned char buf[2];

        if (loc_file >= filesize)
            return 0;

        // Чтение следующего символа
        FILE* fp = fopen(SELFNAME, "rb");
        fseek(fp, loc_file++, SEEK_SET);
        (void) fread(buf, 1, 2, fp);
        fclose(fp);

        int ch = buf[0];

        // Прописные русские буквы
        if (ch == 0xD0) {

            if (buf[1] == 0x01) buf[0] = 0xA5;
            else if (buf[1] >= 0x90 && buf[1] < 0xC0) ch = buf[1] - 0x10;
            else if (buf[1] >= 0xB0 && buf[1] < 0xC0) ch = buf[1] - 0x10;
            loc_file++;
        }
        // Строчные русские буквы
        else if (ch == 0xD1) {

            if (buf[1] == 0x91) ch = 0x85;
            else if (buf[1] >= 0x80) ch = buf[1] + 0x60;
            loc_file++;
        }

        return ch;
    }

    int gray(int x) {

        x = x < 0 ? 0 : (x > 255 ? 255 : x);
        return (x + x*256 + x*65536);
    }

    // Поиск точки соприкосновения со сферой
    float sphere(struct vec3 d, struct vec3 o, float r) {

        float t = -1;

        float a = d.x*d.x + d.y*d.y + d.z*d.z;
        float b = -2*(d.x*o.x + d.y*o.y + d.z*o.z);
        float c = o.x*o.x + o.y*o.y + o.z*o.z - r*r;
        float D = b*b - 4*a*c;

        if (D >= 0) {

            D = sqrt(D);
            float x1 = (-b - D) / (2*a),
                  x2 = (-b + D) / (2*a);

            if (x1 > 0 && x2 < 0) t = x1;
            if (x1 < 0 && x2 > 0) t = x2;
            if (x1 > 0 && x2 > 0) t = x1 < x2 ? x1 : x2;
        }

        return t;
    }

    // Нормализация вектора
    struct vec3 normalize(struct vec3 n) {

        float l = 1 / sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
        return {n.x*l, n.y*l, n.z*l};
    }

    // Пропечатать символ
    void printchr(unsigned char ch, int fore) {

        int i, j;

        if (ch == 0)
            return;

        // Печатать все, кроме символа переноса строки
        if (ch != 10) {

            for (i = 0; i < 16; i++)
            for (j = 0; j < 8; j++) {

                int bt = QB_FONT_8x16[ch][i] & (1 << (7 ^ j));
                int cl = bt ? fore : 0;
                if (cl >= 0) textbuf[8*loc_x+j][16*loc_y+i] = cl;
            }

            loc_x++;
        }

        // Перевод каретки
        if (8*(loc_x + 1) > win->w() || ch == 10) {

            loc_y++;
            loc_x = 0;

            // Перенос строки
            if (16*(loc_y + 1) > win->h()) {

                for (i = 0; i < win->h() - 16; i++)
                for (j = 0; j < win->w(); j++)
                    textbuf[j][i] = textbuf[j][i+16];

                for (i = win->h() - 16; i < win->h(); i++)
                for (j = 0; j < win->w(); j++)
                    textbuf[j][i] = 0;

                loc_y--;
            }
        }
    }

    int update() {

        struct vec3 c, cam, cl, sun, sun_n, lno, ref;
        float m, u, v, cloud, refl, lspe, dl;
        float radius = 1;
        int nextsym = 1;

        int width   = win->w(),
            height  = win->h();
        int width2  = width >> 1,
            height2 = height>>1;

        org    = {0, 0, 1.5};
        radius = 1 + 0.25*sin(timer);
        sun    = {org.x + 4*sin(timer), 1, org.z - 4*cos(timer)};
        sun_n  = normalize({sun.x - org.x, sun.y - org.y, sun.z - org.z});

        win->cls(0);

        // Случайные звезды
        for (int i = 0; i < 512; i++)
            win->pset(width * win->rnd(i,0), height * win->rnd(0,i), -gray(255*win->rnd(i, i)));

        // Основной цикл обработки
        for (int y = -height2; y <= height2; y++)
        for (int x = -width2; x < width2; x++) {

            cl = {x: 0, y: 0, z: 0};

            // Поиск точки пересения луча со сферой
            c  = {x: (float)x / height2, y: (float)y / height2, z: 1.0};
            m  = sphere(c, org, radius);

            if (m > 0) {

                cam = {x: c.x*m, y: c.y*m, z: c.z*m};

                // Вычисление нормали
                c = normalize({
                    x: cam.x - org.x,
                    y: cam.y - org.y,
                    z: cam.z - org.z
                });

                // Вычисление текстуры
                u = win->atn(c.x, c.z) + timer;
                v = win->atn(c.y, c.z);

                u = u - floor(u);
                v = v - floor(v);

                // Directional light
                dl = c.x*sun_n.x + c.y*sun_n.y + c.z*sun_n.z;
                if (dl < 0) dl = 0;

                // Текстуры
                m     = win->fbm(32*u, 32*v) * 255;

                // Облака с учетом directional light
                cloud = dl * (float) trunc(pow(win->fbm(64.0*u, 64*v), 2) * 255);

                if (m < 132) {

                    cl = {0, 0, 192};

                    lno = normalize({sun.x - cam.x, sun.y - cam.y, sun.z - cam.z});
                    cam = normalize({-cam.x, -cam.y, -cam.z});

                    // Вычисление коэффициента отраженного света lspe
                    refl = 2*(cam.x*c.x + cam.y*c.y + cam.z*c.z);
                    ref  = normalize({c.x*refl - cam.x, c.y*refl - cam.y, c.z*refl - cam.z});
                    lspe = ref.x*lno.x + ref.y*lno.y + ref.z*lno.z;
                    lspe = lspe < 0 ? 0 : pow(lspe, 11);

                    // Умножение на Directional Light
                    cl = {cl.x*dl + 255*lspe, cl.y*dl + 255*lspe, cl.z*dl + 255*lspe};
                }
                else if (m > 138) {

                    m  = 255*((m-138)/117);
                    cl = {m*dl, (64 + m)*dl, m*dl};
                }

                // Добавление облаков
                if (cloud > 32) {

                    cl.x += 2*cloud;
                    cl.y += 2*cloud;
                    cl.z += 2*cloud;
                }

                // Clamp
                cl.x = (int)(cl.x > 255 ? 255 : (cl.x < 0 ? 0 : cl.x));
                cl.y = (int)(cl.y > 255 ? 255 : (cl.y < 0 ? 0 : cl.y));
                cl.z = (int)(cl.z > 255 ? 255 : (cl.z < 0 ? 0 : cl.z));

                win->pset(width/2 + x, height/2 - y, -(cl.x*65536 + cl.y*256 + cl.z));
            }
        }

        // Следующий символ
        if (rand()%3 == 1) {
            nextsym = readnext();
            printchr(nextsym, 15);
        }

        // Наложение текста
        for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {

            int cl = win->point(x, y);
            int tx = SDL2FrameworkQbPalette[ (int)textbuf[x][y] ];

            int r, g, b;
            unsigned char r0 = cl>>16, g0 = cl >> 8, b0 = cl;
            unsigned char r1 = tx>>16, g1 = tx >> 8, b1 = tx;

            r = (r0*3/4) + (r1>>0); if (r > 255) r = 255;
            g = (g0*3/4) + (g1>>0); if (g > 255) g = 255;
            b = (b0*3/4) + (b1>>0); if (b > 255) b = 255;

            win->pset(x, y, -(r*65536 + g*256 + b));
        }

        // Курсор
        for (int y = 0; y < 2; y++)
        for (int x = 0; x < 8; x++)
            win->pset(loc_x*8+x, (loc_y+1)*16-1-y, 10);

        timer += 0.00025;

        return nextsym;
    }
};
