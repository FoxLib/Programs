int     audio::audio_phase;
float   audio::audio_volume;
SDL_AudioSpec audio::wanted;

audio::audio(uint freq) {

    SDL_Init(SDL_INIT_AUDIO);

    wanted.freq     = freq;
    wanted.format   = AUDIO_S8;
    wanted.channels = 2;
    wanted.samples  = 1024;
    wanted.callback = audio::fillbuffer;
    wanted.userdata = NULL;

    if (SDL_OpenAudio(& wanted, NULL) < 0) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        exit(1);
    }

    audio_phase  = 0;
    audio_volume = 127.0;

    SDL_PauseAudio(0);
}

// Основной буфер аудио
void audio::fillbuffer(void* udata, Uint8* stream, int len) {

    for (int i = 0; i < len; i += 2) {

        // Время в секундах
        float audio_time = (float)(audio_phase) / (float)wanted.freq;

        // Результат: LR.x = [-1,1], [-1,1]
        vec2f LR = event_audio( audio_time );

        // Не превышая границ
        if (LR.x <= -1.0) LR.x = -1.0;
        if (LR.x >=  1.0) LR.x =  1.0;
        if (LR.y <= -1.0) LR.y = -1.0;
        if (LR.y >=  1.0) LR.y =  1.0;

        // left -- right
        stream[i + 0] = (float) audio_volume * LR.x;
        stream[i + 1] = (float) audio_volume * LR.y;

        audio_phase++;
    }
}
