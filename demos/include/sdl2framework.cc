#include <iostream>

#include <time.h>
#include <sys/timeb.h>
#include <memory.h>
#include <fcntl.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "sdl2framework.h"

class SDL2framework {

protected:

    SDL_Surface* surface;
    SDL_Window*  window;
    SDL_Event    event;
    int previous_timer;
    int timer_loop;
    struct timeb clock_timer;
    int access_key_dn, access_key_up;
    int event_cause;
    int useindex;
    int rec_frame_id;
    int width, height, scale;
    int color_fore, color_back, font_height;

public:

    // Конструктор окна
    SDL2framework(int w, int h) {

        useindex = 0;
        create("SDL2 Application", w, h);
    }

    // Видеорежим
    SDL2framework(int mode) {

        scale = 1;
        useindex = 0;
        int w = 160, h = 120;

        switch (mode) {

            // 640x400x16
            case  3: w = 1280; h = 800; scale = 2; useindex = 15; break;
            // 640x360x16
            case 11: w = 1280; h = 720; scale = 2; useindex = 15; break;
            // 640x480x16
            case 12: w = 1280; h = 960; scale = 2; useindex = 15; break;
            // 320x200x256
            case 13: w = 960; h = 600; scale = 3; useindex = 255; break;
            // 320x180x256
            case 14: w = 960; h = 540; scale = 3; useindex = 255; break;
            default:

                exit(3);
        }

        create("SDL2 Application Index Color", w, h);
    }

    ~SDL2framework() {

        SDL_FreeSurface( surface );
        SDL_DestroyWindow( window );
        SDL_Quit();
    }

    // Рисуемые величины width x height
    int w()  { return width / scale; }
    int h()  { return height / scale; }

    // Создание окна SDL, инициализация
    void create(const char* title, int w, int h) {

        if (SDL_Init(SDL_INIT_EVERYTHING))
            exit(1);

        // Создать окно
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
        if (window == NULL)
            exit(2);

        // Получение данных о поверхности
        surface = SDL_GetWindowSurface(window);

        // 50 кадров в секунду
        timer_loop      = 20;
        previous_timer  = 0;
        rec_frame_id    = 0;
        color_fore      = 15;
        color_back      = 0;
        font_height     = 16;

        width  = w;
        height = h;
    }

    // Нажатая клавиша
    int sdlkey(SDL_Event event) {

        /* Получение ссылки на структуру с данными о нажатой клавише */
        SDL_KeyboardEvent * eventkey = &event.key;

        /* Получить скан-код клавиш */
        return eventkey->keysym.scancode;
    }

