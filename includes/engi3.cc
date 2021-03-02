#include "engi3.h"
#include "m3image.cc"

class math3 {

protected:

    int         PPD;
    int         auto_id;
    int         scene_id;
    int         planes_id;
    int         sphere_id;
    m3plane     planes[1024];
    m3sphere    spheres[1024];
    m3object    scene[2048];

public:

    math3() { init();  }

    void init() {

        PPD = 200;
        planes_id = 0;
        scene_id = 0;
        sphere_id = 0;
        auto_id = 1;

        // Инициализация сцены
        for (int i = 0; i < 2048; i++) scene[i].type = 0;
    }

    // Скалярное произведение
    float mul(vec3 a, vec3 b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    // Умножение вектора на скаляр
    vec3 mul(vec3 a, float k) {
        return {a.x*k, a.y*k, a.z*k};
    }

    // Длина вектора
    float len(vec3 n) {
        return sqrt(mul(n, n));
    }

    // Нормализация вектора
    vec3 norm(vec3 n) {
        float l = len(n);
        l = l == 0 ? 1 : 1 / l;
        return mul(n, l);
    }

    vec3 add(vec3 a, vec3 b) {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }
    vec3 sub(vec3 a, vec3 b) {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    // Получение луча для проекции с камеры
    vec3 cam(int x, int y) {
        return {(float)x / PPD, (float)y / PPD, 1.0};
    }

    // Вращение точки по оси XZ
    vec3 roty(vec3 v, float a) {

        return {
            v.x * cos(a) - v.z*sin(a),
            v.y,
            v.z * cos(a) + v.x*sin(a)
        };
    }

    // Предварительные вычисления
    m3plane plane_param(vec3 origin, vec3 a, vec3 b, vec3 c) {

        m3plane pc;

        // Для обновления в scene_update_origin
        pc.origin = origin;
        pc.a = a;
        pc.b = b;
        pc.c = c;

        // Этап предвычисления
        a = sub(a, origin);
        b = sub(b, origin);
        c = sub(c, origin);

        // Разности
        vec3 AB = sub(b, a),
             AC = sub(c, a);

        // Коэффициенты
        pc.A = { a.y*AC.z -  a.z*AC.y,  a.z*AC.x -  a.x*AC.z,  a.x*AC.y -  a.y*AC.x};
        pc.B = { a.z*AB.y -  a.y*AB.z,  a.x*AB.z -  a.z*AB.x,  a.y*AB.x -  a.x*AB.y};
        pc.C = {AB.z*AC.y - AB.y*AC.z, AB.x*AC.z - AB.z*AC.x, AB.y*AC.x - AB.x*AC.y};
        pc.D = {AC.y*AB.z - AB.y*AC.z, AB.x*AC.z - AC.x*AB.z, AC.x*AB.y - AB.x*AC.y};
        pc.t = mul(a, pc.D);

        return pc;
    }

    // Поиск точки пересечения из origin по вектору cam
    // Центр находится в ctr с radius
    castpoint cast_sphere(vec3 origin, vec3& cam, vec3 center, float radius) {

        castpoint pt;

        pt.cast = 0;
        pt.t    = 0;

        // Учесть начальную позицию точки
        vec3 ctr = sub(center, origin);

        // Расчет точки пересечения
        float a =  mul(cam, cam);
        float b = -mul(cam, ctr)*2;
        float c =  mul(ctr, ctr) - radius*radius;
        float d =  b*b - 4*a*c;

        // Точка пересечения найдена
        if (d >= 0) {

            d = sqrt(d);

            float t;
            float t1 = (-b + d) / (2*a);
            float t2 = (-b - d) / (2*a);

            if (c > 0)
                 t  = (t1 < t2) ? t1 : t2;  // Вне сферы
            else t  = (t2 < t1) ? t1 : t2;  // Внутри сферы

            // Какой самый ближний перед камерой (границей раздела)
            if (t1  < M3_ZCLIP && t2 >= M3_ZCLIP) t = t2;
            if (t1 >= M3_ZCLIP && t2  < M3_ZCLIP) t = t1;

            // Точка впереди луча
            if (t >= M3_ZCLIP) {

                pt.cast   = 1;
                pt.t      = t;
                pt.coord  = add(origin, mul(cam, t));       // Точка на сфере
                pt.normal = norm(sub(pt.coord, center));    // Нормаль к точке
                pt.uv.x   = atan2(pt.normal.z, pt.normal.x) / (2*M_PI); // u
                pt.uv.y   = atan2(pt.normal.y, pt.normal.z) / (2*M_PI); // v

                // Развернуть нормаль в обратную сторону
                if (c < 0) pt.normal = mul(pt.normal, -1);
            }
        }

        return pt;
    }

    // Поиск пересечения с плоскостью, заданной по 3-м точкам
    castpoint cast_plane(vec3& cam, m3plane& pc) {

        castpoint pt;

        pt.cast = 0;
        pt.t    = 0;

        // pt.normal == вычисляется по точкам [a, b, c]

        // Вычисление новых параметров
        float u = mul(cam, pc.A);
        float v = mul(cam, pc.B);
        float d = mul(cam, pc.C);

        // Учет нуля в детерминанте
        if (d != 0) {

            d    = 1 / d;
            pt.t = pc.t * d;

            // Точка лежит впереди
            if (pt.t >= M3_ZCLIP) {

                pt.cast  = 1;
                pt.coord = add(pc.origin, mul(cam, pt.t));
                pt.uv    = {d*u, d*v};
            }
        }

        return pt;
    }

    // Определение точки треугольника
    int is_triangle(castpoint& m) { return m.cast && m.uv.x > 0 && m.uv.y > 0 && m.uv.x + m.uv.y <= 1; }
    int is_plane(castpoint& m) { return m.cast && m.uv.x > 0 && m.uv.y > 0 && m.uv.x <= 1 && m.uv.y <= 1; }

    // Добавить плоскость на сцену
    // * M3_OBJECT_PLANE
    // * M3_OBJECT_SQUARE
    // * M3_OBJECT_TRIANGLE
    int add_plane(int type, vec3 a, vec3 b, vec3 c) {

        m3object o;
        m3plane  plane = plane_param({0, 0, 0}, a, b, c);

        // Назначить ID
        o.id   = planes_id;
        o.type = type;
        o.ai   = auto_id++;

        scene[scene_id++] = o;

        // Добавить плоскость
        planes[planes_id++] = plane;

        return (scene_id-1);
    }

    // Добавить на сцену шар
    int add_sphere(vec3 center, float radius) {

        m3object o;
        m3sphere sp;

        o.id   = sphere_id;
        o.type = M3_OBJECT_SPHERE;
        o.ai   = auto_id++;

        scene[scene_id++] = o;

        sp.center = center;
        sp.radius = radius;
        spheres[sphere_id++] = sp;

        return (scene_id-1);
    }

    // Поиск объекта пересечения
    castpoint raycast(vec3 origin, vec3 cam) {

        castpoint p, tmp;

        p.cast = 0;

        float min = -1;
        for (int i = 0; i < scene_id; i++) {

            tmp.cast = 0;

            int id   = scene[i].id;
            int ai   = scene[i].ai;
            int type = scene[i].type;

            // В зависимости от типа объекта, разный способ вычисления
            switch (type) {

                // Это суперплоскость
                case M3_OBJECT_PLANE:
                case M3_OBJECT_SQUARE:
                case M3_OBJECT_TRIANGLE:

                    // Перевычисление origin для плоскости, если нужно
                    if (planes[id].origin.x != origin.x ||
                        planes[id].origin.y != origin.x ||
                        planes[id].origin.z != origin.z) {
                        planes[id] = plane_param(origin, planes[id].a, planes[id].b, planes[id].c);
                    }

                    tmp = cast_plane(cam, planes[id]);
                    break;

                case M3_OBJECT_SPHERE:

                    tmp = cast_sphere(origin, cam, spheres[id].center, spheres[id].radius);
                    break;
            }

            // Наложение ограничений
            switch (scene[i].type) {

                case M3_OBJECT_SQUARE:   if (tmp.cast && !is_plane(tmp))    tmp.cast = 0; break;
                case M3_OBJECT_TRIANGLE: if (tmp.cast && !is_triangle(tmp)) tmp.cast = 0; break;
            }

            // Точка пересечения есть, проверить минимум
            if (tmp.cast) {

                if (min < 0 || (tmp.t > 0 && tmp.t < min)) {

                    p       = tmp;
                    p.type  = type;
                    p.ai    = ai;
                    min     = tmp.t;
                }
            }
        }

        return p;
    }

};


