#include "main.h"

// ffmpeg -r 60 -i build/%06d.jpeg -i rotor.mp3 -c:v libx264 -ac 2 -c:a libmp3lame -q:a 4 -y test.mp4

studio app(320*3, 200*3);

image dos(320, 200);
uchar sky[65536];
uchar atn[200][320];

// Палитра
void pal_generator() {

    for (int i = 0; i < 64; i++) {

        dos.pal(0x00 + i, i, i, i);
        dos.pal(0x40 + i, i, i, 63);
    }
}

// Облака
void sky_generator() {

    uint ax = 0;
    signed short  dx;
    unsigned char bl = 0;

    for (int cx = 0; cx < 65536; cx++) {

        ax  = (ax * cx) + 1;
        dx  = (ax & 0xf000);
        dx /= 4096;
        bl += dx;

        if (bl & 0x80) bl = 0;

        sky[ cx ] = bl; // 0x40 | ((bl & 0x7F) >> 1);
    }
}

// Угловой поворот
void tan_generator() {

    for (int i = -100; i < 100; i++)
    for (int j = -160; j < 160; j++) {
        atn[100 + i][160 + j] = 40.4253555453 * atan2(i, j);
    }
}

int main(int argc, char* argv[]) {

    image out(1920, 1080);

    pal_generator();
    sky_generator();
    tan_generator();

    int time = 0;
    int cl;
    
    for (int k = 0; k < 3600*60; k++) {

        for (int i = -100; i < 100; i++)
        for (int j = -160; j < 160; j++) {

            int x  = j + 160;
            int y  = i + 100;
            int rs = (i*i + j*j);
            
            int sk = sky[ ((i+100)*320 + j + time) % 65536 ];

            if (rs <= 0x0200) { // Внутренний шар
                cl = (rs >> 4) ^ 0x3f;

            } else if (rs <= 0x1900) { // Ротор

                cl = 0x10 + ((atn[y][x] + time) & 0x2e);

                // Без кольца
                if (!(cl & 0x20)) {
                    cl = 0x40 | (((cl + sk) & 0x7f) >> 1);
                }

            } else if (rs <= 0x1C00) { // Проблеск неба
                cl = 0x40 | ((sk & 0x7f) >> 1);

            } else if (rs <= 0x2300) { // Внешнее кольцо
                cl = (atn[y][x] - time) & 0x3f;

            } else { // Градиент
                cl = (i + j + 200) >> 3;
            }

            dos.pset(160 + j, 100 + i, dos.pal( cl ));
        }

        // out.draw(&dos, 0, 0, 320, 200, 71, 0, 1920-71, 1080);
        // app.savejpeg(&out, k);

        app.update(&dos);    

        SDL_Delay(10);
        time++;
    }
    
    app.loop();

    return 0;
}
