class object3d {
    
private:    
protected:

    // Количества
    int count_vertex;   /* кол-во точек */
    int count_faces;    /* кол-во треугольников */
    
    // Массив вершин
    struct point3d vertex[ 4096 ];   
    
    // Массив треугольников 
    struct face3d faces[ 1024 ];    

public:

    // Конструктор класса
    object3d() {
        count_vertex = 0;        
        count_faces  = 0;        
    }
    
    // Загрузка файла с геометрией объекта
    int loadFile(const char* filename) {
        
        int i;        
        FILE* f = fopen(filename, "r");
        
        if (f == NULL) {
            return -1;
        }
        
        // Чтение кол-ва точек в объекте
        fscanf(f, "%d", & count_vertex);
        
        // Загрузить вершины
        for (i = 0; i < count_vertex; i++) {            
            fscanf(f, "%f %f %f", &vertex[i].x, &vertex[i].y, &vertex[i].z);            
        }
        
        // Загрузить точки соединения и uv-маппинг
        fscanf(f, "%d", & count_faces);
        for (i = 0; i < count_faces; i++) {
            fscanf(f, "%d %d %d", &faces[i].a, &faces[i].b, &faces[i].c);
        }

        fclose(f);
        return 0;
    }
    
    // Отрисуется содержимое данного объекта
    void draw() {
        
        int i, a, b, c;
        
        for (i = 0; i < count_faces; i++) {
            
            glBegin(GL_TRIANGLES);

            // Индексы вершин из vertex[]
            a = faces[i].a; b = faces[i].b; c = faces[i].c;

        glColor3f( 1.0, 1.0, 0.5 );
            glVertex3f( vertex[a].x, vertex[a].y, vertex[a].z );
        glColor3f( 1.0, 0.5, 0.5 );
            glVertex3f( vertex[b].x, vertex[b].y, vertex[b].z );
        glColor3f( 0.5, 0.5, 0.5 );
            glVertex3f( vertex[c].x, vertex[c].y, vertex[c].z );
        
            glEnd();
        }        
    }
    
};
 
