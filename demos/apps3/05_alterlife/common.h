class TObject;
class TScene;
class TEngine;

/* Точка в пространстве */
struct TPoint {    
    
    float x;
    float y;
    float z;
};

struct TUVMapping {
    
    float u;
    float v;
    
};

/* Треугольник */
struct TFace {
        
    int    vt[3];
    struct TUVMapping uv[3];
};

struct Texture {
    
    int     w;
    int     h;
    GLuint  texture_id; 
    
};
