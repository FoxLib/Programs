
// Создать новый VBO контекст по заданных данным
// ---------------------------------------------------------------------
struct vbo_context create_vbo_context(
    GLuint      texture_id,         // ID текстуры в textures[]
    GLuint      shader_id,          // ID шейдера в shaders[]
    GLuint      verticles_by_face,  // Кол-во точек на сторону (3 или 4)
    GLfloat*    verticles_data,     // Набор вершин (x,y,z) (u, v) (nx,ny,nz) -- 8 float на 1 точку
    GLuint*     faces_data,         // Информация о треугольниках (3) или квадратах (4)
    GLuint      count_verticles,    // Количество вершин
    GLuint      count_faces         // Количество сторон
    ) 
{
        
    struct vbo_context context;
    
    // Перенос данных инициализации в объект
    context.vao         = 0; 
    context.vbo         = 0; 
    context.ibo         = 0; 
    context.verticles   = verticles_data; 
    context.indexes     = faces_data; 
    context.texture_id  = texture_id;
    context.shader_id   = shader_id;
    context.index_count = verticles_by_face;
    context.count_faces = count_faces;

    // Бинд параметров
    glEnableClientState(GL_NORMAL_ARRAY);

    GLuint program = shaders[ shader_id ].program;

    // Получение номеров атрибутов для данного шейдера
    // Если возвращается -1, то атрибут не имеет значения
    loc_vertex   = glGetAttribLocation(program, "position");
    loc_normal   = glGetAttribLocation(program, "normal");
    loc_texcoord = glGetAttribLocation(program, "texCoord");

    // Ширина ячейки (8 GLfloat: V.xyz; T.st; N.xyz)
    GLuint stride = 8 * sizeof(GLfloat);
    
    // Генерация и присоединение VAO
    glGenVertexArrays(1, &context.vao);
    glBindVertexArray(context.vao);

    // Генерация и присоединение VBO
    glGenBuffers(1, &context.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, context.vbo);

    // Заполнить данными
    glBufferData(GL_ARRAY_BUFFER, stride * count_verticles, context.verticles, GL_STATIC_DRAW);

    // Атрибуты вершин (идут 1-й позицией в GLSL)
    glEnableVertexAttribArray(loc_vertex);
    glVertexAttribPointer(loc_vertex, 3, GL_FLOAT, GL_FALSE, stride, (char*)0 + 0 * sizeof(GLfloat));

    // Атрибуты текстур (идут 2-й позицией в GLSL)
    glEnableVertexAttribArray(loc_texcoord);
    glVertexAttribPointer(loc_texcoord, 2, GL_FLOAT, GL_FALSE, stride, (char*)0 + 3 * sizeof(GLfloat));

    // Атрибуты нормалей (идут 3-й позицией в GLSL)
    glEnableVertexAttribArray(loc_normal);    
    glVertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, stride, (char*)0 + 5 * sizeof(GLfloat));

    // Включить нормали
    glNormalPointer(GL_FLOAT, stride, (char*)0 + 5 * sizeof(GLfloat)); 

    // Генерация Index Buffer Object
    glGenBuffers(1, &context.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.ibo);

    // Заполнение данными
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * count_faces * verticles_by_face, context.indexes, GL_STATIC_DRAW);

    // Отсоединить VAO
    glBindVertexArray(0);    
    
    // DEBUGGER
    // ----------------
    if (0 && texture_id == 7) {
        
        int i; for (i = 0; i < 8 * count_verticles; i += 8) {
                        
            printf("%d: p=(%f,%f,%f) uv=(%f,%f) n=(%f,%f,%f)\n", i/8, 
                context.verticles[i+0], context.verticles[i+1], context.verticles[i+2],
                context.verticles[i+3], context.verticles[i+4],
                context.verticles[i+5], context.verticles[i+6], context.verticles[i+7]
            );
        } 
    }
    // ----------------
    
    return context;
}

