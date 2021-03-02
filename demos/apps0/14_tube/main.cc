#include "main.h"

// ffmpeg -r 60 -i build/%06d.jpeg -c:v libx264 -s 1920x1080 -pix_fmt yuv420p -y test.mp4
// ffmpeg -r 60 -i build/%06d.jpeg -i Observer.mp3 -c:v libx264 -c:a libvorbis -shortest -s 1920x1080 -pix_fmt yuv420p -y output.mkv

studio app(1024, 768);

uint rgb(vec3f c) {

    int r = (int)(c.x*255.0);
    int g = (int)(c.y*255.0);
    int b = (int)(c.z*255.0);
    
    return (r<<16) + (g<<8) + b;
}

// Цвет точки перевод в rgb
vec3f rgbf(uint color) {

    float r = ((color>>16)&0xff) / 255.0,
          g = ((color>>8)&0xff) / 255.0,
          b = ((color)&0xff) / 255.0;

    return {r, g, b};
}

// Расчет x,y,z
vec3f get_point(vec3f v, vec2i dim, vec2f sh, float r) {

    float x, y, z, t, at;

    t = r / (384.0 * sqrt(v.x*v.x + v.y*v.y));
    x = v.x*t;
    y = v.y*t;
    z = v.z*t;

    // Положение точки
    at = (atan2(y, x) + M_PI) / (2.0*M_PI) * (int)(r / 40.0);
    int xi = dim.x*(at + sh.x); 
    int yi = dim.y*(z  + sh.y) * 2.0;

    if (xi < 0) xi += (floor(-xi / dim.x) + 1)*dim.x;
    if (yi < 0) yi += (floor(-yi / dim.y) + 1)*dim.y;

    return {(float)(xi % dim.x), (float)(yi % dim.y), z};
}

int main(int argc, char* argv[]) {

    int i, j;
    int w  = 1024, h = 768;
    int wh = w>>1, hh = h>>1;

    image img(w, h);
    image t1(new jpeg("tex.jpg"));
    image t2(new jpeg("star.jpg"));

    float a = 0, b = 0, c = 0;
    uint color;

    int frames = (1*60 + 52)*60;

//    frames = 280;

    for (int k = 0; k < frames; k+=1) {

        a  = sin(k*0.005);
        b  = sin(k*0.007); //  + sin(a)*0.25;
        c += 0.005 + (abs(sin(k*0.001))*0.15);
        
        for (i = -hh; i < hh; i++) 
        for (j = -wh; j < wh; j++) {

            vec3f v;
            vec3f N = {(float)j / (float)wh, (float)i / (float)wh, 1.0};

            // Поворот по Y
            v.x = cos(a)*N.x - sin(a)*N.z;
            v.y = N.y;
            v.z = cos(a)*N.z + sin(a)*N.x;            

            // Поворот по X
            N.x = v.x;
            N.y = cos(b)*v.y - sin(b)*v.z;
            N.z = cos(b)*v.z + sin(b)*v.y;            

            vec3f at = get_point(N, {t1.width, t1.height}, {a*5.0, c}, 384);
            vec3f cl = rgbf( t1.point(at.x, at.y) );

            float dist;

            // Просвет -- звезды
            if (cl.x*cl.x + cl.y*cl.y + cl.z*cl.z < 0.5) {

                at = get_point(N, {t2.width, t2.height}, {a*5.0, 0}, 768);
                cl = rgbf( t2.point(at.x * 0.1, at.y * 0.1) );      
            }

            dist = 2.0 / abs(at.z);
            if (dist > 1.0) dist = 1.0;
            color = rgb({cl.x*dist, cl.y*dist, cl.z*dist});

            img.pset(wh + j, hh - 1 - i, color);
        }

        app.savejpeg(&img, k);
        img.font(8, 8, k, 0xffffff);
        app.update(&img);
    }
    
    app.loop();
    return 0;
}
