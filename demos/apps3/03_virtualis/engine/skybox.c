// Генерирование Skybox
void skybox_generator() {
    
    int i, j, k, tex_id;
    
    char buffer[256];
    
    char* skybox_images[6] = {
        "front",
        "back",
        "right",
        "left",
        "top",
        "bottom"
    };    
    
    GLfloat verticles[ SKYBOX_VERTEX_CNT ][6] = {
        
        // x    y     z     u    v
        
        // Спереди
        {-1.0,  1.0,  1.0, 0.0, 0.0 },
        { 1.0,  1.0,  1.0, 1.0, 0.0 },
        { 1.0, -1.0,  1.0, 1.0, 1.0 },
        {-1.0, -1.0,  1.0, 0.0, 1.0 },

        // Сзади
        { 1.0,  1.0, -1.0, 0.0, 0.0 },
        {-1.0,  1.0, -1.0, 1.0, 0.0 },
        {-1.0, -1.0, -1.0, 1.0, 1.0 },
        { 1.0, -1.0, -1.0, 0.0, 1.0 },
        
        // Справа
        { 1.0,  1.0,  1.0, 0.0, 0.0 },
        { 1.0,  1.0, -1.0, 1.0, 0.0 },
        { 1.0, -1.0, -1.0, 1.0, 1.0 },
        { 1.0, -1.0,  1.0, 0.0, 1.0 },
        
        // Слева
        {-1.0,  1.0, -1.0, 0.0, 0.0 },
        {-1.0,  1.0,  1.0, 1.0, 0.0 },
        {-1.0, -1.0,  1.0, 1.0, 1.0 },
        {-1.0, -1.0, -1.0, 0.0, 1.0 },
        
        // Сверху
        {-1.0,  1.0, -1.0, 0.0, 0.0 },
        { 1.0,  1.0, -1.0, 1.0, 0.0 },
        { 1.0,  1.0,  1.0, 1.0, 1.0 },
        {-1.0,  1.0,  1.0, 0.0, 1.0 }
        
    };
    
    for (i = 0; i < SKYBOX_FACES_CNT; i++) {
        
        // Выборка файла
        sprintf(buffer, "data/skybox/sky_%s.jpg", skybox_images[i]);
        
        // Загрузка текстуры в память
        tex_id = jpeg_texture_load( loadjpeg(buffer) );    
        
        // Определение 4 точек
        for (j = 0; j < 4; j++) {
            
            GLfloat u = verticles[4*i + j][3];
            GLfloat v = verticles[4*i + j][4];
            
            // Для предотвращения WRAP
            if (u == 0.0) u = 0.001;
            if (u == 1.0) u = 0.999;
            if (v == 0.0) v = 0.001;
            if (v == 1.0) v = 0.999;
            
            skybox_verticles[ i ][ j ].x = verticles[4*i + j][0] * SKYBOX_SIZE; 
            skybox_verticles[ i ][ j ].y = verticles[4*i + j][1] * SKYBOX_SIZE;
            skybox_verticles[ i ][ j ].z = verticles[4*i + j][2] * SKYBOX_SIZE;
            skybox_verticles[ i ][ j ].u = u;
            skybox_verticles[ i ][ j ].v = v;
            
            // Описываем сами квадраты (точки соединения)
            skybox_faces[i][j] = j;
        }

        // Создать Skybox
        skybox_vbo[i] = create_vbo_context(
            tex_id, 0, 4, /* текстура, номер шейдера, 4-гольник */
            (GLfloat*)skybox_verticles[i], 
            skybox_faces[i], 
            4, 1 /* 4 точки, 1 сторона */
        );

    }
}
