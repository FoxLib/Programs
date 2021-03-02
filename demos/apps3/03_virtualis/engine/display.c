#define MIN_CAMERA_DISTANCE     0.05
#define MAX_CAMERA_DISTANCE     16384.0

// ---------------------------------------------------------------------
// Функция, которая перенастраивает матрицу проекции
// при изменениий размера окна
// ---------------------------------------------------------------------

void display_reshape(int w, int h) {

    // Предотвратить деление на 0
    h = h > 0 ? h : 1;

    // Установить вид (0, 0, w, h) - новый размер окна
    glViewport(0, 0, w, h);

    // Переключиь в матрицу проекции
    glMatrixMode(GL_PROJECTION);

    // Загрузить единичную матрицу
    glLoadIdentity();

    // Загрузить матрицу перспективы
    // Соотношение зависит от заданных (w, h)

    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE);

    // Развернуть камеру в сторону +Z
    glScalef(1.0, 1.0, -1.0);

    // Переключаем в режим матрицы моделей
    glMatrixMode(GL_MODELVIEW);
}

// Отрисовка кадра
void display_func() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Обновить движение
    update_motion();
    
    // Отрисовка Skybox
    camera_init_rotate();

    int i;
    for (i = 0; i < SKYBOX_FACES_CNT; i++) {
        draw_vbo(& skybox_vbo[i]);
    }

    // Отрисовка ландшафта
    camera_init_rotate();
    
    // Высота над ландшафтом
    player_y = 0.5 + land_pick_height(player_x, player_z);    
    
    // Положение игрока в пространстве
    glTranslatef(-player_x, -player_y, -player_z);

    // Нарисовать ландшафт
    draw_vbo(& landscape_vbo);
    
    // Воду отрисовать в самый последний момент
    glDisable(GL_CULL_FACE);
    draw_vbo(& water_vbo);
    glEnable(GL_CULL_FACE); 

    // Если мы под водой
    if (player_y < 0) {
        
        glLoadIdentity();
        draw_vbo(& subwater_vbo);        
    }

    glFlush();
    glutSwapBuffers();
}

// Пассивный режим
void display_idle_func() {

    display_func();
}
