class raytrace {

public:

    image*      canvas;         // Изображение
    
    float   dist;               // Дистанция от камеры
    float   zclip;              // Клиппинг

    // Границы
    int     top, right, bottom, left;
    
    vec2f   center;

    /** Создать контекст из изображения */
    raytrace(image* img) {

        center.x = img->width  * 0.5;
        center.y = img->height * 0.5;
        dist     = img->width;
        canvas   = img;
        zclip    = 0.05;    

        // Границы окна при рисовании
        left     = -(img->width>>1)+1;
        right    =  (img->width>>1);
        bottom   = -(img->height>>1)+1;
        top      =  (img->height>>1);
    }

    /** Вычисление проекции */
    vec2f projection(vec3f pt) {

        vec2f proj;
        
        proj.x = center.x + pt.x*dist / pt.z;
        proj.y = center.y - pt.y*dist / pt.z;

        return proj;
    }

    /** Относительно окна pset */
    raytrace* pset(int x, int y, uint color) {
        canvas->pset(x + (int)center.x, (int)center.y - y, color);
        return this;
    }

    /** Нарисовать 3д-точку */
    raytrace* pset3d(vec3f pt, uint color) {

        if (pt.z < zclip)
            return this;

        vec2f r = projection(pt);
        canvas->pset(r.x, r.y, color);    
        return this;
    }

    /** Получение вектора для отброса лучей */
    vec3f viewport(float x, float y) {
        
        vec3f ray = {0.0,  0.0, 1.0};

        ray.x = x / (float)dist;
        ray.y = y / (float)dist;

        return ray;
    }
    /**
     * O - положение сферы в пространстве
     *     камера стоит в (0.0, 0.0, 0.0)
     * V - вектор направления рейкестера
    */

    TCollide cast_sphere(vec3f v, vec3f o, float r) {

        TCollide tc;

        tc.cast = 0;
        tc.dist = 0.0;

        float a =     v.x*v.x + v.y*v.y + v.z*v.z;           // t*t
        float b = -2*(o.x*v.x + o.y*v.y + o.z*v.z);          // t
        float c =    (o.x*o.x + o.y*o.y + o.z*o.z) - r*r;    // 1
        float det = b*b - 4*a*c;

        if (det < 0) {
            return tc;
        } else {
            det = sqrt(det);
        }

        float k1 = (-b + det) / (2*a);
        float k2 = (-b - det) / (2*a);

        // Самое близкое расстояние до сферы
        tc.dist = abs(k1) < abs(k2) ? k1 : k2;
        tc.cast = 1;

        // Расчет нормалей и координат
        tc.coord  = dot(v, tc.dist);

        // Расчет нормали к точке на сфере
        tc.normal = normal( sub(tc.coord, o) );

        // Нормализованный вектор, исходящий из точки соприкосновения от сферы к наблюдателю
        tc.direct = normal( dot(v, -1.0) );

        return tc;
    }

    /** Вычитание вектор один из другого */
    vec3f sub(vec3f a, vec3f b) {

        vec3f c;

        c.x = a.x - b.x;
        c.y = a.y - b.y;
        c.z = a.z - b.z;

        return c;
    }

    /** Сложение вектора один из другим */
    vec3f add(vec3f a, vec3f b) {

        vec3f c;

        c.x = a.x + b.x;
        c.y = a.y + b.y;
        c.z = a.z + b.z;

        return c;
    }

    /** Умножение вектора на число */
    vec3f dot(vec3f vector, float scalar) {

        vec3f r;

        r.x = vector.x * scalar;
        r.y = vector.y * scalar;
        r.z = vector.z * scalar;

        return r;
    }

    /** Векторное умножение */
    float dot(vec3f a, vec3f b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    /** Расчет длины вектора */
    float length(vec3f vec) {
        return sqrt(dot(vec, vec));
    }

    /** Вычисление нормали сферы, основываясь на координатах (x,y,z) к центру сферы */
    vec3f normal(vec3f vec) {
        return dot(vec, 1.0 / length(vec));
    }

    /** N-нормаль,
     *  L-вектор (исходящий из нормали),
     *  результат - нормализованный вектор */
    vec3f reflect(vec3f N, vec3f L) {

        // R = 2N<N,L> - L
        return normal( sub(dot( N, 2.0 * dot(N, L) ), L) );
    }

    /** Вычисление отраженного луча от результата Collide */
    vec3f reflect(TCollide castray, vec3f point) {

        // нормаль и [point - coord]
        return reflect( castray.normal, sub(point, castray.coord) );
    }

    /** Расчет нечеткого отражения, ray - нормализованный вектор */
    float specular(vec3f ray, vec3f dir, float density) {

        float sp = dot( ray, dir ) / length(dir);

        if (sp < 0.0)
            sp = 0.0;

        return pow(sp, density);
    }

    /** Не дает выйти за диапазон */
    float clamp(float val, float min, float max) {

        if (val < min)
            return min;

        if (val > max)
            return max;

        return val;
    }

    // Преобразование вектора
    vec3f vec3(float x, float y, float z) {

        return {x, y, z};
    }

    // Преобразование цвета
    vec3f rgb(uint c) {

        vec3f r;

        r.x = ((c>>16) & 0xff) / 256.0;
        r.y = ((c>>8)  & 0xff) / 256.0;
        r.z = ((c>>0)  & 0xff) / 256.0;

        return r;
    }

    // Вектор в цвет
    uint rgb(vec3f v) {

        int r = clamp(v.x * 255.0, 0, 255);
        int g = clamp(v.y * 255.0, 0, 255);
        int b = clamp(v.z * 255.0, 0, 255);

        return (r<<16) + (g<<8) + b;
    }

    // По нормали (от сферы), вычислить UV
    vec2f uv_sphere(vec3f N) {

        vec2f uv;

        uv.x = 1.0 + atan2(N.x, N.z) / (M_PI);
        uv.y = 0.5 - asin(N.y) / M_PI;

        return uv;
    }

    // Преломление Фреснела
    vec3f fresnel(TCollide tc, float k) {

        vec3f N = tc.normal;

        // N*<N,L>*(k - 1.0) - L*k
        vec3f F = sub(dot(dot(N, dot(N, tc.direct)), (k - 1.0)), dot(tc.direct, k));

        // Нормализованный вектор
        return normal(F);
    }

    // Длина до проекции точки C на линию (ab)
    float getprojection(vec3f a, vec3f b, vec3f c) {

        vec3f ab = sub(b, a);
        vec3f ac = sub(c, a);
        float dt = dot(ab, ab);
        return (dt == 0) ? 0 : dot(ab, ac) / dt;
    }

    // Расчет проекции точки C на линию (ab)
    vec3f abprojection(vec3f a, vec3f b, vec3f c) {

        // r = a + (b-a)*t
        return add(a, dot(sub(b, a), getprojection(a, b, c)));
    }
};
