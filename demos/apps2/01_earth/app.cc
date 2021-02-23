
class App {
protected:

    SDL2framework* win;
    struct vec3 org;
    float timer;

public:

    App(SDL2framework* _win) {

        win = _win;
        timer = 0;
    }

    int gray(int x) {

        x = x < 0 ? 0 : (x > 255 ? 255 : x);
        return (x + x*256 + x*65536);
    }

    // Арктангенс
    float atn(float x, float y) { return (atan2(y, x) + M_PI) / (2*M_PI); }

    // Псевдослучайное число
    float rnd(float x, float y) {

        float m = sin(x*12.9898 + y*78.233) * 43758.545312;
        return m - floor(m);
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

    void update() {

        struct vec3 c, cam, cl, sun, sun_n, lno, ref;
        float m, u, v, cloud, refl, lspe, dl;
        float radius = 1;

        org    = {0, 0, 1.5};
        radius = 1 + 0.25*sin(timer);
        sun    = {org.x + 4*sin(timer), 1, org.z - 4*cos(timer)};
        sun_n  = normalize({sun.x - org.x, sun.y - org.y, sun.z - org.z});

        win->cls(0);

        // Случайные звезды
        for (int i = 0; i < 255; i++) win->pset(320 * rnd(i,0), 200 * rnd(0,i), -gray(i));

        // Основной цикл обработки
        for (int y = -100; y <= 100; y++)
        for (int x = -160; x < 160; x++) {

            cl = {x: 0, y: 0, z: 0};

            // Поиск точки пересения луча со сферой
            c  = {x: (float)x / 100, y: (float)y / 100, z: 1.0};
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
                u = atn(c.x, c.z) + timer;
                v = atn(c.y, c.z);

                u = u - floor(u);
                v = v - floor(v);

                // Directional light
                dl = c.x*sun_n.x + c.y*sun_n.y + c.z*sun_n.z;
                if (dl < 0) dl = 0;

                // Текстуры
                m     = fbm(32*u, 32*v) * 255;

                // Облака с учетом directional light
                cloud = dl * (float) trunc(pow(fbm(64.0*u, 64*v), 2) * 255);

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

                win->pset(160 + x, 100 - y, -(cl.x*65536 + cl.y*256 + cl.z));
            }
        }

        timer += 0.00025;

    }
};
