image::image() {
    init();
}

image::~image() {

    if (canvas  != NULL) free(canvas);
    if (zbuffer != NULL) free(zbuffer);
    if (stencil != NULL) free(stencil);
}

// Создать пустое изображение
image::image(int w, int h) {

    init();
    create(w, h);
}

// Создать изображение
image* image::create(int w, int h) {

    init();

    width  = w;
    height = h;

    center.x = w >> 1;
    center.y = h >> 1;

    fov      = center.x;
    fov2     = fov * fov;

    int size    = width * height;
    canvas      = (uint*)  malloc(sizeof(uint) * size);
    zbuffer     = (float*) malloc(sizeof(float) * size);
    stencil     = (uint*)  malloc(sizeof(uint) * size);

    cls(0);

    return this;
}

// Скопировать данные из jpeg и создать нужный canvas
template <class T>
image::image(T* obj) {

    width  = obj->width;
    height = obj->height;

    create(width, height);

    for (int i = 0; i < height; i++)
    for (int j = 0; j < width;  j++) {
        pset(j, i, obj->point(j, i));
    }
}

// Скопировать jpg в image
template <class T>
image* image::copy(T* obj) {

    for (int i = 0; i < obj->height; i++)
    for (int j = 0; j < obj->width; j++)
        pset(j, i, obj->point(j, i));

    return this;
}

void image::init() {

    canvas  = NULL;
    zbuffer = NULL;
    stencil = NULL;

    texture     = NULL;
    defcolor    = 0xffffff;
    stencil_id  = 0;
    zclip       = 0.25;

    identity();
}

// Установка точки
image* image::pset(int x, int y, uint c) {

    if (x >= 0 && x < width && y >= 0 && y < height) {
        canvas[y*width + x] = c;
    }

    return this;
}

// Извлечение точки
uint image::point(int x, int y) {

    if (x >= 0 && x < width && y >= 0 && y < height) {
        return canvas[y*width + x];
    }

    return 0;
}

// Извлечение точки по (0,0)-(w,h) циклически
uint image::point(int x, int y, int w, int h) {

    // Коррекция
    if (x < 0) x += (1 + w) * (int)(-w / x);
    if (y < 0) y += (1 + h) * (int)(-h / y);

    x %= w;
    y %= h;

    return canvas[y*width + x];
}

// Очистить экран в определенный цвет
image* image::cls(unsigned int color) {

    for (int i = 0; i < width * height; i++) {

        canvas [ i ] = color;
        stencil[ i ] = 0;
        zbuffer[ i ] = 250000.0;
    }

    return this;
}

// Создание непрозрачности на основе цвета и допуска
image* image::transparent(uint color, long diff2) {

    unsigned char rc = color>>16, gc = color>>8, bc = color;

    for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++) {

        uint  c = point(j, i) & 0xffffff;
        unsigned char r = c>>16, g = c>>8, b = c;

        // Расчет расстояния до цвета
        long  d = (r - rc)*(r - rc) +
                  (g - gc)*(g - gc) +
                  (b - bc)*(b - bc);

        // Если мало, то объект прозрачный (255), иначе непрозрачный (0)
        unsigned char t = (d <= diff2) ? 255 : 0;

        // Примешивание alpha-канала
        pset(j, i, (t << 24) | (c & 0xFFFFFF));
    }

    return this;
}

// Просто картинку
image* image::draw(image* obj) {
    return draw(obj, 0, 0);
}

// Просто картинку
image* image::draw(image* obj, int x, int y) {

    for (int i = 0; i < obj->height; i++)
    for (int j = 0; j < obj->width; j++) {
        pset(x + j, y + i, blend(obj->point(j, i), point(x+j, y+i)) );
    }

    return this;
}

// Копирует картинку в (x,y) новыми размерами (w,h)
image* image::draw(image* obj, int x, int y, int w, int h) {

    // Фактор изменения размера
    float fw = (float)obj->width  / (float)w;
    float fh = (float)obj->height / (float)h;

    for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {

        uint a = obj->point((float)j*fw, (float)i*fh);
        uint b = point(x + j, y + i);

        pset(x + j, y + i, blend(a, b));
    }

    return this;
}

// Нарисовать картинку из области (sx, sy) размером (w, h)
image* image::draw(image* obj, int x, int y, int sx, int sy, int w, int h) {

    for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
        pset(x + j, y + i, blend(obj->point(j+sx, i+sy), point(x+j, y+i)) );
    }

    return this;
}

