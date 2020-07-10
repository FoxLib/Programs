/**
 * Библиотека симуляции QBASIC программ
 */

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "qblib.h"

// Обновление экрана 60 Гц
uint qb_sdl_timer60(uint interval, void *param) {

    SDL_Event     event;
    SDL_UserEvent userevent;

    /* Создать новый Event */
    userevent.type  = SDL_USEREVENT;
    userevent.code  = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

// Основная функция инициализации
void main_init() {

    qb_fore_color = 15;
    qb_back_color = 0;
    qb_locate_x   = 0;
    qb_locate_y   = 0;
    qb_scroll_x   = 1;
    qb_scroll_y   = 1;
    qb_autoflip   = 1;

    mouse.x = 0;
    mouse.y = 0;
    mouse.st = 0;

    heap_location = 0;
    rec_frame_id  = 0;
    is_dither_palette = 0;
    qb_useindex   = 1;

    for (int i = 0; i < 256; i++) qb_palette[i] = QB_DOS_PALETTE_13[i];
}

void cls() {

    qb_locate_x = 0;
    qb_locate_y = 0;
    linebf(0, 0, qb_screen_width, qb_screen_height, qb_back_color);
}

// Инициализация окна
// 12 - 640x480x16
// 13 - 320x200x256
// 14 - 640x480x256
void screen(int mode) {

    int w, h;

    switch (mode) {

        case 12:
        case 14: w = 640; h = 480; qb_width = 80; qb_height = 30; qb_screen_factor = 2; qb_font_height = 16; break;
        case 13: w = 320; h = 200; qb_width = 40; qb_height = 25; qb_screen_factor = 4; qb_font_height = 8;  break;
        default: w = 640; h = 480; qb_width = 80; qb_height = 30; qb_screen_factor = 2; qb_font_height = 16; break;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    qb_screen_mode   = mode;
    qb_screen_width  = w;
    qb_screen_height = h;
    qb_sdl_screen    = SDL_SetVideoMode(
                        qb_screen_factor * w,
                        qb_screen_factor * h,
                        32,
                        SDL_HWSURFACE | SDL_DOUBLEBUF
                       );

    SDL_WM_SetCaption("QBASIC 4.5 C++ SDL1.2 Edition", 0);

    // Принудительное обновление экрана (flip) 50 Hz
    SDL_AddTimer(20, qb_sdl_timer60, NULL);

    main_init();
}

// Генерация палитры для дизеринга
void ditherpal() {

    is_dither_palette = 1;

    int i = 0;
    for (int r = 1; r <= 8; r++)
    for (int g = 1; g <= 8; g++)
    for (int b = 1; b <= 4; b++) {

        int _r = r == 1 ? 0 : (r<<5) - 1;
        int _g = g == 1 ? 0 : (g<<5) - 1;
        int _b = b == 1 ? 0 : (b<<6) - 1;

        palette(i++, _r, _g, _b);
    }
}

// Получение трансляции цвета
uint _get_color(int color) {

    // Использовать нативные цвета
    if (qb_useindex == 0)
        return color;

    // Выбор из палитры
    switch (qb_screen_mode) {

        case 12: color = qb_palette[color & 0x0f]; break;
        case 13: color = qb_palette[color & 0xff]; break;
        case 14: color = qb_palette[color & 0xff]; break;
    }

    return color;
}

// Номер точки
int point(int x, int y) {

    if (x >= 0 && x < qb_screen_width && y >= 0 && y < qb_screen_height) {
        return qb_screen[x][y];
    }
    return 0;
}

// Запись пикселя в RAW-массив для дизеринга
void pixel(int x, int y, int cl) {

    if (x >= 0 && x < QB_MAX_WIDTH && y >= 0 && y < QB_MAX_HEIGHT) {
        qb_pixels[x][y].r = (cl >> 16) & 0xff;  // R
        qb_pixels[x][y].g = (cl >> 8) & 0xff;   // G
        qb_pixels[x][y].b = (cl) & 0xff;        // B
    }
}

// Установка точки на экране
// Если число отрицательное, то записывается только в `qb_screen`
void pset(int x, int y, int cl) {

    int fc    = qb_screen_factor;
    int width = qb_screen_width;

    // Это нужно иногда для того, чтобы записать в backbuffer
    if (x < QB_MAX_WIDTH && y < QB_MAX_HEIGHT && x >= 0 && y >= 0) {

        // Сохранение полного цвета в любом случае
        if (qb_useindex) {
            qb_screen[x][y] = abs(cl);
            pixel(x, y, _get_color(qb_screen[x][y]));
        } else {
            pixel(x, y, cl);
        }
    }

    // Проверка границ
    if (x >= 0 && x < qb_screen_width && y >= 0 && y < qb_screen_height) {

        // Отрицательные точки не рисуются
        if (cl < 0) return;

        // Рисовать крупный пиксель
        for (int i = 0; i < fc*fc; i++) {

            int offset =  fc*(x + fc*width*y);
                offset += i % fc + fc*width*(i/fc);

            ( (Uint32*)qb_sdl_screen->pixels )[ offset ] = _get_color(cl);
        }
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

// Скроллинг экрана
void _scroll_xy(int hg) {

    int fc = qb_screen_factor;

    // Сдвиг виртуального курсора
    qb_locate_x++;
    if (qb_scroll_x && qb_locate_x >= qb_width) {

        qb_locate_x = 0;
        qb_locate_y++;

        if (qb_scroll_y && qb_locate_y >= qb_height) {

            int lx = fc*fc*qb_screen_width*(qb_screen_height - hg);
            int lz = fc*fc*qb_screen_width*(qb_screen_height);

            // Сдвинуть экран наверх на `hg` пикселей
            for (int j = 0; j < lx; j++) {

                ((Uint32*)qb_sdl_screen->pixels)[ j ] =
                ((Uint32*)qb_sdl_screen->pixels)[ j + hg*fc*fc*qb_screen_width ];
            }

            // Очистить нижнюю строку
            for (int j = lx; j < lz; j++) ((Uint32*)qb_sdl_screen->pixels)[ j ] = _get_color(qb_back_color);

            qb_locate_y = qb_height - 1;
        }
    }
}

// Нарисовать на экране символ
void _print_char(int x, int y, unsigned char ch) {

    // Печать 8x8 символов
    switch (qb_screen_mode) {

        case 13:

            for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {

                int bt = QB_FONT_8x8[ch][i] & (1 << (7 ^ j));
                int cl = bt ? qb_fore_color : qb_back_color;
                if (cl >= 0) pset(8*x + j, 8*y + i, cl);
            }

            _scroll_xy(8);
            break;

        case 12:
        case 14:

            for (int i = 0; i < 16; i++)
            for (int j = 0; j < 8; j++) {

                int bt = QB_FONT_8x16[ch][i] & (1 << (7 ^ j));
                int cl = bt ? qb_fore_color : qb_back_color;
                if (cl >= 0) pset(8*x + j, 16*y + i, cl);
            }

            _scroll_xy(16);
            break;
    }
}

// Установить курсор на экране
void locate(int x, int y) {

    qb_locate_x = x;
    qb_locate_y = y;
}

// Установка цвета
void color(int cl, int bg) {

    qb_fore_color = cl;
    qb_back_color = bg;
}

// Использовать ли индексные цвета
void useindex(int f) { qb_useindex = f; }

// Палитра
void palette(int cl, unsigned char r, unsigned char g, unsigned char b) {
    qb_palette[cl] = (r<<16) + (g<<8) + b;
}

// Напечатать символами
void print(const char* s) {

    int i = 0;
    unsigned char ch;

    while ((ch = s[i])) {

        // Прописные русские буквы
        if (ch == 0xD0) {

            ch = s[++i];
            if (ch == 0x01) ch = 0xA5;
            else if (ch >= 0x90 && ch < 0xC0) ch -= 0x10;
            else if (ch >= 0xB0 && ch < 0xC0) ch -= 0x10;
        }
        // Строчные русские буквы
        else if (ch == 0xD1) {

            ch = s[++i];
            if (ch == 0x91) ch = 0x85;
            else if (ch >= 0x80) ch += 0x60;
        }

        // Пропечатать символ
        _print_char(qb_locate_x, qb_locate_y, ch);

        i++;
    }
}

// Аллокатор новой строки во временном буфере
char* _allocstr(char* s) {

    int hp;
    int l = 0; while (s[++l]); // Длина строки вместе с \0

    // Отрезать строку
    if (l >= HEAP_STRING_SIZE) l = (HEAP_STRING_SIZE - 1);

    // Если в конец строки нельзя дописать ничего
    if (heap_location + l >= HEAP_STRING_SIZE) {
        heap_location = 0;
        hp = 0;
    } else {
        hp = heap_location;
    }

    for (int i = 0; i < l; i++)
        heap_tmpstring[heap_location + i] = s[i];

    // К следующей строке
    heap_location += l;

    // Вернуть указатель на новую строку
    return heap_tmpstring + hp;
}

// Вырезать часть строки UTF8
char* mid(const char* s, int start, int len) {

    int k = 0, i = 0;
    char tmp[2048]; tmp[0] = 0;

    // Отсчитать `start` символов от начала
    for (int j = 0; j < start; j++) {

        if (s[i] == 0) return _allocstr(tmp);
        if (s[i] & 0x80) i += 2; else i++;
    }

    // Отсчитать символы `len`
    for (int j = 0; j < len; j++) {

        if (s[i] == 0) break;
        if (s[i] & 0x80) {
            tmp[k++] = s[i++];
            tmp[k++] = s[i++];
        } else {
            tmp[k++] = s[i++];
        }
    }

    tmp[k] = 0;
    return _allocstr(tmp);
}

// Длина строки
int   len(const char* s) {

    int k = 0, i =0;
    while (s[i]) {

        if (s[i] & 0x80) i += 2; else i++;
        k++;
    }
    return k;
}

// Число в строку
char* ltoa(long int dig)  { char tmp[32]; sprintf(tmp, "%li", dig); return _allocstr(tmp); }
char* itoa(int dig)   { char tmp[32]; sprintf(tmp, "%d", dig); return _allocstr(tmp); }
char* ftoa(float dig) { char tmp[64]; sprintf(tmp, "%f", dig); return _allocstr(tmp); }
char* concat(char* a, char* b) {

    char tmp[HEAP_STRING_SIZE >> 1];
    strcpy(tmp, a);
    strcat(tmp, b);
    return _allocstr(tmp);
}

// Сгенерировать палитру. Формат:
// static const unsigned char pal[N][4] = { {ID, r, g, b}, ... };
void generate_palette(const int N, const unsigned char pal[][4]) {

    for (int i = 0; i < N - 1; i++) {

        int cl_sta = pal[i  ][0],
            cl_end = pal[i+1][0];
        int cl_dif = (cl_end - cl_sta + 1);

        for (int k = 0; k < cl_dif; k++) {

            float t = (float) k / (float) cl_dif;
            int   sta_r = pal[i][1],   sta_g = pal[i][2],   sta_b = pal[i][3];
            int   end_r = pal[i+1][1], end_g = pal[i+1][2], end_b = pal[i+1][3];

            // Линейная интерполяция
            float r = sta_r*(1 - t) + end_r*t;
            float g = sta_g*(1 - t) + end_g*t;
            float b = sta_b*(1 - t) + end_b*t;

            palette(cl_sta + k, r, g, b);
        }
    }
}

// =========================================== Запись видео ============

// Сохранение снимка экрана в PPM
void saveppm(const char* name) {

    unsigned char tmp[256];

    int is_stdout = (strlen(name) == 0);
    FILE* fp = is_stdout ? stdout : fopen(name, "w+");

    sprintf((char*)tmp, "P6\n%d %d\n255\n", qb_screen_width, qb_screen_height);
    fputs((char*)tmp, fp);

    for (int i = 0; i < qb_screen_height; i++)
    for (int j = 0; j < qb_screen_width; j++) {

        //int cl = _get_color(qb_screen[j][i]);
        //unsigned char r = cl >> 16, g = cl >> 8, b = cl;

        tmp[0] = qb_pixels[j][i].r;
        tmp[1] = qb_pixels[j][i].g;
        tmp[2] = qb_pixels[j][i].b;
        fwrite(tmp, 1, 3, fp);
    }

    if (is_stdout == 0) fclose(fp);
}

// Последовательная запись экрана в файлы
void record2ppm(const char* filename) {

    char tmp[60];

    sprintf(tmp, "%s-%06d.ppm", filename, ++rec_frame_id);
    saveppm(tmp);
}

// Записывает только если задан rec. maxtime - ограничение в кадрах (1 сек = 60 кадров)
void record(int argc, char* argv[], int maxframe) {

    if (argc > 1 && strcmp(argv[1], "rec") == 0) {

        rec_frame_id++;
        if ((rec_frame_id && rec_frame_id <= maxframe) || (maxframe == 0)) {
            saveppm("");
        }
    }
}

// Загрузка бегущей строки в память
void load_running_string(const char* rs) {

    FILE* fp = fopen(rs, "r");

    if (fp) {

        fseek(fp, 0, SEEK_END);
        int _len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (!fread(rec_running_string, 1, _len, fp)) _len = 0;
        rec_running_string[_len] = 0;
        fclose(fp);

        rec_ticker_fine     = 0;
        rec_ticker_coarse   = 0;
        qb_scroll_x         = 0;
        qb_scroll_y         = 0;
        rec_ticker_more     = qb_width;

    } else {

        printf("File [%s] not found\n", rs);
        exit(1);
    }
}

// Выполнить один сдвиг
int step_runstr(int row) {

    if (rec_ticker_fine == 0) {

        // Получение следующего символа
        char* sym;

        locate(qb_width, row);

        // Распарсить символ (UTF8)
        sym = mid(rec_running_string, rec_ticker_coarse++, 1);

        // Более символов нет: отсчитать количество символов до конца
        if (sym[0] == 0 && rec_ticker_more > 0) rec_ticker_more--;

        // Печать итогового символа. Если он непечатаемый - напечатать пробел
        if (sym[0] >= 0x20 || sym[0] < 0) print(sym); else print(" ");

    }

    for (int i = qb_font_height*row; i < qb_font_height*(row + 1); i++)
    for (int j = 0; j < qb_screen_width + 8; j++)
        pset(j, i, qb_screen[j + 1][i]);

    rec_ticker_fine = (rec_ticker_fine + 1) % 8;

    // Оставшееся количество символов
    return rec_ticker_more;
}

// Найти ближайший цвет
int dither_search_nearest(rgbf& incl) {

    // Специальная палитра для дизеринга
    if (is_dither_palette) {

        int r = incl.r < 0 ? 0 : (incl.r > 255 ? 7 : (int)(incl.r) >> 5);
        int g = incl.g < 0 ? 0 : (incl.g > 255 ? 7 : (int)(incl.g) >> 5);
        int b = incl.b < 0 ? 0 : (incl.b > 255 ? 3 : (int)(incl.b) >> 6);
        return b + (g<<2) + (r<<5);
    }

    float dist_min = 4 * pow(256, 2);
    int   dist_col = 0;

    // dospal
    for (int i = 0; i < 256; i++) {

        // Сравниваемый цвет
        struct rgbf cl = {
            (float)((qb_palette[i] >> 16) & 0xff),
            (float)((qb_palette[i] >>  8) & 0xff),
            (float)((qb_palette[i]      ) & 0xff)
        };

        // Дистанция между точками (r'-r)^2 + (g'-g)^2 + (b'-b)^2
        float dist = pow(incl.r - cl.r, 2) +
                     pow(incl.g - cl.g, 2) +
                     pow(incl.b - cl.b, 2);

        if (dist < dist_min) {

            dist_min = dist;
            dist_col = i;
        }
    }

    return dist_col;
}

// Процедура дизеринга (left, top)-(right, bottom), dith=0 (nearest) 1(dithered)
void ditherect(int left, int top, int right, int bottom, int dith) {

    rgbf ws[QB_MAX_WIDTH][QB_MAX_HEIGHT];
    int  idx[QB_MAX_WIDTH][QB_MAX_HEIGHT];

    // Копировать целочисленные точки в не целочисленные
    for (int y = 0; y < qb_screen_height; y++)
    for (int x = 0; x < qb_screen_width;  x++) {

        ws[x][y] = {
            (float) qb_pixels[x][y].r,
            (float) qb_pixels[x][y].g,
            (float) qb_pixels[x][y].b
        };
    }

    // Выполнить дизеринг
    for (int y = top;  y < bottom; y++)
    for (int x = left; x < right;  x++) {

        // Старые цвета
        rgbf old = ws[x][y];

        // Поиск ближайшего цвета из палитры
        int color_id = dither_search_nearest(old);

        // Полученный индекс цвета
        idx[x][y] = color_id;

        // Учесть ошибки квантования
        if (dith) {

            // Заменить на новый цвет (из палитры)
            rgbf ncl = {
                (float)((qb_palette[color_id] >> 16) & 0xff),
                (float)((qb_palette[color_id] >>  8) & 0xff),
                (float)((qb_palette[color_id]      ) & 0xff)
            };

            // Отметить цвет @todo может это вообще не надо
            ws[x][y] = ncl;

            // Вычисляем ошибку квантования
            rgbf quant;

            quant.r = (old.r - ncl.r);
            quant.g = (old.g - ncl.g);
            quant.b = (old.b - ncl.b);

            //   x 7
            // 3 5 1

            // [+1, +0] 7/16
            if (x + 1 < right) {

                ws[x + 1][y].r += (quant.r * 7.0/16.0);
                ws[x + 1][y].g += (quant.g * 7.0/16.0);
                ws[x + 1][y].b += (quant.b * 7.0/16.0);
            }

            // [-1, +1] 3/16
            if (x - 1 >= 0 && y + 1 < bottom) {

                ws[x - 1][y + 1].r += (quant.r * 3.0/16.0);
                ws[x - 1][y + 1].g += (quant.g * 3.0/16.0);
                ws[x - 1][y + 1].b += (quant.b * 3.0/16.0);
            }

            // [+0, +1] 5/16
            if (y + 1 < bottom) {

                ws[x][y + 1].r += (quant.r * 5.0/16.0);
                ws[x][y + 1].g += (quant.g * 5.0/16.0);
                ws[x][y + 1].b += (quant.b * 5.0/16.0);
            }

            // [+1, +1] 1/16
            if (x + 1 < right && y + 1 < bottom) {

                ws[x + 1][y + 1].r += (quant.r * 1.0/16.0);
                ws[x + 1][y + 1].g += (quant.g * 1.0/16.0);
                ws[x + 1][y + 1].b += (quant.b * 1.0/16.0);
            }
        }
    }

    useindex(1);

    // Копировать целочисленные точки в не целочисленные
    for (int y = top; y < bottom; y++)
    for (int x = left; x < right; x++)
        pset(x, y, idx[x][y]);
}

// Преобразование в цвет для вывода
int rgb(int r, int g, int b) {

    r = r < 0 ? 0 : (r > 255 ? 255 : r);
    g = g < 0 ? 0 : (g > 255 ? 255 : g);
    b = b < 0 ? 0 : (b > 255 ? 255 : b);
    return (r<<16) | (g << 8) | b;
}

// Получение угла (0..1) от X, Y
float atanxy(float x, float y) { return (M_PI - atan2(x, -y)) / (2*M_PI); }

// =====================================================================
// Обработчики SDL
// =====================================================================

// Нажатая клавиша
int sdlkey(SDL_Event event) {

    /* Получение ссылки на структуру с данными о нажатой клавише */
    SDL_KeyboardEvent * eventkey = &event.key;

    /* Получить скан-код клавиш */
    return eventkey->keysym.scancode;
}

void flip() { SDL_Flip(qb_sdl_screen); }

// Главный цикл. Параметр blocking показывает, при каком событии будет
// разблокировано ожидание. Если же blocking = 0, то ожидания нет.
int sdlevent(int blocking) {

    SDL_Event event;
    sdl_event_trigger  = EVT_NONE;
    sdl_trigger_frames = 0;

    do {

        // Получение и запись событий в активационный пул
        while (SDL_PollEvent(& event)) {

            switch (event.type) {

                case SDL_QUIT:
                    return 0;

                case SDL_KEYDOWN:

                    sdl_event_trigger |= EVT_KEY_DOWN;
                    break;

                case SDL_KEYUP:

                    sdl_event_trigger |= EVT_KEY_UP;
                    break;

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

                case SDL_USEREVENT:

                    sdl_trigger_frames++;
                    sdl_event_trigger |= EVT_REDRAW;
                    break;

                default:
                    break;
            }
        }

        // Разблокировать при наступлении события
        if ((sdl_event_trigger & blocking)) blocking = 0;

        // Есть событие на обновление экрана
        if ((sdl_event_trigger & EVT_REDRAW) && qb_autoflip) flip();

        // События отсутствуют
        if (sdl_event_trigger == 0) SDL_Delay(1);

    } while (blocking);

    return sdl_event_trigger | EVT_IDLE;
}

