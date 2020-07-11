

char video_data[1024 * 768 * 3];        // Экранные пиксели
char bios_font[8*16*256];               // Шрифты bios

void setup() {   
    
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f); 
    
}

// Запись пикселя
void put_pixel(int x, int y, long color) {
    
    int addr = ((479 - y) * 640 + x) * 3;
    video_data[addr++ ] = (color >> 16) & 0xff;
    video_data[addr++ ] = (color >> 8) & 0xff;
    video_data[addr   ] = (color) & 0xff;
}

// Отобразить 1 символ
void display_char(int x, int y, unsigned char chr, long color) {
    
    int i, j;
    int addr = (chr * 16);
    
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 8; j++) {            
            if (src_bios_font[ addr + i ] & (1 << j)) {                
                put_pixel(x*8 + j, y*16 + i, color);        
            }            
        }
    }
    
}

// Отображение строки
void display_str(int x, int y, char* str, long color) {
    
    while (*str) {
        
        display_char(x, y, *str, color);
        x++;
        str++;    
    }
}

// Перерисовать весь дисплей заново
// ---------------------------------------------------------------------

void redraw() {
    
    int i, j;
    for (i = 0; i < 480; i++) {
    for (j = 0; j < 640; j++) {        
        put_pixel(j, i, 
            i >= 480 - 16 ? 0xcccccc : (i < 16 ? 0x00a0a0 : 0x004080)
        );        
    } }

    display_str(0, 29, "BARSIC 1.0 (C) Entropia Prescaler Ltd. All Rights Reversed. Patient 1526-13-345.", 0x000000);
    
    display_str(1, 0, "Run  About", 0x000000);
    display_str(1, 0, "R    A", 0xffffff);
    display_str(74, 0, "Ready", 0x000000);
    
    display_str(1, 2, "PRINT 'Hello, kitty!' ", 0xcccccc);
}

// Полное обновление дисплея
// ---------------------------------------------------------------------

void display()
{   
    int i, j; 
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
    // Читать пиксели каждый раз не нужно
    // glReadPixels(0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, (void*)& video_data);

    // Обновление экрана
    glDrawPixels(640, 480, GL_RGB, GL_UNSIGNED_BYTE, (void*)& video_data);
    
    // Не дать развернуть
    glutReshapeWindow(640, 480);
    glutSwapBuffers();
}
