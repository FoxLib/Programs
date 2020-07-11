#include <GL/freeglut.h>

/* Ядро для вызова из статических методов */
TEngine* OEngine;

class TEngine {

private:

    int    argc;
    char** argv;

protected:

    struct Texture textures[256];
    std::vector<TScene> scenes;

public:

    TEngine() {
    }

    TEngine(int* Ac, char** Av) {

        argc = *Ac;
        argv = Av;

        /* SDL init */
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
        SDL_EnableUNICODE(1);

        /* GLUT init */
        glutInit(Ac, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowSize(1024, 768);
        glutCreateWindow("Alter Life");
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /* Периодический таймер */
        glutTimerFunc(10, TEngine::GLTimer, 0);

        /* Назначить события */
        glutReshapeFunc   (TEngine::GLReshape);
        glutKeyboardFunc  (TEngine::KeybDn);
        glutKeyboardUpFunc(TEngine::KeybUp);
        glutSpecialFunc   (TEngine::KeybDnSp);
        glutSpecialUpFunc (TEngine::KeybUpSp);

        /* Записать ссылку на текущий созданный объект */
        OEngine = this;
    }

    /*
     * Главная функция отрисовки дисплея
     */

    void Display() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---------------

        glLoadIdentity();
        glTranslatef(0, 0, 2);
        
        // glActiveTexture( textures[1].texture_id );
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,  textures[1].texture_id  );
        
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-1, 1, 1); 
        glTexCoord2f(1, 0); glVertex3f( 1, 1, 1);
        glTexCoord2f(1, 1); glVertex3f( 1, -1, 1);
        glTexCoord2f(0, 1); glVertex3f(-1, -1, 1);
        glEnd();
        
        // ---------------

        glutSwapBuffers();
    }

    /* Создание сцены */
    TScene* CreateScene() {

        TScene Scene;                           // Создание объекта
        scenes.insert(scenes.end(), Scene);     // Вставка
        return & scenes.at(scenes.size() - 1);  // Возврат указателя
    }

    /* Загрузка текстур */
    void LoadTextureMap() {

        int  ct, i, texture_id;
        
        char texname[256];
        char tmp[320];
        
        TJpeg jpeg;

        FILE* f = fopen("data/texture.map", "r");
        if (f == NULL) {
            exit(1);
        }

        // Кол-во текстур
        fscanf(f, "%d", & ct); 

        for (i = 0; i < ct; i++) {

            // Прочитать текстуру
            fscanf(f, "%d %s", & texture_id, texname);            
            
            // Источник данных находится в этой папке
            sprintf(tmp, "data/texture/%s", texname);

            // Создание текстуры и загрузка ее в видеопамять
            if (jpeg.load(tmp) == 0) {

                // Сгенерировать текстуру в видеопамяти 
                glGenTextures(1, & textures[ texture_id ].texture_id );                
                
                // Получение указателя
                GLuint textureID = textures[ texture_id ].texture_id;

                // Сделаем созданную текстуру текущий, таким образом все следующие функции будут работать именно с этой текстурой
                glBindTexture(GL_TEXTURE_2D, textureID);

                // Когда изображение увеличивается, то мы используем метод ближайшего соседа
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                
                // Когда изображение уменьшается, применяется линейная фильтрация                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                // Передадим изображение OpenGL...
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, jpeg.width, jpeg.height, 0, GL_RGB, GL_UNSIGNED_BYTE, jpeg.data);
                
                // Освободить память, чтобы места не занимать
                jpeg.unset();
            }
            else {
                
                printf("Failed load %s\n", texname);
                exit(1);
            }
        }

        fclose(f);
    }

    // -----------------------------------------------------------------

    // Таймер является статической функцией (100 Hz)
    static void GLTimer(int n) {

        // Render Scene
        OEngine->Display();

        glutTimerFunc(10, TEngine::GLTimer, n + 1);
    }

    // Каждое изменение размера окна
    static void GLReshape(int width, int height) {

        // Матрица проекции
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        
        // Загрузить единичную матрицу
        glLoadIdentity();
        
        // Выполнить умножение на матрицу перспективы
        gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.01, 1000);
        
        // Развернуть в другую сторону камеру
        glScalef(1.0, 1.0, -1.0);

        // Включить матрицу модели
        glMatrixMode(GL_MODELVIEW);
    }

    static void KeybDn(unsigned char key, int x, int y) {
    }

    static void KeybUp(unsigned char key, int x, int y) {
    }

    static void KeybDnSp(int key, int x, int y) {
    }

    static void KeybUpSp(int key, int x, int y) {
    }
};
