#include <jpeg.h>
#include <minisdl.h>

int main(int argc, char* argv[]) {

    jpeg image(640, 480);

    // image.save("test.jpeg", 95);

    MiniSDL app(640, 480, "Application");
    app.loop();
    return 0;
}