    // Получение ASCII
    int get_key_ascii(SDL_Event event) {

        /* Получение ссылки на структуру с данными о нажатой клавише */
        SDL_KeyboardEvent * eventkey = & event.key;

        int xt = 0;
        int k = eventkey->keysym.scancode;

        switch (k) {

            /* A */ case 0x26: xt = 0x41; break;
            /* B */ case 0x38: xt = 0x42; break;
            /* C */ case 0x36: xt = 0x43; break;
            /* D */ case 0x28: xt = 0x44; break;
            /* E */ case 0x1a: xt = 0x45; break;
            /* F */ case 0x29: xt = 0x46; break;
            /* G */ case 0x2a: xt = 0x47; break;
            /* H */ case 0x2b: xt = 0x48; break;
            /* I */ case 0x1f: xt = 0x49; break;
            /* J */ case 0x2c: xt = 0x4A; break;
            /* K */ case 0x2d: xt = 0x4B; break;
            /* L */ case 0x2e: xt = 0x4C; break;
            /* M */ case 0x3a: xt = 0x4D; break;
            /* N */ case 0x39: xt = 0x4E; break;
            /* O */ case 0x20: xt = 0x4F; break;
            /* P */ case 0x21: xt = 0x50; break;
            /* Q */ case 0x18: xt = 0x51; break;
            /* R */ case 0x1b: xt = 0x52; break;
            /* S */ case 0x27: xt = 0x53; break;
            /* T */ case 0x1c: xt = 0x54; break;
            /* U */ case 0x1e: xt = 0x55; break;
            /* V */ case 0x37: xt = 0x56; break;
            /* W */ case 0x19: xt = 0x57; break;
            /* X */ case 0x35: xt = 0x58; break;
            /* Y */ case 0x1d: xt = 0x59; break;
            /* Z */ case 0x34: xt = 0x5A; break;

            /* 0 */ case 0x13: xt = 0x30; break;
            /* 1 */ case 0x0A: xt = 0x31; break;
            /* 2 */ case 0x0B: xt = 0x32; break;
            /* 3 */ case 0x0C: xt = 0x33; break;
            /* 4 */ case 0x0D: xt = 0x34; break;
            /* 5 */ case 0x0E: xt = 0x35; break;
            /* 6 */ case 0x0F: xt = 0x36; break;
            /* 7 */ case 0x10: xt = 0x37; break;
            /* 8 */ case 0x11: xt = 0x38; break;
            /* 9 */ case 0x12: xt = 0x39; break;

            /* ` */ case 0x31: xt = 0x60; break;
            /* - */ case 0x14: xt = 0x2D; break;
            /* = */ case 0x15: xt = 0x3D; break;
            /* \ */ case 0x33: xt = 0x5C; break;
            /* [ */ case 0x22: xt = 0x5B; break;
            /* ] */ case 0x23: xt = 0x5D; break;
            /* ; */ case 0x2f: xt = 0x3B; break;
            /* ' */ case 0x30: xt = 0x27; break;
            /* , */ case 0x3b: xt = 0x2C; break;
            /* . */ case 0x3c: xt = 0x2E; break;
            /* / */ case 0x3d: xt = 0x2F; break;

            /* F1  */ case 67: xt = key_F1; break;
            /* F2  */ case 68: xt = key_F2; break;
            /* F3  */ case 69: xt = key_F3; break;
            /* F4  */ case 70: xt = key_F4; break;
            /* F5  */ case 71: xt = key_F5; break;
            /* F6  */ case 72: xt = key_F6; break;
            /* F7  */ case 73: xt = key_F7; break;
            /* F8  */ case 74: xt = key_F8; break;
            /* F9  */ case 75: xt = key_F9; break;
            /* F10 */ case 76: xt = key_F10; break;
            /* F11 */ case 95: xt = key_F11; break;
            /* F12 */ case 96: xt = key_F12; break;

            /* bs */ case 0x16: xt = key_BS; break;     // Back Space
            /* sp */ case 0x41: xt = 0x20; break;       // Space
            /* tb */ case 0x17: xt = key_TAB; break;    // Tab
            /* ls */ case 0x32: xt = key_LSHIFT; break; // Left Shift
            /* lc */ case 0x25: xt = key_LALT;  break;  // Left Ctrl
            /* la */ case 0x40: xt = key_LCTRL; break;  // Left Alt
            /* en */ case 0x24: xt = key_ENTER; break;  // Enter
            /* es */ case 0x09: xt = key_ESC; break;    // Escape
            /* es */ case 0x08: xt = key_ESC; break;

            // ---------------------------------------------
            // Специальные (не так же как в реальном железе)
            // ---------------------------------------------

            /* UP  */  case 0x6F: xt = key_UP; break;
            /* RT  */  case 0x72: xt = key_RT; break;
            /* DN  */  case 0x74: xt = key_DN; break;
            /* LF  */  case 0x71: xt = key_LF; break;
            /* Home */ case 0x6E: xt = key_HOME; break;
            /* End  */ case 0x73: xt = key_END; break;
            /* PgUp */ case 0x70: xt = key_PGUP; break;
            /* PgDn */ case 0x75: xt = key_PGDN; break;
            /* Del  */ case 0x77: xt = key_DEL; break;
            /* Ins  */ case 0x76: xt = key_INS; break;
            /* NLock*/ case 0x4D: xt = key_NL; break;

            default: return -k;
        }

        /* Получить скан-код клавиш */
        return xt;
    }

