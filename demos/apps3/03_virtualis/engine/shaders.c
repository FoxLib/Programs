// Выделить память для загрузки шейдера
char* load_shader_source(const char* destination_file) {
    
    char* data;
    
    FILE* fp = fopen(destination_file, "r");
    
    if (!fp) {
        
        printf("[Critical] Shader '%s' not found\n", destination_file);
        exit(1);
    }
        
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // Выделить место (+0 zero)
    data = (char*)malloc(filesize + 1);
    
    // Прочесть данные
    fread(data, 1, filesize, fp);
    
    // NULL-terminated
    data[ filesize ] = 0;

    fclose(fp);    
    return data;
}

// Загрузка шейдера с диска
void shader_load(int shader_id, const char* vertex_shader, const char* fragment_shader) {
    
    int     link_ok, len;
    char    log_buffer[1024];
    
    GLuint v_shader, f_shader;

    // Загрузка шейдеров в память
    char* v_shader_src = load_shader_source(vertex_shader);
    char* f_shader_src = load_shader_source(fragment_shader);

    // Создать вершинный и фрагментные шейдеры
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Передача исходного кода
    glShaderSource(v_shader, 1, (const char**) & v_shader_src, NULL);
    glShaderSource(f_shader, 1, (const char**) & f_shader_src, NULL);
    
    // Компиляция шейдера
    glCompileShader( v_shader );
    glCompileShader( f_shader );
    
    // Создание программы
    shaders[ shader_id ].program = glCreateProgram();     
    GLuint program = shaders[ shader_id ].program;
    
    // Прикрепить шейдеры к программе
    glAttachShader( program, v_shader);
    glAttachShader( program, f_shader);   
    
    // Линкуем шейдерную программу 
    glLinkProgram( program );

    // Проверка статуса линковки программы
    glGetProgramiv( program, GL_LINK_STATUS, & link_ok );
    
    if (!link_ok) {
        
        printf("[Critical] Program not linked correctly. Shaders broken\n");
        
        glGetShaderInfoLog(v_shader, sizeof(log_buffer), &len, log_buffer);
        printf("  [Vertex] %s\n", log_buffer);
                
        glGetShaderInfoLog(f_shader, sizeof(log_buffer), &len, log_buffer);        
        printf("  [Fragment] %s\n", log_buffer);
        
        glGetProgramInfoLog(program, sizeof(log_buffer), &len, log_buffer);
        printf("  [Program] %s\n", log_buffer);                
        
        exit(1);
    }
     
    free(v_shader_src);
    free(f_shader_src);    
}

