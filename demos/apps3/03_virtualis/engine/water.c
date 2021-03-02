// Генератор текстуры воды
void water_generator() {
    
    // Загрузка текстуры воды
    int water_texture = jpeg_texture_load( loadjpeg("data/basictex/waternormal.jpg") );    
    int i;
    
    GLfloat vertex[4][4] = {
        
        { -1.0,  1.0,  0.0, 0.0 },
        {  1.0,  1.0,  1.0, 0.0 },
        {  1.0, -1.0,  1.0, 1.0 },
        { -1.0, -1.0,  0.0, 1.0 }
        
    };
        
    for (i = 0; i < 4; i++) {

        water_verticles[i].x = 256.0 * vertex[i][0];
        water_verticles[i].y = 0.0;
        water_verticles[i].z = 256.0 * vertex[i][1];

        water_verticles[i].u = 128.0 * vertex[i][2];
        water_verticles[i].v = 128.0 * vertex[i][3];            
        water_indicies[i] = i; 
        
        subwater_verticles[i].x = vertex[i][0];
        subwater_verticles[i].y = vertex[i][1];
        subwater_verticles[i].z = 0.1;
        subwater_verticles[i].u = vertex[i][2];
        subwater_verticles[i].v = vertex[i][3];    
        
        subwater_indicies[i] = i; 
    }

    // Создать VBO с ландшафтом
    water_vbo    = create_vbo_context(water_texture, SHADER_WATER,    4, (GLfloat*)water_verticles, water_indicies, 4, 1);
    subwater_vbo = create_vbo_context(water_texture, SHADER_SUBWATER, 4, (GLfloat*)subwater_verticles, subwater_indicies, 4, 1);
}