// Нарисовать картинку из области (x1,y1-x2,y2) в область (dx1,dy1-dx2,dy2)
image* image::draw(image* obj, int x1, int y1, int x2, int y2, int dx1, int dy1, int dx2, int dy2) {

    float fw = (float)(x2 - x1) / (float)(dx2 - dx1);
    float fh = (float)(y2 - y1) / (float)(dy2 - dy1);

    for (int i = dy1; i < dy2; i++)
    for (int j = dx1; j < dx2; j++) {

        int x = x1 + fw*(float)(j - dx1);
        int y = y1 + fh*(float)(i - dy1);

        pset(j, i, blend(obj->point(x, y), point(j, i)) );
    }

    return this;
}

/* Рисование линии по алгоритму Брезенхема
 * https://ru.wikipedia.org/wiki/Алгоритм_Брезенхэма
 */
image* image::line(long x1, long y1, long x2, long y2, uint color) {

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

    return this;
}

/** Рисование незаполненного блока */
image* image::outline(int x1, int y1, int x2, int y2, uint color) {

    line(x1, y1, x2, y1, color);
    line(x1, y1, x1, y2, color);
    line(x2, y1, x2, y2, color);
    line(x1, y2, x2, y2, color);

    return this;
}

// Рисование блока
image* image::block(int x1, int y1, int x2, int y2, uint color) {

    int i;

    for (i = y1; i <= y2; i++)
        line(x1, i, x2, i, color);

    return this;
}

// Рисование окружности
image* image::circle(int xc, int yc, int radius, uint color) {

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

    return this;
}

// Рисование закрашенной окружности
image* image::circlefill(int xc, int yc, int radius, uint color) {

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

    return this;
}

// Рисование шрифтом 8x8
image* image::font(int x, int y, const char* s, uint color) {

    int i, j, height = 16;

    while (*s) {

        unsigned char ch = *s; s++;

        for (i = 0; i < height; i++) {
            for (j = 0; j < 8; j++) {

                if (font8x16[ch*16 + i] & (1<<(7-j))) {
                    pset(x + j, y + i, color);
                }
            }
        }

        x += 8;
    }

    return this;
}

// Рисовать цифры
image* image::font(int x, int y, float num, uint color) {

    char f[128];
    sprintf(f, "%.2f", num);
    font(x, y, f, color);

    return this;
}

/** Конвертер HSV -> RGB
 * H = 0..359 (цвета радуги)
 * S = 0 (не насыщенный) -- 100 (самый насыщенный)
 * V = 0 (черный) -- 255 самый яркий */
uint image::hsv2rgb(int H, int S, int V) {

    int r = 0, g = 0, b = 0;

    V = V > 255 ? 255 : V;
    S = S > 100 ? 100 : S;

    int Hi   = (H / 60) % 6;
    int Vmin = ((100 - S) * V) / 100;
    int a    = (float)(V - Vmin) * ((float)(H % 60) / 60.0);
    int Vinc = Vmin + a;
    int Vdec = V - a;

    switch (Hi) {

        case 0: r = V;    g = Vinc; b = Vmin; break;
        case 1: r = Vdec; g = V;    b = Vmin; break;
        case 2: r = Vmin; g = V;    b = Vinc; break;
        case 3: r = Vmin; g = Vdec; b = V;    break;
        case 4: r = Vinc; g = Vmin; b = V;    break;
        case 5: r = V;    g = Vmin; b = Vdec; break;
    }

    return ((r & 0xff)<<16) + ((g & 0xff) << 8) + (b & 0xff);
}

// Сложить цвета
uint image::additive(uint A, uint B) {

    // Извлечение компонент
    unsigned char ro = A >> 16, go = A >> 8, bo = A;
    unsigned char rc = B >> 16, gc = B >> 8, bc = B;

    int r = ro + rc;
    int g = go + gc;
    int b = bo + bc;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    return (r<<16) + (g<<8) + b;
}

// Смешивание
uint image::blend(uint A, uint B) {

    // Извлечение компонент
    unsigned char ro = A >> 16, go = A >> 8, bo = A, ao = (A >> 24);
    unsigned char rc = B >> 16, gc = B >> 8, bc = B;

    // Смешивание цвета
    int r = (ro*(255 - ao) + rc*ao) >> 8;
    int g = (go*(255 - ao) + gc*ao) >> 8;
    int b = (bo*(255 - ao) + bc*ao) >> 8;

    return (r<<16) + (g<<8) + b;
}

