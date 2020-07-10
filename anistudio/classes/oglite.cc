
// Конструктор
oglite::oglite(int w, int h, const char* n) {

    width  = w;
    height = h;
    name   = (char*)n;
}

// Инициализация
void oglite::init(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(width, height);      // Размер экрана
    window = glutCreateWindow(name);        // Создать окно
    glEnable(GL_DEPTH_TEST);                // Включить Z-буфер
    glEnable(GL_TEXTURE_2D);                // Включить текстурирование
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);   // Определить фоновый цвет
    GLenum glew_status = glewInit();        // Поддержка шейдеров

    // Проверка поддержки GLEW
    if (GLEW_OK != glew_status) {

        printf("GLEW error: %s\n", glewGetErrorString(glew_status));
        if (!GLEW_VERSION_2_0) {
            printf("No support for OpenGL 2.0 found\n");
        }
        exit(1);
    }

    // Включить Z-буфер
    glClearDepth(1.0f);

    // Z-буфер, который проверяет тип LESS
    glDepthFunc(GL_LESS);

    // Включить смешивание
    glEnable(GL_BLEND);

    // Режим смешивания ALPHA
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Смешивание путем сложения цветов
    glBlendEquation(GL_FUNC_ADD);

    // Включить срез невидимых областей
    glEnable(GL_CULL_FACE);

    // Показать передние грани
    glCullFace(GL_FRONT);

    // Тип определения передной грани: против часовой стрелки
    glFrontFace(GL_CCW);

    // Загрузка матрицы проекции
    viewport(width, height);
};

// Установка viewport
void oglite::viewport(int w, int h) {

    h = h > 0 ? h : 1;      // Предотвратить деление на 0
    glViewport(0, 0, w, h); // Установить вид (0, 0, w, h)

    width  = w;
    height = h;

    projmatrix();
}

// Матрица проекции
void oglite::projmatrix() {

    // Переключить в матрицу проекции
    glMatrixMode(GL_PROJECTION);

    // Загрузить единичную матрицу
    glLoadIdentity();

    // Ортографическая проекция
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0, 32000.0);

    // Загрузить матрицу перспективы 45 градусов, вид [0.01 - 32000.0]
    //gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.01, 32000.0);

    // (3) Разворот по Z=-Z в обратную сторону
    glScalef(1.0, 1.0, -1.0);

    /*
    glRotatef(10, 0, 1, 0);         // (2) Вращение
    glTranslatef(0, 0, 3);          // (1) Поворот
    */

    // Переключаем в режим матрицы моделей
    glMatrixMode(GL_MODELVIEW);
}

// Загрузка текста
char* oglite::load_text(const char* filename) {

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

// Загрузка шейдера с диска
GLuint oglite::load_shader(const char* vertex, const char* fragment) {

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
    glCompileShader( v_shader );
    glCompileShader( f_shader );

    // Получить ID программы
    GLuint program = glCreateProgram();

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

// Загрузка текстуры
GLuint oglite::load_texture(GLuint* bgra, GLuint width, GLuint height) {

    GLuint texture_id;

    if (!width) {
        printf("[КРИТИЧНО] Текстура не была загружена\n");
        exit(1);
    }

    for (uint i = 0; i < width * height; i++) {
        bgra[i] ^= 0xff000000;
    }

    // Текстура создается в памяти, назначается GLuint
    glGenTextures(1, (GLuint *) & texture_id);

    // Выбираем эту текстуру, работаем с 2D: x, y.
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Линейная интерполяция для приближенных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR | GL_NEAREST);

    // Линейная интерполяция для удаленных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Повторять текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bgra);

    return texture_id;
}