    // Опрос событий
    int poll() {

        event_cause = 0;

        while (!event_cause) {

            // Опрос событий
            while (SDL_PollEvent(&event) != 0)
            {
                switch (event.type) {

                    case SDL_QUIT:
                        return 0;

                    case SDL_KEYDOWN:

                        event_cause |= EventKeyDown;
                        access_key_dn = get_key_ascii(event);
                        break;

                    case SDL_KEYUP:

                        event_cause |= EventKeyUp;
                        access_key_up = get_key_ascii(event) | 0x80;
                        break;
/*
                    case SDL_MOUSEMOTION:

                        mouse.x = event.motion.x / qb_screen_factor;
                        mouse.y = event.motion.y / qb_screen_factor;
                        sdl_event_trigger |= EVT_MOUSE_MOVE;
                        break;

                    case SDL_MOUSEBUTTONDOWN:

                        mouse.x = event.motion.x / qb_screen_factor;
                        mouse.y = event.motion.y / qb_screen_factor;

                        switch (event.motion.state) {
                            case SDL_BUTTON_LEFT:  mouse.st |= LF_CLICK; break;
                            case SDL_BUTTON_RIGHT: mouse.st |= RT_CLICK; break;
                        }

                        sdl_event_trigger |= EVT_MOUSE_DOWN;
                        break;

                    case SDL_MOUSEBUTTONUP:

                        mouse.x = event.motion.x / qb_screen_factor;
                        mouse.y = event.motion.y / qb_screen_factor;

                        switch (event.motion.state) {
                            case SDL_BUTTON_LEFT:  mouse.st &= ~LF_CLICK; break;
                            case SDL_BUTTON_RIGHT: mouse.st &= ~RT_CLICK; break;
                        }

                        sdl_event_trigger |= EVT_MOUSE_UP;
                        break;
*/
                    default:
                        break;

                }
            }

            // Остановка на перерисовку и ожидание
            ftime(&clock_timer);

            // Вычисление разности времени
            int time_curr = clock_timer.millitm;
            int time_diff = time_curr - previous_timer;
            if (time_diff < 0) time_diff += 1000;

            // Если прошло 20 мс, выполнить инструкции, обновить экран
            if (time_diff >= timer_loop || time_diff < 0) {

                previous_timer = time_curr;

                // Обновление экрана, если есть что обновлять
                SDL_UpdateWindowSurface(window);

                event_cause |= EventUpdateScreen;
            }

            SDL_Delay(1);
        }

        return event_cause;
    }

    // Запись пикселя RAW
    void pixel(int x, int y, int cl) {

        if (x >= 0 && x < width && y >= 0 && y < height) {
            ( (Uint32*)surface->pixels )[ y*width + x ] = cl;
        }
    }

    // Цветовая палитра
    int getcolor(int cl) {

        if (cl < 0) return -cl;
        return useindex ? SDL2FrameworkQbPalette[cl & useindex] : cl;
    }

    // -----------------------------------------------------------------

    void cls(int cl) {

        for (int i = 0; i < width*height; i++)
            ( (Uint32*)surface->pixels )[i] = cl;
    }

    // Цвет точки (положительное число)
    int point(int x, int y) {

        if (x >= 0 && x < width && y >= 0 && y < height) {
            return ( (Uint32*)surface->pixels )[ scale*y*width + scale*x ];
        }

        return 0;
    }

    // В зависимости от видеорежима
    void pset(int x, int y, int cl) {

        if (scale == 1) {

            pixel(x, y, getcolor(cl));

        } else {

            for (int i = 0; i < scale; i++)
            for (int j = 0; j < scale; j++)
                pixel(scale*x + j, scale*y + i, getcolor(cl));
        }
    }

