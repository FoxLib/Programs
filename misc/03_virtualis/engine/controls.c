
// Нажатие мыши
void mouse_click(int button, int state, int x, int y) {
    
    switch (button) 
    {
        case GLUT_LEFT_BUTTON:            
            break;

        case GLUT_RIGHT_BUTTON:            
            break;
    }

    mouse_button  = button;
    mouse_state   = state;
    
}

// Пассивная мышь
void mouse_passive(int x, int y) {
    
    mouse_pressed = 0;    
}

// Движение мыши
void mouse_motion(int x, int y) {
    
    // Мышь только что зажата?
    if (mouse_pressed == 0)
    {
        mouse_x = x;
        mouse_y = y;
        mouse_pressed = 1;
    }

    mouse_button  = 0;
    mouse_state = 0;

    // Вращение вокруг оси X (вверх-вниз)
    camera_x += (GLfloat)(y - mouse_y) / 8.0;
    
    // Вращение вокруг оси Y (влево-вправо)
    camera_y += (GLfloat)(x - mouse_x) / 8.0;

    // Ограничитель вращения
    if (camera_x < -90) {
        camera_x = -90;
    }
    else if (camera_x > 90) {
        camera_x = 90;
    }

    mouse_x = x;
    mouse_y = y;    
}

// Нажатие спец. клавиши
void keyboard_special(int key, int x, int y) { 
    
    keyboard_spec[key] = 1; 
}

// Отжатие спец. клавиши
void keyboard_special_up(int key, int x, int y) { 
    
    keyboard_spec[key] = 0; 
}

// Отжатие клавиши
void keyboard_up(unsigned char key, int x, int y) { 
    
    keyboard[ key ] = 0;     
}

// Функция нажатия на кнопку
void keyboard_func(unsigned char key, int x, int y) {
        
    // Клавиша нажата
    keyboard[ key ] = 1;
    
    // Против дребезга
    usleep(100);

    // В случае нажатия кнопки ESC
    if (key == 27)
    { 
        // Закрыть окно
        glutDestroyWindow(window); 
          
        // Выход из программы
        exit(0);                   
    } 
}

// Обновление движения каждый кадр
void update_motion() {
        

    // Текущее время
    clock_t timer = clock();
    
    
    current_speed = ((GLfloat)(timer - last_time) / 1000.0) * 0.035;    
    current_speed = current_speed < 0 ? -current_speed : current_speed;
    
    
//current_speed *= 10;
    
    last_time = timer;
    
    // [key] W
    if (keyboard[ KEY_w ])
    {
        player_x += sin(camera_y / 180 * M_PI) * current_speed;
        player_z += cos(camera_y / 180 * M_PI) * current_speed;
    }
        
    // [key] S
    if (keyboard[ KEY_s ])
    {
        player_x += sin(camera_y / 180 * M_PI + M_PI) * current_speed;
        player_z += cos(camera_y / 180 * M_PI + M_PI) * current_speed;
    }

    // [key] D
    if (keyboard[ KEY_d ])
    {
        player_x += sin(camera_y / 180 * M_PI + M_PI/2) * current_speed;
        player_z += cos(camera_y / 180 * M_PI + M_PI/2) * current_speed;
    }

    // [key] A
    if (keyboard[ KEY_a ])
    {
        player_x += sin(camera_y / 180 * M_PI - M_PI/2) * current_speed;
        player_z += cos(camera_y / 180 * M_PI - M_PI/2) * current_speed;
    }

}
