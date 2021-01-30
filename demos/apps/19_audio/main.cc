#include <screen.cc>
#include <math.h>

#define BUFFERSIZE 65536

FILE* fp;

int cursor;
int echo_delay;
int echo[BUFFERSIZE];

// Объявление
SDL_AudioSpec sdl_audio = {44100, AUDIO_U8, 1, 0, 128};

// Обработчик
void audio_callback(void *data, unsigned char *stream, int len)
{
    uint8_t buf[4096];
    uint8_t bufout[len];

    fseek(stdin, 0, SEEK_END);
    int size = fread(buf, 1, len, fp);

    for (int i = 0; i < len; i++) {

        // Учет максимального звука
        int z = echo[ (cursor) % BUFFERSIZE ];
        int v = buf[i] + z;
        int o = v - 128;

        // Ограничения [0..255]
        if (o > 127) o = 127;
        else if (o < -128) o = -128;

        // Текущий звук
        stream[i] = o + 128; // *(0.5 + 0.5*sin(cursor/100)) + 128;

        // Создание эхо
        echo[ (cursor + echo_delay) % BUFFERSIZE ] = (v - 128)/3;

        // Сдвиг курсора
        cursor++;

        // Для вывода
        bufout[i] = v;
    }

    fwrite(bufout, 1, len, stdout);
}

int main(int argc, char** argv) {

    fp = stdin;
    cursor = 0;

    echo_delay = 8192; // 4096;

    screen app(640, 480);

    // Инициализировать аудио
    sdl_audio.callback = audio_callback;
    SDL_OpenAudio(&sdl_audio, 0);
    SDL_PauseAudio(0);

    while (app.poll()) {
        // ..
    }
}

