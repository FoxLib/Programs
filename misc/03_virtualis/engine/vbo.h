struct vbo_context {
    
    GLuint      vao;            /* Vertex Attribute Object */
    GLuint      vbo;            /* Vertex Buffer Object */
    GLuint      ibo;            /* Index Buffer Object */
    GLuint      texture_id;     /* textures[<ID>] */
    GLuint      shader_id;      /* shaders[<ID>] */
    GLuint      index_count;    /* кол-во индексов */
    GLuint      count_faces;    /* кол-во точек на индекс */
        
    GLfloat*    verticles;      /* Ссылка на Verticles */
    GLuint*     indexes;        /* Ссылка на Indexes */
        
};

struct vbo_verticle {
    
    // Координаты
    GLfloat     x;
    GLfloat     y;
    GLfloat     z;
    
    // Текстуры
    GLfloat     u;
    GLfloat     v;
    
    // Нормали
    GLfloat     nx;
    GLfloat     ny;
    GLfloat     nz;

};

// Прототип функции создания VBO
struct vbo_context create_vbo_context(GLuint, GLuint, GLuint, GLfloat*, GLuint*, GLuint, GLuint);

// Обновить нормали для VBO
void vbo_update_normals(GLuint, GLuint, GLfloat*, GLuint*);

// Прототип уничтожения буфера VBO
// void destroy_vbo_context(struct vbo_context);