/* VGA палитра */
void image::pal(uint idx, unsigned char r, unsigned char g, unsigned char b) {

    r = (r % 64) << 2;
    g = (g % 64) << 2;
    b = (b % 64) << 2;

    vgadac[ idx & 0xff ] = (r<<16) + (g<<8) + (b);
}

uint image::pal(uint idx) {
    return vgadac[ idx & 0xff ];
}

// Умножение на матрицу
vec3f image::mult(double mat[4][4], vec3f in) {

    return {
        mat[0][0]*in.x + mat[0][1]*in.y + mat[0][2]*in.z + mat[0][3],
        mat[1][0]*in.x + mat[1][1]*in.y + mat[1][2]*in.z + mat[1][3],
        mat[2][0]*in.x + mat[2][1]*in.y + mat[2][2]*in.z + mat[2][3]
    };
}

// ---------------------------------------------------------------------
// Проекция 3D -> 2D
vec2f image::proj3d(vec3f a) {

    vec2f p;

    if (a.z == 0) a.z = 1;

    p.x = (int)(center.x + a.x * fov / a.z);
    p.y = (int)(center.y - a.y * fov / a.z);

    return p;
}

// Получение (u,v) по координатам на экране
vec2f image::getuv(int j, int i) {

    int x = j - center.x;
    int y = center.y - i;

    float u = x*param[0].x + y*param[0].y + param[0].z;
    float v = x*param[1].x + y*param[1].y + param[1].z;
    float D = x*param[2].x + y*param[2].y + param[2].z;

    if (D != 0) {
        u = u / D;
        v = v / D;
    }

    if (u < 0.0) u = 0.0;
    if (u > 1.0) u = 1.0;
    if (v < 0.0) v = 0.0;
    if (v > 1.0) v = 1.0;

    return {u, v};
}

// Получение (u,v) маппинга по u=0..1, v=0..1 на треугольнике
vec2f image::get_vecuv(vec2f u_v, vec2f a, vec2f b, vec2f c) {

    double x = a.x + (b.x - a.x)*u_v.x + (c.x - a.x)*u_v.y;
    double y = a.y + (b.y - a.y)*u_v.x + (c.y - a.y)*u_v.y;

    return {x, y};
}

// Получение (x,y,z) маппинга по u=0..1, v=0..1 на треугольнике
vec3f image::get_vecuv(vec2f u_v, vec3f a, vec3f b, vec3f c) {

    double x = a.x + (b.x - a.x)*u_v.x + (c.x - a.x)*u_v.y;
    double y = a.y + (b.y - a.y)*u_v.x + (c.y - a.y)*u_v.y;
    double z = a.z + (b.z - a.z)*u_v.x + (c.z - a.z)*u_v.y;

    return {x, y, z};
}

// Рисование треугольника в 3D (abc)
image* image::tri(vec3f a, vec3f b, vec3f c) {

    a = mult(camera_mat, a);
    b = mult(camera_mat, b);
    c = mult(camera_mat, c);

    // Вычислить параметры текстурирования
    // Сначала, получаем разности
    double  ABx = b.x - a.x, ACx = c.x - a.x,
            ABy = b.y - a.y, ACy = c.y - a.y,
            ABz = b.z - a.z, ACz = c.z - a.z;

    // Рассчитываем коэффициенты
    // A1/A2/A3
    param[0].x =  fov*(a.z*ACy - a.y*ACz);
    param[0].y =  fov*(a.x*ACz - a.z*ACx);
    param[0].z = fov2*(a.y*ACx - a.x*ACy);

    // B1/B2/B3
    param[1].x =  fov*(a.y*ABz - a.z*ABy);
    param[1].y =  fov*(a.z*ABx - a.x*ABz);
    param[1].z = fov2*(a.x*ABy - a.y*ABx);

    // C1/C2/C3
    param[2].x =  fov*(ABy*ACz - ABz*ACy);
    param[2].y =  fov*(ABz*ACx - ABx*ACz);
    param[2].z = fov2*(ABx*ACy - ABy*ACx);

    // Отрезать невидимые грани

    // Разрезать при z-clip
    // ---------------------------------

    vec3f vx[4];
    int   vx_cnt = 0;

    triangle[0] = a;
    triangle[1] = b;
    triangle[2] = c;

    for (int i = 0; i < 3; i++) {

        vec3f va = triangle[i],
              vb = triangle[(i + 1) % 3];

        // Точка лежит в правильной половине
        if (va.z >= zclip) { vx[ vx_cnt++ ] = va; }

        // Разрезать грани на 2 части
        if ((va.z > zclip && vb.z < zclip) ||
            (va.z < zclip && vb.z > zclip)) {

            double zc = (zclip - va.z) / (vb.z - va.z);

            vb.x = va.x * (1.0 - zc) + vb.x * zc;
            vb.y = va.y * (1.0 - zc) + vb.y * zc;
            vb.z = zclip;

            vx[ vx_cnt++ ] = vb;
        }
    }

    // Получился треугольник
    if (vx_cnt >= 3) {

        triangle[0] = vx[0];
        triangle[1] = vx[1];
        triangle[2] = vx[2];

        tri(proj3d(vx[0]), proj3d(vx[1]), proj3d(vx[2]));
    }

    // Получился четырехугольник
    if (vx_cnt == 4) {

        triangle[0] = vx[0];
        triangle[1] = vx[2];
        triangle[2] = vx[3];

        tri(proj3d(vx[0]), proj3d(vx[2]), proj3d(vx[3]));
    }

    return this;
}

