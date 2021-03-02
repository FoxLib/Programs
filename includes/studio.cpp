/*
 * @desc Русский язык
 * @p.s. Студия для разработки анимации
 * @p.p.s. Для себя
 * @license http://fsf.org/
 * @license http://www.gnu.org/philosophy/why-not-lgpl.html
 */

class studio {

public:

    // Размер окна
    int width,  height;

    // Параметры
    int start, end, cnt;

    // Поверхность окна
    SDL_Surface*  screen;

    // Событие в окне
    SDL_Event     event;

    // Окно экспорта
    image         canvas;    

    // Конструктор класса
    studio(int _width, int _height) {

        width  = _width;
        height = _height;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
        SDL_EnableUNICODE(1);

        // Показать экран
        screen = SDL_SetVideoMode(width, height, 32, SDL_DOUBLEBUF); // SDL_HWSURFACE |  ?

        // Название
        SDL_WM_SetCaption("Animate studio", 0 );

        // 1/60 секунды
        SDL_AddTimer(16, studio::timer, NULL);

        // Создать экранный фрейм
        canvas.create(width, height);
    }

    // Получение пареметров для рендеринга
    void argv(int argc, char* argv[]) {

        if (argc < 4) { printf("param: start end count\n"); exit(0); }

        sscanf(argv[1], "%d", & start);
        sscanf(argv[2], "%d", & end);
        sscanf(argv[3], "%d", & cnt);
    }

    // -----------------------------------------------------------------
    // Обработка событий
    // -----------------------------------------------------------------

    // Вызов события таймера, которое генерирует USER-event
    static uint timer(uint interval, void * param) {

        SDL_Event     event;
        SDL_UserEvent userevent;

        /* Создать новый Event */
        userevent.type  = SDL_USEREVENT;
        userevent.code  = 0;
        userevent.data1 = NULL;
        userevent.data2 = NULL;

        event.type = SDL_USEREVENT;
        event.user = userevent;

        SDL_PushEvent(& event);

        return (interval);
    }

    // Бесконечный цикл ожидания событий, и у моря - погоды
    void loop() {

        for(;;) {

            events();
            SDL_Delay(1);
        }
    }

    // Прочитать и обработать события
    void events() {

        while (SDL_PollEvent(& event)) {

            switch (event.type) {

                case SDL_QUIT: { // Нажат крестик сверху окна

                    SDL_CloseAudio();
                    exit(0);
                }
                case SDL_MOUSEBUTTONDOWN: { // Мышь нажата

                    action_mouse_down(); break;
                }
                case SDL_MOUSEBUTTONUP: { // Мышь отпущена

                    action_mouse_up(); break;
                }
                case SDL_MOUSEMOTION: { // Мышь перемещена

                    action_mouse_motion(); break;
                }
                case SDL_KEYDOWN: { // Нажата клавиша

                    action_key_down( event.key.keysym.scancode ); break;
                }
                // @todo SDL_KEYUP
                case SDL_USEREVENT: { // Таймер

                    action_user_event(); break;
                }
            }
        }
    }

    // Обменять буфер экрана
    studio* flip() {

        SDL_Flip(screen);
        return this;
    }

    /** Сохрание фрейма jpeg */
    studio* savejpeg(image* img, int id) {

        char filename[256];

        // Создать временный JPEG
        jpeg jpg(img->width, img->height);

        // Скопировать точки из img в jpg
        for (int i = 0; i < img->height; i++)
        for (int j = 0; j < img->width; j++)
            jpg.pset(j, i, img->point(j, i));

        // Создать область памяти
        sprintf(filename, "build/%06d.jpeg", id);

        // Выгрузка с хорошим качеством в файл
        jpg.make(95);
        jpg.save(filename);

        return this;
    }

    // Обновление экрана из img
    studio* update(image* img) {

        float fw = (float)img->width  / (float)canvas.width;
        float fh = (float)img->height / (float)canvas.height;

        for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            ( (Uint32*)screen->pixels )[ j + width*i ] = img->point(fw*(float)j, fh*(float)i);

        flip();
        events();

        return this;
    }

    void action_mouse_down() { }
    void action_mouse_up() { };
    void action_mouse_motion() { };
    void action_key_down(int code) { if (code == 9) { exit(0); SDL_CloseAudio(); } } ;
    void action_user_event() { };
};
