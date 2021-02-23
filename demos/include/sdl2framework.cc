#include <iostream>

#include <time.h>
#include <sys/timeb.h>
#include <memory.h>
#include <fcntl.h>
#include <SDL2/SDL.h>

enum SDL2FrameworkEvents {

    EventUpdateScreen   = 1,
    EventKeyDown        = 2,
    EventKeyUp          = 4,
};

enum SDL2FrameworkKeyASCII {

    key_LSHIFT      = 0x01,
    key_LALT        = 0x02,
    key_LCTRL       = 0x03,
    key_UP          = 0x04,
    key_DN          = 0x05,
    key_LF          = 0x06,
    key_RT          = 0x07,
    key_BS          = 0x08,
    key_TAB         = 0x09,
    key_ENTER       = 0x0A,
    key_HOME        = 0x0B,
    key_END         = 0x0C,
    key_PGUP        = 0x0D,
    key_PGDN        = 0x0E,
    key_DEL         = 0x0F,
    key_F1          = 0x10,
    key_F2          = 0x11,
    key_F3          = 0x12,
    key_F4          = 0x13,
    key_F5          = 0x14,
    key_F6          = 0x15,
    key_F7          = 0x16,
    key_F8          = 0x17,
    key_F9          = 0x18,
    key_F10         = 0x19,
    key_F11         = 0x1A,
    key_ESC         = 0x1B,
    key_INS         = 0x1C,
    key_NL          = 0x1D,
    key_F12         = 0x1E,
    key_SPECIAL     = 0x1F          // Особая клавиша
};

static const int SDL2FrameworkQbPalette[256] = {
  0x000000, 0x0000aa, 0x00aa00, 0x00aaaa, 0xaa0000, 0xaa00aa, 0xaa5500, 0xaaaaaa, // 0
  0x555555, 0x5555ff, 0x55ff55, 0x55ffff, 0xff5555, 0xff55ff, 0xffff55, 0xffffff, // 8
  0x000000, 0x141414, 0x202020, 0x2c2c2c, 0x383838, 0x454545, 0x515151, 0x616161, // 10
  0x717171, 0x828282, 0x929292, 0xa2a2a2, 0xb6b6b6, 0xcbcbcb, 0xe3e3e3, 0xffffff, // 18
  0x0000ff, 0x4100ff, 0x7d00ff, 0xbe00ff, 0xff00ff, 0xff00be, 0xff007d, 0xff0041, // 20
  0xff0000, 0xff4100, 0xff7d00, 0xffbe00, 0xffff00, 0xbeff00, 0x7dff00, 0x41ff00, // 28
  0x00ff00, 0x00ff41, 0x00ff7d, 0x00ffbe, 0x00ffff, 0x00beff, 0x007dff, 0x0041ff, // 30
  0x7d7dff, 0x9e7dff, 0xbe7dff, 0xdf7dff, 0xff7dff, 0xff7ddf, 0xff7dbe, 0xff7d9e, // 38
  0xff7d7d, 0xff9e7d, 0xffbe7d, 0xffdf7d, 0xffff7d, 0xdfff7d, 0xbeff7d, 0x9eff7d, // 40
  0x7dff7d, 0x7dff9e, 0x7dffbe, 0x7dffdf, 0x7dffff, 0x7ddfff, 0x7dbeff, 0x7d9eff, // 48
  0xb6b6ff, 0xc7b6ff, 0xdbb6ff, 0xebb6ff, 0xffb6ff, 0xffb6eb, 0xffb6db, 0xffb6c7, // 50
  0xffb6b6, 0xffc7b6, 0xffdbb6, 0xffebb6, 0xffffb6, 0xebffb6, 0xdbffb6, 0xc7ffb6, // 58
  0xb6ffb6, 0xb6ffc7, 0xb6ffdb, 0xb6ffeb, 0xb6ffff, 0xb6ebff, 0xb6dbff, 0xb6c7ff, // 60
  0x000071, 0x1c0071, 0x380071, 0x550071, 0x710071, 0x710055, 0x710038, 0x71001c, // 68
  0x710000, 0x711c00, 0x713800, 0x715500, 0x717100, 0x557100, 0x387100, 0x1c7100, // 70
  0x007100, 0x00711c, 0x007138, 0x007155, 0x007171, 0x005571, 0x003871, 0x001c71, // 78
  0x383871, 0x453871, 0x553871, 0x613871, 0x713871, 0x713861, 0x713855, 0x713845, // 80
  0x713838, 0x714538, 0x715538, 0x716138, 0x717138, 0x617138, 0x557138, 0x457138, // 88
  0x387138, 0x387145, 0x387155, 0x387161, 0x387171, 0x386171, 0x385571, 0x384571, // 90
  0x515171, 0x595171, 0x615171, 0x695171, 0x715171, 0x715169, 0x715161, 0x715159, // 98
  0x715151, 0x715951, 0x716151, 0x716951, 0x717151, 0x697151, 0x617151, 0x597151, // A0
  0x517151, 0x517159, 0x517161, 0x517169, 0x517171, 0x516971, 0x516171, 0x515971, // A8
  0x000041, 0x100041, 0x200041, 0x300041, 0x410041, 0x410030, 0x410020, 0x410010, // B0
  0x410000, 0x411000, 0x412000, 0x413000, 0x414100, 0x304100, 0x204100, 0x104100, // B8
  0x004100, 0x004110, 0x004120, 0x004130, 0x004141, 0x003041, 0x002041, 0x001041, // C0
  0x202041, 0x282041, 0x302041, 0x382041, 0x412041, 0x412038, 0x412030, 0x412028, // C8
  0x412020, 0x412820, 0x413020, 0x413820, 0x414120, 0x384120, 0x304120, 0x284120, // D0
  0x204120, 0x204128, 0x204130, 0x204138, 0x204141, 0x203841, 0x203041, 0x202841, // D8
  0x2c2c41, 0x302c41, 0x342c41, 0x3c2c41, 0x412c41, 0x412c3c, 0x412c34, 0x412c30, // E0
  0x412c2c, 0x41302c, 0x41342c, 0x413c2c, 0x41412c, 0x3c412c, 0x34412c, 0x30412c, // E8
  0x2c412c, 0x2c4130, 0x2c4134, 0x2c413c, 0x2c4141, 0x2c3c41, 0x2c3441, 0x2c3041, // F0
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000  // F8
};

class SDL2framework {

protected:

    SDL_Surface* surface;
    SDL_Window*  window;
    SDL_Event    event;
    int width, height, scale;
    int previous_timer;
    int timer_loop;
    struct timeb clock_timer;
    int access_key_dn, access_key_up;
    int event_cause;
    int useindex;

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

            case 12: w = 640; h = 400; scale = 2; useindex = 15; break;
            case 13: w = 960; h = 600; scale = 3; useindex = 255; break;
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
        return useindex ? SDL2FrameworkQbPalette[cl & useindex] : cl;
    }

    // -----------------------------------------------------------------

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

    // -----------------------------------------------------------------

    // Наличие нажатой клавиши
    int key_down()  { return (event_cause & EventKeyDown) ? access_key_dn : 0; }
    int key_up()    { return (event_cause & EventKeyUp)   ? access_key_up : 0; }
    int timer()     { return (event_cause & EventUpdateScreen); }
};
