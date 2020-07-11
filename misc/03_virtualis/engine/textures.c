// Алиас glBindTexture
#define bindTexture(texture_id) glBindTexture( GL_TEXTURE_2D, textures[ texture_id ] );    

// Допустимое количество текстур 
#define MAX_TEXTURES        4096

// Объявляем их
GLuint textures[ MAX_TEXTURES ];

// Автоинкремент
GLuint tex_autoincrement = 0;

// ---------------------------------------------------------------------
// Загрузка текстуры напрямик из jpeg-файла (который находится в памяти)
// ---------------------------------------------------------------------

GLint jpeg_texture_load(struct jpeg_file_struct image) {
    
    tex_autoincrement++;
    
    // Начинаем с 1-й
    GLint texture_id = tex_autoincrement;
    
    if (!image.width) {
        
        printf("[Critical] Texture not loaded\n");
        exit(1);
    }

    // Текстура создается в памяти, назначается GLuint
    glGenTextures(1, (GLuint *) &textures[ texture_id ]);
    
    // Выбираем эту текстуру, работаем с 2D: x, y.
    glBindTexture(GL_TEXTURE_2D, textures[ texture_id ]);

    // Линейная интерполяция для приближенных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR

    // Линейная интерполяция для удаленных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Повторять текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /**
     * @url https://www.opengl.org/sdk/docs/man2/xhtml/gluBuild2DMipmaps.xml
     *
     * GLint gluBuild2DMipmaps(
     * GLenum       target,
     * GLint        internalFormat,
     * GLsizei      width,
     * GLsizei      height,
     * GLenum       format,
     * GLenum       type,
     * const void * data);
     */

     gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, (void *)image.data);
     
     return tex_autoincrement;
};
