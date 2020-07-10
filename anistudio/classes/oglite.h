class oglite {

public:

    int    width;
    int    height;
    char*  name;
    GLuint window;

    oglite(int, int, const char*);
    void    init(int, char**);
    void    viewport(int, int);
    void    projmatrix();
    char*   load_text(const char*);
    GLuint  load_shader(const char*, const char*);
    GLuint  load_texture(GLuint*, GLuint, GLuint);
};
