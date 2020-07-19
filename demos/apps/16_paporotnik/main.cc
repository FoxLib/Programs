#include <qblib.c>

struct ifspoint {
    int    x, y;
    double radius;
    double theta;
};

struct generator {
    float phi, radius;
};

static generator gn[3] =
{
    {       6, 0.9          },
    {    45+6, 0.9*0.6      },
    {270+45+6, 0.9*0.6*0.6  },
};

void ifs(ifspoint origin, int d) {

    vec2i c = {320 + origin.x, 240 - origin.y};
    int   p = point(c.x, c.y);

    pset(c.x, c.y, max(p, 40 + d));

    if (d > 0) {

        for (int i = 0; i < 3; i++) {

            ifspoint dv = origin;

            dv.theta  += (gn[i].phi / 180*M_PI);
            dv.radius *=  gn[i].radius;

            dv.x += sin(dv.theta)*dv.radius;
            dv.y += cos(dv.theta)*dv.radius;

            ifs(dv, d - 1);
        }
    }
}

int main(int argc, char* argv[]) {

    screen(14);

    int iter = 0;
    while (sdlevent(EVT_REDRAW)) {

        cls();
        ifs({ 0, -120, 50, 0 }, 14);

        gn[0].phi += 0.1;
        gn[1].phi += 0.1;
        gn[2].phi += 0.1;

        if (iter < 3600) record(argc, argv, 0);

        color(15, 0); locate (0, 0); print(itoa(iter++));

    }
    return 0;
}
