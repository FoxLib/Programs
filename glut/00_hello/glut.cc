#include "glut.h"

// Объявление статической переменной
int     GLUT::GLUT_milliseconds;
GLUT*   GLUT::instance;

// Сразуж е создать окно
GLUT::GLUT(int argc, char** argv, int millis) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    GLUT_milliseconds = millis;
    instance = this;
}

// Создать новое окно
void GLUT::window(const char* t, int w, int h) {

    width  = w;
    height = h;

    glutInitWindowSize(w, h);
    glutInitWindowPosition(64, 64);

    // Создать новое окно необходимых размеров
    main_window = glutCreateWindow(t);

    // Поддержка шейдеров
    GLenum glew_status = glewInit();

    // Проверка поддержки GLEW
    if (GLEW_OK != glew_status) {

        printf("GLEW error: %s\n", glewGetErrorString(glew_status));
        if (!GLEW_VERSION_2_0) printf("No support for OpenGL 2.0 found\n");
        exit(1);
    }

    // Параметры по умолчанию
    glClearColor(0.0, 0.0, 0.0, 0);
    glDepthFunc(GL_LESS);       // Z-буфер, который проверяет тип LESS
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Режим смешивания ALPHA
    glBlendEquation(GL_FUNC_ADD); // Смешивание путем сложения цветов
    glClearDepth(1.0f);         // Включить Z-буфер
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);    // Включить текстурирование
    glEnable(GL_BLEND);         // Включить смешивание
    glEnable(GL_CULL_FACE);     // Включить срез невидимых областей
    glCullFace(GL_FRONT);       // Показать передние грани
    glFrontFace(GL_CCW);        // Тип определения передной грани: против часовой стрелки

    // Регистрация событий
    glutReshapeFunc (GLUT::func_reshape);
    glutIdleFunc    (GLUT::func_idle);
    glutDisplayFunc (GLUT::func_display);
    glutTimerFunc   (GLUT_milliseconds, GLUT::func_timer, 0);

    viewport(w, h);
}

// Установка viewport
void GLUT::viewport(int w, int h) {

    h = h > 0 ? h : 1;      // Предотвратить деление на 0
    glViewport(0, 0, w, h); // Установить вид (0, 0, w, h)

    width  = w;
    height = h;

    setcamera(0);
}

// Матрица проекции; ortho=1 - ортографическая
void GLUT::setcamera(int ortho) {

    // Переключить в матрицу проекции
    glMatrixMode(GL_PROJECTION);

    // Загрузить единичную матрицу
    glLoadIdentity();

    // Ортографическая проекция
    if (ortho)
         glOrtho(-1.0, 1.0, -1.0, 1.0, 0, 32000.0);
    else gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.01, 32000.0);

    // Переключаем в режим матрицы моделей
    glMatrixMode(GL_MODELVIEW);
}

// Загрузка текста
char* GLUT::load_text(const char* filename) {

    char* data;
    FILE* fp = fopen(filename, "r");
    int   fs;

    if (!fp) {
        printf("[КРИТИЧНО] Файл '%s' не найден\n", filename);
        exit(1);
    }

    // Получение размера файла
    fseek(fp, 0, SEEK_END);
    fs = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data = (char*) malloc(fs + 1); // Выделить место (+0 zero)
    fread(data, 1, fs, fp);        // Прочесть данные
    data[ fs ] = 0;
    fclose(fp);

    return data;
}

// Загрузка шейдера с диска
GLuint GLUT::load_shader(const char* vertex, const char* fragment) {

    int     link_ok, len;
    char    log_buffer[1024];
    GLuint  v_shader, f_shader;

    // Загрузка шейдеров в память
    char* v_shader_src = load_text(vertex);
    char* f_shader_src = load_text(fragment);

    // Создать вершинный и фрагментные шейдеры
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);

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

// Загрузка текстуры
GLuint GLUT::load_texture(GLuint* bgra, GLuint width, GLuint height) {

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

// Дефолтная текстура
GLuint GLUT::texture_xor(int w, int h) {

    GLuint* tex = (GLuint*)(malloc(w*h*sizeof(GLuint)));

    for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++) {

        unsigned char cl = i ^ j;
        tex[i*h + j] =(cl>>1)*65536 | cl*256 | (cl>>1);
    }

    GLuint texture_id = load_texture(tex, w, h);
    free(tex);

    return texture_id;
}


// Задать униформу
GLint GLUT::uniform(GLuint _prg_id, const char* name, float value) {

    GLint loc = glGetUniformLocation(_prg_id, name);
    if (loc != -1) {
        glUniform1f(loc, value);
    }
    return loc;
}
