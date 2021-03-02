// Инициализация событий
void opengl::events() {

    // Обработчик событий
    glutReshapeFunc         (event_reshape);            // При изменений размера окна
    glutDisplayFunc         (event_display_repaint);    // Отрисовщик
    glutIdleFunc            (event_display_idle);       // Кадры при IDLE
    glutKeyboardFunc        (event_kb_dn);              // Нажатие на клаву
    glutKeyboardUpFunc      (event_kb_up);              // Кнопка клавы отпущена
    glutSpecialFunc         (event_kbsp_dn);            // Спецтаз нажат
    glutSpecialUpFunc       (event_kbsp_up);            // Спецтаз отпущен
    glutMouseFunc           (event_m_click);            // Мыщъх нажат
    glutMotionFunc          (event_m_motion);           // Мышь водится
    glutPassiveMotionFunc   (event_m_passive);          // Мышь пассивна

    ogl_instance = this;
}

// Изменен размер окна
void opengl::event_reshape(int w, int h) {

    h = h > 0 ? h : 1;              // Предотвратить деление на 0
    glViewport(0, 0, w, h);         // Установить вид (0, 0, w, h)

    // Загрузить матрицу перспективы 45 градусов, вид [0.01 - 100000.0]
    glMatrixMode(GL_PROJECTION);    // Переключить в матрицу проекции
    glLoadIdentity();               // Загрузить единичную матрицу
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.01, 100000.0);
    glScalef(1.0, 1.0, -1.0);       // (3) Разворот по Z
    glRotatef(10, 0, 1, 0);         // (2) Вращение
    glTranslatef(0, 0, 3);          // (1) Поворот
    glMatrixMode(GL_MODELVIEW);     // Переключаем в режим матрицы моделей
}

// Нажата клавиша
void opengl::event_kb_dn(unsigned char key, int x, int y) {
    ogl_instance->keyboard[ key ] = {(float)x, (float)y, 1.0};
}

// Отпущена клавиша
void opengl::event_kb_up(unsigned char key, int x, int y) {
    ogl_instance->keyboard[ key ] = {(float)x, (float)y, 0.0};
}

// Нажата клавиша
void opengl::event_kbsp_dn(int key, int x, int y) {
    ogl_instance->keybspec[ key ] = {(float)x, (float)y, 1.0};
}

// Отпущена клавиша
void opengl::event_kbsp_up(int key, int x, int y) {
    ogl_instance->keybspec[ key ] = {(float)x, (float)y, 0.0};
}

// Нажата клавиша (GLUT_LEFT_BUTTON | GLUT_RIGHT_BUTTON)
void opengl::event_m_click(int button, int state, int x, int y) {

    ogl_instance->m_button  = button;
    ogl_instance->m_state   = state;
    ogl_instance->m_click   = { (float)x, (float)y };
}

// Движение мышкой
void opengl::event_m_motion(int x, int y) {

    // Мышь только что зажата?
    if (ogl_instance->mouse.z == 0) {
        ogl_instance->mouse.x = x;
        ogl_instance->mouse.y = y;
        ogl_instance->mouse.z = 1;
    }

    ogl_instance->m_button = 0;
    ogl_instance->m_state  = 0;

    ogl_instance->cam_view.x += (GLfloat)(y - ogl_instance->mouse.y);  // Вращение вокруг оси X (вверх-вниз)
    ogl_instance->cam_view.y += (GLfloat)(x - ogl_instance->mouse.x);  // Вращение вокруг оси Y (влево-вправо)

    ogl_instance->mouse.x = x;
    ogl_instance->mouse.y = y;
}

// Мышь отпущена
void opengl::event_m_passive(int x, int y) {
    ogl_instance->mouse.z = 0;
}

// Окно ничего не делает, обязательный параметр
void opengl::event_display_idle() {
}

// Обязательный вызов отрисовщика
void opengl::event_display_repaint() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ---
    glBegin(GL_QUADS); // Начинаем рисование
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0,  1.0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0,  1.0, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0, -1.0, 0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, -1.0, 0);
    glEnd();
    // ---

    glutSwapBuffers();
}
