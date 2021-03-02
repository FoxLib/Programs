// Клавиши управления
#define KEY_w       119
#define KEY_s       115
#define KEY_a       97
#define KEY_d       100

// Состояния клавиш
int keyboard[256];
int keyboard_spec[256];

// Состояние нажатия кнопки
int mouse_button = 0;

// Состояние мыши
int mouse_state = 0;

// Сигнал зажатой мыши
int mouse_pressed = 0;

// Последняя позиция мыши
int mouse_x = 0;
int mouse_y = 0;

// Предыдущее время
clock_t last_time = 0;

// Текущая скорость
GLfloat current_speed = 0.0;
