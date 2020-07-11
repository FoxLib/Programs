// Количество точек для обработки и кол-во сторон
#define SKYBOX_FACES_CNT        5
#define SKYBOX_VERTEX_CNT       4 * SKYBOX_FACES_CNT

// Размер скайбокса
#define SKYBOX_SIZE             128.0

// 4 точки на 6 сторон, каждая точка - 8 GLfloat
struct vbo_verticle skybox_verticles[ 6 ][ 4 ];

// 6 сторон по 4 точки
GLuint skybox_faces[ 6 ][ 4 ];

// Объект Skybox для VBO
struct vbo_context skybox_vbo[6];