// Главное рисование треугольника в 2D (abc)
image* image::tri(vec2f a, vec2f b, vec2f c) {

    int i, j, k;

    vec2f vt[3] = {a, b, c}, tv;

    // Сортировка точек
    for (i = 0; i < 3; i++)
    for (j = i + 1; j < 3; j++) {
        if (vt[i].y > vt[j].y) {
            tv = vt[i]; vt[i] = vt[j]; vt[j] = tv;
        }
    }

    // Не рисовать ошибочный
    if (vt[0].y == vt[2].y) {
        return this;
    }

    // Общая линия
    vec3f left = { vt[0].x, vt[0].y, (vt[2].x - vt[0].x) / (vt[2].y - vt[0].y) };

    // Перебрать 2 треугольника
    for (k = 0; k < 2; k++) {

        if (vt[k].y == vt[k+1].y) {
            continue;
        }

        vec3f right = { vt[k].x, vt[k].y, (vt[k+1].x - vt[k].x) / (vt[k+1].y - vt[k].y) };

        // Верхняя точка за верхней границей
        // Добавить смещения до 0-й точки
        if (vt[k].y < 0) {

            if (vt[k+1].y >= 0) {

                left.x  -= vt[k].y * left.z;
                right.x -= vt[k].y * right.z;
                vt[k].y  = 0;
            }
            else {

                left.x  += (vt[k+1].y - vt[k].y) * left.z;
                continue;
            }
        }

        // Рисовать построчно
        for (i = vt[k].y; i < vt[k+1].y; i++) {

            // Больше нельзя нарисовать ничего
            if (i >= height) return this;

            int lf = left.x < right.x ? left.x : right.x;
            int rt = left.x > right.x ? left.x : right.x;

            // Не рисовать, если за границами
            if (rt < 0 || lf >= width) {

                left.x  += left.z;
                right.x += right.z;
                continue;
            }

            // Прилепить к границам
            if (lf < 0) lf = 0;
            if (rt >= width) rt = width - 1;

            // Интерполяция
            int   IPL  = 0;
            int   SIZE = 8;

            // Афинный тип
            // int SIZE = rt - lf; SIZE = SIZE < 1 ? 1 : SIZE;

            vec2f UV_start = getuv(lf, i);
            vec2f UV_a = {0, 0},    UV_b = {0, 0},    UV_c = {0, 0};
            vec3f CP_a = {0, 0, 0}, CP_b = {0, 0, 0}, CP_c = {0, 0, 0};

            // Вычисление первой точки текстуры
            if (texture != NULL) { UV_b = get_vecuv(UV_start, uv[0], uv[1], uv[2]); UV_a = UV_b; }

            // Расчет точки треугольника
            CP_b = get_vecuv(UV_start, triangle[0], triangle[1], triangle[2]);

            // Рисование точки текстуры
            for (j = lf; j <= rt; j++) {

                int cpoint = width*i + j;

                // На каждой контрольной точке
                if (IPL == 0) {

                    // Расчет текстур
                    if (texture) {

                        UV_a   = UV_b; // Для текущего используется предыдущая
                        UV_b   = get_vecuv(getuv(j + SIZE, i), uv[0], uv[1], uv[2]);

                        UV_c.x = (UV_b.x - UV_a.x) / SIZE;
                        UV_c.y = (UV_b.y - UV_a.y) / SIZE;
                    }

                    CP_a   = CP_b; // Для текущего используется предыдущая
                    CP_b   = get_vecuv(getuv(j + SIZE, i), triangle[0], triangle[1], triangle[2]);

                    CP_c.x = (CP_b.x - CP_a.x) / SIZE;
                    CP_c.y = (CP_b.y - CP_a.y) / SIZE;
                    CP_c.z = (CP_b.z - CP_a.z) / SIZE;
                }

                // Точка текстуры
                int pt = texture ? texture->point(UV_a.x * texture->width, UV_a.y * texture->height) : defcolor;

                // Тест Z-буфера
                if (CP_a.z <= zbuffer[ cpoint ]) {

                    zbuffer[ cpoint ] = CP_a.z;
                    stencil[ cpoint ] = stencil_id;

                    pset(j, i, pt);
                }

                // Приращение интерполируемых значений
                if (texture) { UV_a.x += UV_c.x; UV_a.y += UV_c.y; }

                // Текущая точка (vertex_coord)
                CP_a.x += CP_c.x;
                CP_a.y += CP_c.y;
                CP_a.z += CP_c.z;

                IPL = (IPL + 1) % SIZE;
            }

            left.x  += left.z;
            right.x += right.z;
        }
    }

    return this;
}

