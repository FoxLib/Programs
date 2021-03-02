opengl::opengl() { }
opengl::opengl(int c, char* v[], int w, int h, const char* title) {

    argc    = c;
    argv    = v;
    name    = (char*) title;
    width   = w;
    height  = h;

    shader_id  = 0;
    texture_id = 0;

    // Очистка клавишного состояния
    for (int i = 0; i < 256; i++) {
        keyboard[i] = {0, 0, 0};
        keybspec[i] = {0, 0, 0};
    }

    glutInit(& argc, argv);

    // Включение режима поддержки возможностей дисплея
    // --------------------------------------------------
    // GLUT_RGBA    32-х битный цвет с прозрачностью
    // GLUT_DOUBLE  двойная буферизация
    // GLUT_ALPHA   поддержка альфа-канала
    // GLUT_DEPTH   поддержка z-буфера
    // --------------------------------------------------

    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(w, h);               // Размер экрана
    window = glutCreateWindow(name);        // Создать окно
    glEnable(GL_DEPTH_TEST);                // Включить Z-буфер
    glEnable(GL_TEXTURE_2D);                // Включить текстурирование
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);   // Фоновый цвет
    GLenum glew_status = glewInit();        // Поддержка шейдеров

    if (GLEW_OK != glew_status) {

        printf("GLEW error: %s\n", glewGetErrorString(glew_status));
        if (!GLEW_VERSION_2_0) {
            printf("No support for OpenGL 2.0 found\n");
        }
        exit(1);
    }

    glClearDepth(1.0f);             // Z-буфер
    glDepthFunc(GL_LESS);           // Типа GL_LESS
    glEnable(GL_BLEND);             // Включить смешивание >> типа alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);   // Аддитивный
    glEnable(GL_CULL_FACE);         // Включить срез невидимых областей
    glCullFace(GL_FRONT);           // Передние грани
    glFrontFace(GL_CCW);            // Против часовой стрелки
}

// Выделить память для загрузки шейдера
char* opengl::load_text(const char* filename) {

    char* data;
    FILE* fp = fopen(filename, "r");
    int   fs;

    if (!fp) {
        printf("[КРИТИЧНО] Шейдер '%s' не найден\n", filename);
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

// Загрузка шейдера с диска
int opengl::load_shader(const char* vertex, const char* fragment) {

    int     link_ok, len;
    char    log_buffer[1024];

    GLuint  v_shader,
            f_shader;

    // Номер нового шейдера
    int id = shader_id++;

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
    glCompileShader( v_shader );
    glCompileShader( f_shader );

    // Создание программы
    shaders[ id ] = glCreateProgram();

    // Получить ID программы
    GLuint program = shaders[ id ];

    // Прикрепить шейдеры к программе
    glAttachShader( program, v_shader );
    glAttachShader( program, f_shader );

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

/** загрузка текстуры в память */
int opengl::load_texture(image* img) {

    if (!img->width) {
        printf("[КРИТИЧНО] Текстура не была загружена\n");
        exit(1);
    }

    // 0-непрозрачный, 255-прозрачный
    for (int i = 0; i < img->width * img->height; i++) {
        img->canvas[i] ^= 0xFF000000;
    }

    // Текстура создается в памяти, назначается GLuint
    glGenTextures(1, (GLuint *) & textures[ texture_id ]);

    // Выбираем эту текстуру, работаем с 2D: x, y.
    glBindTexture(GL_TEXTURE_2D, textures[ texture_id ]);

    // Линейная интерполяция для приближенных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST);

    // Линейная интерполяция для удаленных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Повторять текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /**
     * @url https://www.opengl.org/sdk/docs/man2/xhtml/gluBuild2DMipmaps.xml
     *
     * GLint gluBuild2DMipmaps(
     * GLenum       target,
     * GLint        internalFormat,
     * GLsizei      width,
     * GLsizei      height,
     * GLenum       format,
     * GLenum       type,
     * const void * data);
     */

     gluBuild2DMipmaps
     (
        GL_TEXTURE_2D,
        GL_RGBA,
        img->width,
        img->height,
        GL_BGRA,
        GL_UNSIGNED_BYTE,
        (void *) img->canvas
    );

     return (texture_id++);
}
