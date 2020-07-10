vec2f event_audio(float);

class audio {

public:

    static SDL_AudioSpec wanted;
    
    static int   audio_phase;
    static float audio_volume;

    audio(uint freq);
    static void fillbuffer(void* udata, Uint8* stream, int len);
};