// Треугольник с текстурой цвета
image* image::tri(image* _tex, vec5f a, vec5f b, vec5f c) {

    texture  = _tex;
    defcolor = 0xffffff;

    uv[0] = {a.u, a.v};
    uv[1] = {b.u, b.v};
    uv[2] = {c.u, c.v};

    return tri(
        {a.x, a.y, a.z},
        {b.x, b.y, b.z},
        {c.x, c.y, c.z}
    );
}

// Треугольник с текстурой цвета
image* image::tri(uint _tex, vec5f a, vec5f b, vec5f c) {

    texture  = NULL;
    defcolor = _tex;

    uv[0] = {a.u, a.v};
    uv[1] = {b.u, b.v};
    uv[2] = {c.u, c.v};

    return tri(
        {a.x, a.y, a.z},
        {b.x, b.y, b.z},
        {c.x, c.y, c.z}
    );
}

// Нарисовать квадрат
template <class T>
image* image::quad(T _tex, vec5f a, vec5f b, vec5f c, vec5f d) {

    tri(_tex, a, b, c);
    tri(_tex, a, c, d);

    return this;
}

/** Рисование очерчивающей линии вокруг объекта в stencil buffer */
image* image::stencil_outline(uint clr) {

    for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++) {

        uint am = stencil[width*i + j];

        if (((j > 0) && (am != stencil[width*i + j - 1])) ||
            ((i > 0) && (am != stencil[width*(i-1) + j]))) pset(j,i, clr);

    }

    return this;
}

/** Сброс матрицы */
image*  image::identity() {

    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
        camera_mat[i][j] = (i == j) ? 1.0 : 0.0;
    }

    return this;
}

/** Умножить камеру на матрицу */
image* image::mult(double mat[4][4]) {

    double result[4][4];

    // Умножить
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {

        double m = 0;
        for (int k = 0; k < 4; k++) {
            m += camera_mat[i][k] * mat[k][j];
        }
        result[i][j] = m;
    }

    // Скопировать
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
        camera_mat[i][j] = result[i][j];
    }

    return this;
}

/** Установка положения камеры */
image*  image::translate(vec3f v) {

    double mt[4][4];

    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
        mt[i][j] = (i == j) ? 1 : 0;
    }

    mt[0][3] = v.x;
    mt[1][3] = v.y;
    mt[2][3] = v.z;

    return mult(mt);
}

/** Вращение камеры на угол */
image* image::rotate(int axis, double angle) {

    double mt[4][4];
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
        mt[i][j] = (i == j) ? 1 : 0;
    }

    switch (axis) {

        case AXIS_X:

            mt[1][1] = cos(angle); mt[1][2] = -sin(angle);
            mt[2][1] = sin(angle); mt[2][2] =  cos(angle);
            break;

        case AXIS_Y:

            mt[0][0] =  cos(angle); mt[0][2] = sin(angle);
            mt[2][0] = -sin(angle); mt[2][2] = cos(angle);
            break;
    }

    return mult(mt);
}
