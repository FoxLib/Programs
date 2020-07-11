#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glut.h>

// ---------------------------------------------------------------------

#include "cpu/cpu.h"
#include "cpu/cpu.c"
#include "device/vga.h"
#include "resource/bios_font.h"
#include "gui/display.c"

// ---------------------------------------------------------------------

// Список всех опкодов
void (*func[512])() = {
    
    // add,     add,    add,    add,    addi,   addi,   pushs,  pops,
    // or,      or,     or,     or,     ori,    ori,    pushs,  pops,
    // adc,     adc,    adc,    adc,    adci,   adci    pushs,  pops,
    // sbb,     sbb,    sbb,    sbb,    sbbi,   sbbi,   pushs,  pops,
    // and,     and,    and,    and,    andi,   andi,   prefix, daa,
    // sub,     sub,    sub,    sub,    subi,   subi,   prefix, das,
    // xor,     xor,    xor,    xor,    xori,   xori,   prefix, aaa,
    // cmp,     cmp,    cmp,    cmp,    cmpi,   cmpi,   prefix, aas,

};

// ---------------------------------------------------------------------

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("i8086 emulation");

    setup();
    redraw();
    
    glutDisplayFunc(display);
    // glutIdleFunc(display); -- использовать для исполнения инструкции 
    
    
    step();

    glutMainLoop();

    return 0;
}
