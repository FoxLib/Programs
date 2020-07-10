#include "main.h"

studio app(1440, 960);

// mass масса земли
vec2f velocity(float mass, vec2f P, vec2f G)
{
    double A = G.x - P.x;
    double B = G.y - P.y;

    double r = A*A + B*B;
    double v = (mass * 6.67259E-11) / r;
    double K = v / sqrt(r);

    return { A*K, B*K };
}


int main(int argc, char* argv[]) {

    // Земной радиус
//    double R3 = 6.371E+6;

    int i;

    double scale = (1.0/6.371E+6) * 150.0;

    image cvs(1440, 960);

    vec2f p = {0, 1.0 * 6.371E+6};
    vec2f v = {0.0, 0.0};

    cvs.cls(0);
    cvs.circlefill(720, 480, 6.371E+6 * scale, 0x0000c0);
    cvs.circle(720, 480, 6.371E+6 * scale, 0x0040f0);

    for (i = 0; i < 50000; i++) {

        vec2f vx = velocity(5.972E+24, p, {0, 0});

        v.x += vx.x;
        v.y += vx.y;

        if (i < 10) {

            v.x += 0.1;
            v.y += 9.8 * 2.0;
        }

        else if (i < 560) {

            double Rn = 1.0/sqrt(v.x*v.x + v.y*v.y);
            vec2f Nv = { Rn*v.x, Rn*v.y };
            
            v.x += 2.4*9.8*Nv.x;
            v.y += 2.0*9.8*Nv.y;
        }

        p.x += v.x;
        p.y += v.y;

        cvs.pset(720 + p.x * scale, 480 - p.y * scale, 0xffffff);

        cvs.block(0, 0, 200, 32, 0);
        cvs.font(0, 0, p.y, 0xffffff);
        cvs.font(0, 16, i, 0xffffff);

        if (p.x*p.x + p.y*p.y < 39.589641E+12) break;

        if ((i % 1) == 0)
            app.update(&cvs);   

        //SDL_Delay(1);
    }

    app.loop();
    return 0;
}
