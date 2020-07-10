/** Дефолтные методы событий */
void event_display_repaint();
void event_display_idle();

/** Класс обработчик */
class opengl {

public:

    int     argc;
    char**  argv;
    int     width;
    int     height;
    char*   name;
    int     window;
    vec3f   keyboard[256]; // Обычные клавиши
    vec3f   keybspec[256]; // Специальные

    int     m_button;
    int     m_state;
    
    vec2f   m_click;        // Состояние кнопок мыши
    vec3f   mouse;          // Позиция мыши (x,y,pressed)
    vec3f   cam_view;       // Позиция камеры (x,y,0)

    GLuint  shader_id;
    GLuint  texture_id;
    GLuint  shaders[512];   // Шейдерные программы
    GLuint  textures[4096]; // Общее кол-во текстур

    // Информация о геометрии
    mtl*        mtls;       // Материалы
    object3d*   objects;    // 3Д-объекты на сцене
    int         cnt_mats;
    int         cnt_objects;
    
    opengl();
    opengl(int, char* [], int, int, const char*);

    /** Методы */
    char* load_text(const char* filename);
    int   load_shader(const char*, const char*);
    int   load_texture(image*);

    int   add(object3d);

    /** Обработка событий */
    void events();
    static void event_reshape(int, int);
    static void event_display_repaint();
    static void event_display_idle();
    static void event_kb_dn(unsigned char, int, int);
    static void event_kb_up(unsigned char, int, int);
    static void event_kbsp_dn(int, int, int);
    static void event_kbsp_up(int, int, int);
    static void event_m_click(int, int, int, int);
    static void event_m_motion(int, int);
    static void event_m_passive(int, int);
};

// Главный инстанс
opengl* ogl_instance;

