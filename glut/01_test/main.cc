#include <GL/glut.h>

int WIDTH  = 2*640;
int HEIGHT = 2*400;
int k;

GLubyte* PixelBuffer;

void pixel(int x, int y, int r, int g, int b, GLubyte* pixels, int width, int height)
{
    if (0 <= x && x < width && 0 <= y && y < height) {

        int position = (x + y * width) * 4;

        pixels[position]     = r;
        pixels[position + 1] = g;
        pixels[position + 2] = b;
        pixels[position + 3] = 0;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
        pixel(x, y, 0, 0, (x+k)*y/256, PixelBuffer, WIDTH, HEIGHT);

    glDrawPixels(WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, PixelBuffer);
    glutSwapBuffers();

    k++;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);

    PixelBuffer = new GLubyte[WIDTH * HEIGHT * 4];

    int MainWindow = glutCreateWindow("Hello Graphics");
    glClearColor(0.0, 0.0, 1.0, 0);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMainLoop();

    return 0;
}
