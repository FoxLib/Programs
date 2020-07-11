
// ---------------------------------------------------------------------
// Сгенерировать ландшафт в буфер
// ---------------------------------------------------------------------

void land_load() {
    
    FILE* fp = fopen("data/landscape/256_00.bin", "r");
    
    int i;
    for (i = 0; i < 256; i++) {    
        fread(land_data[i], 1, 256, fp);
    }
    
    fclose(fp);
    
}

void land_generator() {

    int i, j, ids = 0;

    // i идет вперед --> по +Z
    for (i = 0; i < LANDSCAPE_WIDTH; i++) {

        for (j = 0; j < LANDSCAPE_WIDTH; j++) {

            // Номер текущей точки
            int cpi = i*LANDSCAPE_WIDTH + j;

            // Указатель точки в массиве вершин
            int idx = 8*cpi;

            GLfloat x = (GLfloat)j;
            GLfloat y = land_data[i][j] / 12.0 - 8.9;
            GLfloat z = (GLfloat)i;
            GLfloat w = (GLfloat)LANDSCAPE_WIDTH;

            // V.xyz
            land_vertex_data[ idx + 0 ] = x;    // X
            land_vertex_data[ idx + 1 ] = y;    // Y
            land_vertex_data[ idx + 2 ] = z;    // Z

            // T.uv
            land_vertex_data[ idx + 3 ] = z;    // U
            land_vertex_data[ idx + 4 ] = x;    // V

            // N.xyz
            land_vertex_data[ idx + 5 ] = 0;    // Nx
            land_vertex_data[ idx + 6 ] = 0;    // Ny
            land_vertex_data[ idx + 7 ] = 0;    // Nz

            // Прямоугольники (GL_QUADS)

            // [cpi   cpi+1  ]
            // [cpi+w cpi+w+1]

            if (i < LANDSCAPE_WIDTH - 1 && j < LANDSCAPE_WIDTH - 1) {

                land_index_data[ ids + 0 ] = cpi + LANDSCAPE_WIDTH;
                land_index_data[ ids + 1 ] = cpi + LANDSCAPE_WIDTH + 1;
                land_index_data[ ids + 2 ] = cpi + 1;
                land_index_data[ ids + 3 ] = cpi;

                ids += 4;
            }

        }
    }

    // Найти нормаль к каждой точке
    vbo_update_normals(4, LANDSCAPE_INDEX_CNT, land_vertex_data, land_index_data);

    // Создать VBO с ландшафтом
    landscape_vbo = create_vbo_context(
        T_grass_id,
        SHADER_LANDSCAPE,
        4,
        land_vertex_data,
        land_index_data,
        LANDSCAPE_VERTEX_CNT,
        LANDSCAPE_INDEX_CNT
    );

}

GLfloat land_pick_xy(int x, int z) {
    
    return land_vertex_data[ 8*(x + z*LANDSCAPE_WIDTH) + 1 ];
    
}

// Получение значения текущего Landscape под ногами
GLfloat land_pick_height(GLfloat x, GLfloat z) {

    GLfloat y = 0.0;

    // Ограничения
    if (x < 0.0) return y;
    if (z < 0.0) return y;
    if (x > (GLfloat)LANDSCAPE_WIDTH) return y;
    if (z > (GLfloat)LANDSCAPE_WIDTH) return y;

    // Целые            Дробная часть
    int x_int = (int)x; GLfloat x_float = x - (GLfloat)x_int;
    int z_int = (int)z; GLfloat z_float = z - (GLfloat)z_int;

    // Положение точек по Y в необходимом квадрате
    GLfloat y00 = land_pick_xy(x_int + 0, z_int + 0);   // Нижняя левая
    GLfloat y01 = land_pick_xy(x_int + 1, z_int + 0);   // Нижняя правая    
    GLfloat y10 = land_pick_xy(x_int + 0, z_int + 1);   // Верхняя левая    
    GLfloat y11 = land_pick_xy(x_int + 1, z_int + 1);   // Верхняя правая

    // Интерполяция
    GLfloat y0 = (1 - x_float) * y00 + x_float * y01;   // Нижняя
    GLfloat y1 = (1 - x_float) * y10 + x_float * y11;   // Верхняя
            y  = (1 - z_float) * y0  + z_float * y1;    // Срединная

    return y;
}
