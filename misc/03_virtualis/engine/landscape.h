// Ширина ландшафта
#define LANDSCAPE_WIDTH         64

// Количество точек 
#define LANDSCAPE_VERTEX_CNT    LANDSCAPE_WIDTH * LANDSCAPE_WIDTH

// Количество прямоугольников
#define LANDSCAPE_INDEX_CNT     (LANDSCAPE_WIDTH - 1) * (LANDSCAPE_WIDTH - 1)

// -------------

// Загрузка данных из файла
unsigned char land_data[256][256];

// Здесь будет хранится [X, Y, Z; U, V]
GLfloat land_vertex_data[ 8 * LANDSCAPE_VERTEX_CNT ]; // может иначе сделать, а не так

// Здесь будут треугольники
GLuint  land_index_data[ 4 * LANDSCAPE_INDEX_CNT ]; // аналогично

// Объект ландшафта для VBO
struct vbo_context landscape_vbo;

// Номер текстуры для ландшафта
GLuint T_grass_id;

