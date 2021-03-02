#include "main.h"

studio app(1024, 768);

vec3f normalize(vec3f n) {

    float l = (n.x*n.x + n.y*n.y + n.z*n.z);
    l = l == 0 ? 1 : 1.0 / sqrt(l);
    return {n.x*l, n.y*l, n.z*l};

}

int main(int argc, char* argv[]) {

    image img(1024, 768);
    image water(new jpeg("water.jpg"));

    int i, j, k;

    float x, y, z, a;
    float plane = -1500.0, ppd = 384.0;

    for (k = 0; k < 6120; k+=1) {

        a = (k / 360.0) * 3.141592;

        plane += 1;

        for (i = 0; i < 768; i++) 
        for (j = 0; j < 1024; j++) {

            int xi = j - 512, yi = 384 - i;

            if (yi < 0) {

                y = plane;
                z = y * ppd / (float) yi;
                x = xi * z / ppd;

                // Нормаль
                uint  CL = water.point((int)(x*1.0) & 0x1ff, (int)(z*1.0) & 0x1ff);

                float r = ((CL>>16) & 0xff);
                float g = ((CL>>8) & 0xff);
                float b = ((CL>>0) & 0xff);

                float Nx = (r - 128.0) / 128.0;
                float Ny = (g - 128.0) / 128.0;
                float Nz = (b - 128.0) / 128.0;

                Nx = (Nx + sin(10.0*a))*0.075;
                Ny = (Ny + cos(10.0*a))*0.075;

                vec3f N   = normalize({ Nx, Nz, Ny });

                // Вектор отражения Rv к Lp
                vec3f L   = normalize({x, y, z});
                float dot = N.x*L.x + N.y*L.y + N.z*L.z;
                              
                vec3f R = {
                    2.0*N.x*dot - L.x,
                    2.0*N.y*dot - L.y,
                    2.0*N.z*dot - L.z
                };

                float Xp = R.x * ppd / R.z;
                float Yp = R.y * ppd / R.z;

                img.pset(j, i, (((int)(Xp) ^ (int)(Yp)) & 0xff) << 8 );
                
                //img.pset(j, i, water.point(((int)Xp) & 0x1FF, ((int)Yp) & 0x1FF) );
                //img.pset(j, i, CL);                
                //img.pset(j, i, ((int)(r*128.0 + 128.0)<<16) + ((int)(g*128.0 + 128.0)<<8) + (int)(b*128.0 + 128.0));
                //img.pset(j, i, ((int)(r*128.0 + 128.0)<<16) + ((int)(g*128.0 + 128.0)<<8));
                //img.pset(j, i, ((int)(b*128.0 + 128.0)));
                
            } else {

                img.pset(j, i, (((int)(xi) ^ (int)(yi + abs(0.025*plane))) & 0xff) << 8 );
            }
        }

        app.update(&img);
        app.savejpeg(&img, k);

        //SDL_Delay(5);
    }
    
    app.loop();
    return 0;
}
