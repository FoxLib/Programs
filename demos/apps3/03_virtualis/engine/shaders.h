// Имена шейдеров
#define SHADER_STANDART     0
#define SHADER_LANDSCAPE    1
#define SHADER_WATER        2
#define SHADER_SUBWATER     3

// Максимальное кол-во
#define MAX_SHADERS_PROGRAM     4096

struct shader {
    
    GLuint  program;

};

// Инициализация шейдеров
struct shader shaders[ MAX_SHADERS_PROGRAM ];

GLuint loc_vertex = 0;
GLuint loc_texcoord = 1;
GLuint loc_normal = 2;