    // Рисование линии по алгоритму Брезенхема
    // https://ru.wikipedia.org/wiki/Алгоритм_Брезенхэма
    void line(int x1, int y1, int x2, int y2, int color) {

        // Инициализация смещений
        int signx  = x1 < x2 ? 1 : -1;
        int signy  = y1 < y2 ? 1 : -1;
        int deltax = x2 > x1 ? x2 - x1 : x1 - x2;
        int deltay = y2 > y1 ? y2 - y1 : y1 - y2;
        int error  = deltax - deltay;
        int error2;

        // Если линия - это точка
        pset(x2, y2, color);

        // Перебирать до конца
        while ((x1 != x2) || (y1 != y2)) {

            pset(x1, y1, color);
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
    }

    // Рисование прямоугольника (Border)
    void lineb(int x1, int y1, int x2, int y2, int color) {

        line(x1, y1, x2, y1, color);
        line(x1, y1, x1, y2, color);
        line(x2, y1, x2, y2, color);
        line(x1, y2, x2, y2, color);
    }

    // Закрашенный прямоугольник BF (Border Fill)
    void linebf(int x1, int y1, int x2, int y2, int color) {

        int _y1 = y1 <= y2 ? y1 : y2;
        int _y2 = y2  > y1 ? y2 : y1;
        for (int i = _y1; i <= _y2; i++) {
            line(x1, i, x2, i, color);
        }
    }

    // Рисование окружности
    void circle(int xc, int yc, int radius, int color) {

        int x = 0,
            y = radius,
            d = 3 - 2*y;

        while (x <= y) {

            // Верхний и нижний сектор
            pset(xc - x, yc + y, color);
            pset(xc + x, yc + y, color);
            pset(xc - x, yc - y, color);
            pset(xc + x, yc - y, color);

            // Левый и правый сектор
            pset(xc - y, yc + x, color);
            pset(xc + y, yc + x, color);
            pset(xc - y, yc - x, color);
            pset(xc + y, yc - x, color);

            d += (4*x + 6);
            if (d >= 0) {
                d += 4*(1 - y);
                y--;
            }

            x++;
        }
    }

    // Рисование закрашенной окружности (Fill)
    void circlef(int xc, int yc, int radius, int color) {

        int x = 0,
            y = radius,
            d = 3 - 2*y;

        while (x <= y) {

            // Верхний и нижний сектор
            line(xc - x, yc + y, xc + x, yc + y, color);
            line(xc - x, yc - y, xc + x, yc - y, color);

            // Левый и правый сектор
            line(xc - y, yc + x, xc + y, yc + x, color);
            line(xc - y, yc - x, xc + y, yc - x, color);

            d += (4*x + 6);
            if (d >= 0) {
                d += 4*(1 - y);
                y--;
            }

            x++;
        }
    }

    // Нарисовать на экране символ
    void print_char(int x, int y, unsigned char ch) {

        for (int i = 0; i < font_height; i++)
        for (int j = 0; j < 8; j++) {

            int bt = (font_height == 16 ? QB_FONT_8x16[ch][i] : QB_FONT_8x8[ch][i]) & (1 << (7 ^ j));
            int cl = bt ? color_fore : color_back;
            if (cl >= 0) pset(8*x + j, 8*y + i, cl);
        }
    }

    // Математика
    // -----------------------------------------------------------------

    // Арктангенс
    float atn(float x, float y) { return (atan2(y, x) + M_PI) / (2*M_PI); }

    // Псевдослучайное число
    float rnd(float x, float y) {

        float m = sin(x*12.9898 + y*78.233) * 43758.545312;
        return m - floor(m);
    }

    float noise(float x, float y) {

        int   ix = floor(x), iy = floor(y);
        float fx = x - ix, fy = y - iy;

        float
            a = rnd(ix,     iy),
            b = rnd(ix + 1, iy),
            c = rnd(ix,     iy + 1),
            d = rnd(ix + 1, iy + 1);

        float
            ux = fx*fx*(3 - 2*fx),
            uy = fy*fy*(3 - 2*fy);

        return a*(1-ux) + b*ux + (c-a)*uy*(1-ux) + (d-b)*ux*uy;
    }

    float fbm(float x, float y) {

        float val = 0, amp = 0.5;

        for (int i = 0; i <= 5; i++) {

            val += amp*noise(x, y);
            x *= 2;
            y *= 2;
            amp *= 0.5;
        }

        return val;
    }

    // -----------------------------------------------------------------

    // Наличие нажатой клавиши
    int key_down()  { return (event_cause & EventKeyDown) ? access_key_dn : 0; }
    int key_up()    { return (event_cause & EventKeyUp)   ? access_key_up : 0; }
    int timer()     { return (event_cause & EventUpdateScreen); }

    // -----------------------------------------------------------------
    void saveppm() { saveppm(""); }
    void saveppm(const char* name) {

        unsigned char tmp[256];
        uint cl;
        int w = width / scale,
            h = height / scale;

        int is_stdout = (strlen(name) == 0);
        FILE* fp = is_stdout ? stdout : fopen(name, "w+");

        sprintf((char*)tmp, "P6\n%d %d\n255\n", w, h);
        fputs((char*)tmp, fp);

        for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {

            cl = ( (Uint32*)surface->pixels )[ (scale*y)*width + (scale*x) ];

            tmp[0] = cl >> 16;
            tmp[1] = cl >> 8;
            tmp[2] = cl;

            fwrite(tmp, 1, 3, fp);
        }

        if (is_stdout == 0) fclose(fp);
    }

    // Запись экрана в PPM-вывод
    int record(int argc, char* argv[]) { return record(argc, argv, 0); }
    int record(int argc, char* argv[], int maxframe) {

        if (argc > 1 && strcmp(argv[1], "rec") == 0) {

            rec_frame_id++;
            if ((rec_frame_id && rec_frame_id <= maxframe) || (maxframe == 0)) {
                saveppm();
            } else if (maxframe) {
                return 1;
            }
        }

        return 0;
    }
};
