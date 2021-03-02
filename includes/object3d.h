class object3d {

    int     used;       // =0 не использован
    uint    mtl_id;     // Номер материала
    char    name[256];  // Имя объекта
    
    vec3f*  vertex;     // Вершины
    vec2f*  texcoord;   // Текстурные координаты
    vec3f*  normals;    // Нормали
    face3*  faces;      // Динамические треугольники

    int     cnt_vertex;
    int     cnt_texcoord;
    int     cnt_normals;
    int     cnt_faces;  // Кол-во треугольников в объекте
    
    float   mat[4][4];  // Матрица преобразования в пространстве

    object3d();

    void construct();
    
    int  add_vertex(vec3f);
    int  add_normal(vec3f);
    int  add_texcoord(vec2f);
    int  add_face(int [], int [], int []);
    int  load_objfile(const char*);
};
