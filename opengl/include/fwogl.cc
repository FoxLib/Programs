#include "fwogl.h"
#include "texture.cc"

// Обновление экрана 50 Гц
uint fwogl_func_sdl_timer(uint interval, void *param) {

    SDL_Event     event;
    SDL_UserEvent userevent;

    /* Создать новый Event */
    userevent.type  = SDL_USEREVENT;
    userevent.code  = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

// Запуск SDL
void startup(const char* caption, int w, int h) {

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    _sdl_screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption(caption, 0);
    SDL_AddTimer(20, fwogl_func_sdl_timer, NULL);
}

// Отслеживание событий SDL
int getevent() {

    int eventlist = 0;

    SDL_Event event;

    // Выйти как только появится событие(я)
    while (eventlist == 0) {

        // Отработка пула прерываний
        while (SDL_PollEvent(& event)) {

            switch (event.type) {

                case SDL_QUIT:
                    return 0;

                case SDL_USEREVENT:
                    eventlist |= EV_FPS;
                    break;
            }
        }

        // @todo Если требуется SDL_flip() - в случае если установлен pset()
        SDL_Delay(1);
    }

    return eventlist;
}

// Перезагрузить матрицу проекции
void glprojectmatrix() {

    glMatrixMode(GL_PROJECTION);    // Переключить в матрицу проекции
    glLoadIdentity();               // Загрузить единичную матрицу

    // Загрузить матрицу перспективы 45 градусов, вид [0.01 - 100000.0]
    gluPerspective(45.0f, (GLfloat)_glut_window_width / (GLfloat)_glut_window_height, 0.01, 100000.0);

    // (3) Разворот по Z=-Z в обратную сторону
    glScalef(1.0, 1.0, -1.0);

    // Переключаем в режим матрицы моделей
    glMatrixMode(GL_MODELVIEW);
}

// Загрузка новой матрицы проекции 3D
void glviewport3d(int w, int h) {

    h = h > 0 ? h : 1;      // Предотвратить деление на 0
    glViewport(0, 0, w, h); // Установить вид (0, 0, w, h)

    _glut_window_width  = w;
    _glut_window_height = h;

    glprojectmatrix();
}

// Изменение размера окна
void lambda_glut_resize(int width, int height) {
    glviewport3d(width, height);
}

// Создание и инициализация окна OpenGL
void glutwin(const char* name, int w, int h, int argc, char* argv[]) {

    glutInit(&argc, argv);

    // Включение всех необходимых опции
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    _glut_window = glutCreateWindow(name);

    // Проверка поддержки GLEW
    GLenum glew_status = glewInit();

    if (glew_status != GLEW_OK) {

        printf("GLEW error: %s\n", glewGetErrorString(glew_status));
        if (!GLEW_VERSION_2_0) {
            printf("No support for OpenGL 2.0 found\n");
        }
        exit(1);
    }

    // Первичная базовая инициализация
    glClearDepth(1.0f);         // Включить Z-буфер
    glDepthFunc(GL_LESS);       // Z-буфер, который проверяет тип LESS

    glEnable(GL_TEXTURE_2D);        // Включить текстурирование
    glEnable(GL_BLEND);             // Включить смешивание
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Режим смешивания ALPHA
    glBlendEquation(GL_FUNC_ADD);   // Смешивание путем сложения цветов

    glEnable(GL_CULL_FACE); // Включить срез невидимых областей
    glCullFace(GL_FRONT);   // Показать передние грани
    glFrontFace(GL_CCW);    // Тип определения передной грани: против часовой стрелки

    // Загрузка матрицы проекции
    glviewport3d(w, h);

    // Обработчик изменения размера
    glutReshapeFunc(lambda_glut_resize);
}

// Загрузка текстуры в память GPU (Alpha=0 прозрачный, 255 непрозрачный)
GLuint texture2gpu(GTexture tex) {

    GLuint  texture_id;
    GLuint* bgra = tex.data();

    if (tex.w() == 0) {
        printf("[КРИТИЧНО] Текстура не была загружена\n");
        exit(1);
    }

    // Текстура создается в памяти, назначается GLuint
    glGenTextures(1, (GLuint *) & texture_id);

    // Выбираем эту текстуру, работаем с 2D: x, y.
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Линейная интерполяция для приближенных объектов (GL_LINEAR | GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Линейная интерполяция для удаленных объектов (GL_NEAREST | GL_LINEAR_MIPMAP_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Повторять текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.w(), tex.h(), 0, GL_BGRA, GL_UNSIGNED_BYTE, bgra);

    return texture_id;
}

// Загрузка текста в память (с выделением области malloc)
char*  load_text(const char* filename) {

    char* data;
    FILE* fp = fopen(filename, "r");
    int   fs;

    if (!fp) {
        printf("[КРИТИЧНО] Файл '%s' не найден\n", filename);
        exit(1);
    }

    // Получение размера файла
    fseek(fp, 0, SEEK_END); fs = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data = (char*) malloc(fs + 1); // Выделить место (+0 zero)
    fread(data, 1, fs, fp);        // Прочесть данные
    data[ fs ] = 0;

    fclose(fp);
    return data;
}

// Загрузка шейдера в память
GLuint load_shader(const char* vertex, const char* fragment) {

    int     link_ok, len;
    char    log_buffer[1024];

    GLuint  v_shader,
            f_shader;

    // Загрузка шейдеров в память
    char* v_shader_src = load_text(vertex);
    char* f_shader_src = load_text(fragment);

    // Создать вершинный и фрагментные шейдеры
    v_shader = glCreateShader( GL_VERTEX_SHADER );
    f_shader = glCreateShader( GL_FRAGMENT_SHADER );

    // Передача исходного кода
    glShaderSource(v_shader, 1, (const char**) & v_shader_src, NULL);
    glShaderSource(f_shader, 1, (const char**) & f_shader_src, NULL);

    // Компиляция шейдера
    glCompileShader(v_shader);
    glCompileShader(f_shader);

    // Получить ID программы
    GLuint program = glCreateProgram();

    // Прикрепить шейдеры к программе
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);

    // Линкуем шейдерную программу
    glLinkProgram( program );

    // Проверка статуса линковки программы
    glGetProgramiv( program, GL_LINK_STATUS, & link_ok );

    if (!link_ok) {

        printf("[КРИТИЧНО] Программа не слинкована\n");

        glGetShaderInfoLog(v_shader, sizeof(log_buffer), &len, log_buffer);
        printf("  [VERTEX] %s\n", log_buffer);

        glGetShaderInfoLog(f_shader, sizeof(log_buffer), &len, log_buffer);
        printf("  [FRAGMENT] %s\n", log_buffer);

        glGetProgramInfoLog(program, sizeof(log_buffer), &len, log_buffer);
        printf("  [PROGRAM] %s\n", log_buffer);

        exit(1);
    }

    free(v_shader_src);
    free(f_shader_src);

    return program;
}
