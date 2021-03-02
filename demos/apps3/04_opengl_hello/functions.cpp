
float   CameraX, 
        CameraY, 
        CameraZ;

void MainIdleFunc() {
    // ...
}

// Вызывается при перерисовке окна
void ReshapeWindow(int width, int height) {
    
    // Задание окна для отрисовки
    glViewport(0, 0, width, height);
    
    // Матрица проекции
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.01, 1000);

    // Матрица модели
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glScalef(1.0, 1.0, -1.0);
}

void MainDisplay() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glScalef(1.0, 1.0, -1.0);
    glTranslatef(-CameraX, 0.0, -CameraZ);
    
    mtemp.draw();

    glutSwapBuffers();
    // glutPostRedisplay();
}

// При нажатии на клавишу
void KeyboardDown(unsigned char key, int x, int y) {
    
    if (key == 27) {
        exit(1);
    }
    
    // WASD
    if (key == 119) { /* W */
        CameraZ += 0.25;
    }
    
    if (key == 97) { /* A */
        CameraX -= 0.25;
    }
    
    if (key == 115) { /* S */
        CameraZ -= 0.25;                
    }
    
    if (key == 100) { /* D */
        CameraX += 0.25;
    }
    
}

// При отпускании клавиши
void KeyboardUp(unsigned char key, int x, int y) {
}

// При нажатии на клавишу
void KeyboardSpecDown(int key, int x, int y) {
}

// При отпускании клавиши
void KeyboardSpecUp(int key, int x, int y) {
}

// Событие таймера 1/60
void EventFPS(int m) {
    
    MainDisplay();
    glutTimerFunc(16, & EventFPS, m + 1);
}

// Инициализация GLUT
void init() {
    
    CameraX = 0;
    CameraY = 0;
    CameraZ = 0;
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);
    
    // Режим дисплея RGBA, есть буфер глубины и двойная буферизация
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);    
    glutInitWindowSize(1024, 768);
    glutCreateWindow("OpenGL Hello World");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Включение теста глубины
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Смешивание через вычитание Alpha-канала
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Обновление перспективы
    glutReshapeFunc(& ReshapeWindow);
  
    // Клавиатура
    glutKeyboardFunc(& KeyboardDown );
    glutKeyboardUpFunc(& KeyboardUp );

    glutSpecialFunc(& KeyboardSpecDown );
    glutSpecialUpFunc(& KeyboardSpecUp );

    // Старт таймера 
    EventFPS(0);
}
