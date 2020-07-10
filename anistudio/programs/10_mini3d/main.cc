#include "main.h"

image   dos(320, 240);
studio  app(640*2, 480*2);

#include "objects.cc"

int main(int argc, char* argv[]) {

    for (int i = 0; i < 5000; i++) {

        dos.cls(0xffffff);    

        dos.identity();
        dos.translate({-0.0, -0.0, 1.65}); // потом перенос
        dos.rotate(AXIS_Y, i*0.001);
 
        paint_table(1.0, 0xf0a030, {1.00, 0.30});

        uint a[] = {0x808080, 0x808080, 0x404040, 0x404040, 0x404040, 0x404040};
        paint_block({-0.5, -0.5, -0.5}, {1, 1, 0.1}, a);
        paint_block({ 0.5, -0.5, -0.5}, {1, 0.35, 0.1}, a);
        paint_block({ 0.5,  0.3, -0.5}, {1, 0.20, 0.1}, a);
        paint_block({ 1.0, -0.15, -0.5}, {0.5, 0.5, 0.1}, a);

        //dos.stencil_outline(0x0);
        app.update(&dos);    
    }

    app.loop();

    return 0;
}
