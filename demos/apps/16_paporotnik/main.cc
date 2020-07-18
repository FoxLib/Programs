#include <qblib.c>

struct ifspoint {

    int    x, y;
    double radius;
    double theta;
};

struct generator {

    float phi, radius;
};


const generator gn[3] =
{
    { 45, 0.5 }, //  10, 0.4
    {-45, 0.5 }, // -45, 0.5
    {  6, 0.95}, //   4, 0.9
};

void ifs(ifspoint origin, int d) {


    vec2i c = {160 + origin.x, 100 - origin.y};
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

    screen(13);

    linebf(0, 0, 320, 200, 1);
    ifs({ 0, -90, 25, 0 }, 14);
    while (sdlevent(EVT_REDRAW)) {
    }

    return 0;
}
