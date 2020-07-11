#define PROGRAM_WIDTH   1024
#define PROGRAM_HEIGHT  768
#define PROGRAM_NAME    "The Virtualis"

// ---------------------------------------------------------------------
// Инициализация GLUT / GL
// ---------------------------------------------------------------------

void init() {
    
    int i;    
  
    // Включение режима поддержки возможностей дисплея
    // GLUT_RGBA    32-х битный цвет с прозрачностью
    // GLUT_DOUBLE  двойная буферизация
    // GLUT_ALPHA   поддержка альфа-канала
    // GLUT_DEPTH   поддержка z-буфера

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    // Размер экрана
    glutInitWindowSize(PROGRAM_WIDTH, PROGRAM_HEIGHT);

    // Создать окно
    window = glutCreateWindow(PROGRAM_NAME);
 
    // Включить Z-буфер
    glEnable(GL_DEPTH_TEST);

    // Параметр: при очистке экрана использовать этот цвет (r=0, g=0.2, b=0.4, a=0)
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

    // Обязательно перед инициализации шейдеров 
    GLenum glew_status = glewInit(); 
    
    if (GLEW_OK != glew_status) {
        
        // GLEW не проинициализировалась
        printf("GLEW error: %s\n", glewGetErrorString(glew_status));
                  
        // Проверяем доступность OpenGL 2.0 
        if (!GLEW_VERSION_2_0) {            
            
            printf("No support for OpenGL 2.0 found\n");                    
        }
        
        exit(1);
    }

    // Включение функции Depth Test (режим GL_LESS) и значения, которым
    // будет заполнен Z-буфер

    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
   
    // Включить смешивание
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    
    // Срез невидимых областей
    glEnable(GL_CULL_FACE);     // Включить
    glCullFace(GL_FRONT);       // Передние грани
    glFrontFace(GL_CCW);        // Против часовой стрелки
 
    // СОБЫТИЯ
    // -------------------------

    // Назначение обработчика при изменений размера окна
    glutReshapeFunc(&display_reshape );

    // Обработчик отрисовщика окна
    // При первом вызове, эта функция вызывается
    glutDisplayFunc(&display_func );

    // Обработчик, когда ничего не происходит (60 раз в секунду)
    glutIdleFunc(&display_idle_func );
        
    // Регистрация обработчиков нажатий на клавишу
    glutKeyboardFunc(&keyboard_func);

    // Событие при отжатой клавише
    glutKeyboardUpFunc(&keyboard_up);

    // Специальная клавиша
    glutSpecialFunc(&keyboard_special);
    glutSpecialUpFunc(&keyboard_special_up);

    // Обработчик событий нажатий клавиш мыши
    glutMouseFunc(&mouse_click); 

    // Обработчик перемещений мыши с нажатой клавишей
    glutMotionFunc(&mouse_motion); 

    // Обработчик перемещений мыши с отпущеной клавишей
    glutPassiveMotionFunc(&mouse_passive);    
    
    // Очистка буфера текущих нажатых клавиш
    for (i = 0; i < 256; i++) {
        
        keyboard[i] = 0;
        keyboard_spec[i] = 0;
        
    }
}
