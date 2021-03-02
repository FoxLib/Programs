#include "main.h"

studio app(960, 540);

int main(int argc, char* argv[]) {

    image   img(960, 540);
    image   legend(new ppm("fresnel.ppm") ); legend.transparent(0, 0);

    vec2f L = {-1.0, 1.5};
    vec2f N = {0.0, 1.0};

    float p = 0;

    for (int id = 0; id < 60*30; id++) {

        img.cls(0);

        img.draw(& legend, 0, 0);
        img.line(0,270,960,270,0x404040);
        img.line(480,0,480,540,0x404040);

        L.x = sin(p); N.x = sin(p * 0.125);
        L.y = cos(p); N.y = cos(p * 0.125);

        p += 0.01;

        // Нормализовать
        float len = sqrt(N.x*N.x + N.y*N.y); N.x /= len; N.y /= len;

        // R = 2N<N,L> - L
        float dot = L.x*N.x + L.y*N.y;

        /*
        float Rx = 2*N.x*dot - L.x;
        float Ry = 2*N.y*dot - L.y;
        */

        float fres = 0.5;

        // Преломление = (fresnel-1.0)*N<N,L> - fresnel*L
        float Rx = N.x*dot*(fres - 1.0) - fres*L.x;
        float Ry = N.y*dot*(fres - 1.0) - fres*L.y;

        // Прорисовать
        img.line(480,270,480+150*L.x,270-150*L.y,0x00ff00);

        img.line(480,270,480+150*N.x,270-150*N.y,0xffff00);
        img.line(480,270,480-150*N.x,270+150*N.y,0xc0c000);
        img.line(480,270,480+150*N.y,270+150*N.x,0x707070);
        img.line(480,270,480-150*N.y,270-150*N.x,0x707070);
        img.line(480,270,480+150*Rx,270-150*Ry,0x00c0ff);

        app.update(& img);
        app.savejpeg(& img, id);
    }

    return 0;
}