// Отрисовать VBO контекст по ссылке
// ---------------------------------------------------------------------
void draw_vbo(struct vbo_context* data) {
    
    // Использовать базовую программу шейдеров
    GLuint program = shaders[ data->shader_id ].program;
    
    // Использовать программу
    glUseProgram( program );     

    // Положение игрока в пространстве и время
    GLuint uniform_player = glGetUniformLocation(program, "player");
    GLuint uniform_timer = glGetUniformLocation(program, "timer");
    
    // Текущее положение в пространстве
    if (uniform_player != -1) {        
        glUniform3f(uniform_player, player_x, player_y, player_z);    
    } 
    
    // Время в миллисекундах в виде Float
    if (uniform_timer != -1) {        
        glUniform1f(uniform_timer, (GLfloat)clock());    
    } 
    
    // Передать параметр текущего положения в пространстве
    if (uniform_player != -1) {        
        glUniform3f(uniform_player, player_x, player_y, player_z);    
    }            
    
    // Активировать базовую текстуру; выбрать текстуру 0
    glActiveTexture(GL_TEXTURE0); 
    
    // Прицепить текстуру 0
    bindTexture(data->texture_id);
    
    // Прицепить VAO
    glBindVertexArray(data->vao);
    
    // Отрисовка элементов буфера c QUADS
    if (data->index_count == 4) {

        glDrawElements(GL_QUADS, 4 * data->count_faces, GL_UNSIGNED_INT, 0);
    
    } // .. else triangles
 
    // Отцепить VBO
    glBindVertexArray(0);   
}

// Обновить нормаль
void vbo_update_normals(

    GLuint      points_by_face,      // 3 или 4
    GLuint      face_count,
    GLfloat*    verticles_data,
    GLuint*     faces_indicies
) {
    
    int i;
    int ind_a, ind_b, ind_c;
    
    struct vec3 AB, AC;
    struct vec3 PA, PB, PC;
    
    for (i = 0; i < face_count; i++) {
        
        ind_a = faces_indicies[ points_by_face * i + 0 ];
        ind_b = faces_indicies[ points_by_face * i + 1 ];

        // В зависимости от того, квадраты или треугольники
        if (points_by_face == 4) {            
            ind_c = faces_indicies[ 4 * i + 3 ];            
        } else {
            ind_c = faces_indicies[ 3 * i + 2 ];            
        }

        // Точка 0 - базовая
        PA.x = verticles_data[ 8*ind_a + 0 ];
        PA.y = verticles_data[ 8*ind_a + 1 ];
        PA.z = verticles_data[ 8*ind_a + 2 ];

        // Точка 1 - вектор AB
        PB.x = verticles_data[ 8*ind_b + 0 ];
        PB.y = verticles_data[ 8*ind_b + 1 ];
        PB.z = verticles_data[ 8*ind_b + 2 ];

        // Точка 2 - вектор AC
        PC.x = verticles_data[ 8*ind_c + 0 ];
        PC.y = verticles_data[ 8*ind_c + 1 ];
        PC.z = verticles_data[ 8*ind_c + 2 ];
        
        GLfloat x2 = PB.x - PA.x,
                x3 = PC.x - PA.x,
                y2 = PB.y - PA.y,
                y3 = PC.y - PA.y,
                z2 = PB.z - PA.z,
                z3 = PC.z - PA.z;
                
        GLfloat Nx = y2*z3 - z2*y3;
        GLfloat Ny = z2*x3 - x2*z3;
        GLfloat Nz = x2*y3 - y2*x3;        
        GLfloat dist = sqrt(Nx*Nz + Ny*Ny + Nz*Nz);

        // Обновить вершину
        verticles_data[ 8*ind_a + 5 ] = Nx / dist;
        verticles_data[ 8*ind_a + 6 ] = Ny / dist;
        verticles_data[ 8*ind_a + 7 ] = Nz / dist;
    }    
}
