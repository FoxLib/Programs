enum AXISES {

    AXIS_X,
    AXIS_Y,
    AXIS_Z,    
};

class image {

public:

    int     width, height;
    uint*   canvas;
    uint    vgadac[ 256 ] ;

    /* 3D */
    float*  zbuffer;        // z-буфер
    double  zclip;          // клиппинг

    uint*   stencil;        // stencil-буфер
    uint    stencil_id;     // рисование id при выводе точек
    
    vec2f   center;         // центр экрана
    image*  texture;        // текущая текстура
    uint    defcolor;       // цвет по умолчанию
    float   fov;            // параметр FOV
    float   fov2;           // параметр FOV*FOV
    vec2f   uv[3];          // координаты текстуры
    vec3f   param[3];       // параметры текстурирования
    vec3f   triangle[3];    // точки 3d треугольника
    double  camera_mat[4][4];  // матрица     

    image();
    ~image();

    image(int, int);

    template <class T>
    image(T* obj);

    template <class T>
    image*  copy(T* obj);

    void    init();
    image*  create(int w, int h);
    image*  pset(int x, int y, uint c);
    uint    point(int x, int y);
    uint    point(int x, int y, int w, int h);
    image*  cls(unsigned int color);
    image*  transparent(uint color, long diff2);
    
    image*  draw(image* obj);
    image*  draw(image* obj, int x, int y);
    image*  draw(image* obj, int x, int y, int w, int h);
    image*  draw(image* obj, int x, int y, int sx, int sy, int w, int h);
    image*  draw(image* obj, int x1, int y1, int x2, int y2, int dx1, int dy1, int dx2, int dy2);

    image*  line(long x1, long y1, long x2, long y2, uint color);
    image*  outline(int x1, int y1, int x2, int y2, uint color);
    image*  block(int x1, int y1, int x2, int y2, uint color);
    image*  circle(int xc, int yc, int radius, uint color);
    image*  circlefill(int xc, int yc, int radius, uint color);

    image*  font(int x, int y, const char*, uint color);
    image*  font(int x, int y, float, uint color);
    
    uint    hsv2rgb(int H, int S, int V);
    uint    additive(uint A, uint B);
    uint    blend(uint A, uint B);

    /* DOS */
    void    pal(uint, unsigned char, unsigned char, unsigned char);
    uint    pal(uint);

    /* 3D */
    vec2f   proj3d(vec3f);
    vec2f   getuv(int, int);
    vec2f   get_vecuv(vec2f, vec2f, vec2f, vec2f);
    vec3f   get_vecuv(vec2f, vec3f, vec3f, vec3f);

    image*  translate(vec3f); 
    image*  rotate(int, double);
    image*  identity();
    
    template <class T>
    image*  quad(T _tex, vec5f a, vec5f b, vec5f c, vec5f d);
    
    image*  tri(vec2f, vec2f, vec2f);
    image*  tri(vec3f, vec3f, vec3f);
    image*  tri(image*, vec5f, vec5f, vec5f);
    image*  tri(uint, vec5f, vec5f, vec5f);

    image*  set_texture(image*, vec2f, vec2f, vec2f);
    image*  stencil_outline(uint);

    vec3f   mult(double [4][4], vec3f);
    image*  mult(double mat[4][4]);
};
