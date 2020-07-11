
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>

// Локальные заголовки
#include "libs/jpegload.h"

GLfloat timelapse = 0.0;

// Массив с указателями на текстуру
GLuint  texture[256];
struct  jpeg_file_struct TMP;

// Функция изменения размера окна
void reshape_gl_window(int w, int h) {

    h = h > 0 ? h : 1;

    // Установить вид (0,0,w,h) - новый размер окна
    glViewport(0, 0, w, h);

    // Переключиь в матрицу проекции
    glMatrixMode(GL_PROJECTION);

    // Загрузить единичную матрицу
    glLoadIdentity();

    // Загрузить матрицу перспективы (дальность рисовки 0.01 - 10000)
    // Соотношение зависит от заданных (w, h)

    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.01, 10000);

    // Переключаем в режим матрицы моделей
    glMatrixMode(GL_MODELVIEW);

}

// Общая функция отображения графики
void display_func() {

    int i, j;

    // Очистить Z-буфер и цветовой буфер
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Доступные точки для куба (их 8), каждая по 3 координаты (x,y,z)
    GLfloat cube_vertex[ 8 ][ 3 ] = {

        {-1.0, -1.0,  1.0},   /* левая верхняя точка (низ) */
        { 1.0, -1.0,  1.0},   /* правая верхняя точка (низ)  */
        { 1.0, -1.0, -1.0},   /* правая нижняя точка (низ)  */
        {-1.0, -1.0, -1.0},   /* левая нижняя точка (низ)  */
        {-1.0,  1.0,  1.0},   /* левая верхняя точка (верх) */
        { 1.0,  1.0,  1.0},   /* правая верхняя точка (верх) */
        { 1.0,  1.0, -1.0},   /* правая нижняя точка (верх)*/
        {-1.0,  1.0, -1.0}    /* левая нижняя точка (верх)*/

    };

    // Указываем номера точек для 6 граней
    int cube_faces[ 6 ][ 4 ] = {
        
        {0, 1, 2, 3},   // Нижняя
        {4, 5, 6, 7},   // Верхняя
        {0, 4, 7, 3},   // Левая
        {1, 5, 6, 2},   // Правая
        {2, 6, 7, 3},   // Задняя        
        {0, 4, 5, 1}    // Передняя

    };
 
    // Цвет каждой грани
    GLfloat cube_colors[ 6 ][ 3 ] = {

        {0.0, 0.0, 0.5},
        {0.0, 0.5, 0.0},
        {0.5, 0.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0}

    };

    // ----------------------
    // Загрузка единичной матрицы в матрицу вида
            glLoadIdentity();    
    /* 3 */ glTranslatef(0.0, 0.0, -4.0);
    /* 2 */ glRotatef(timelapse, 0.0, 1.0, 0.0);
    /* 1 */ glRotatef(timelapse, 1.0, 0.0, 0.0);
    // ----------------------

    // Отрисовка каждого прямоугольника
    for (i = 0; i < 6; i++) {

        // Назначается цвет каждому треугольнику
//        glColor3f(cube_colors[i][0], cube_colors[i][1], cube_colors[i][2]);
    
        // Рисуем прямоугольным полигоном
        glBegin(GL_QUADS);

        // glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, texture[0] );        

        for (j = 0; j < 4; j++) {

            // Получим номер точки для прямоугольника j=(0,1,2,3)
            // Номер прямоугольника --> i
 
            int vn = cube_faces[ i ][ j ];

            GLfloat u = j == 1 || j == 2 ? 1.0 : 0.0;
            GLfloat v = j >= 2 ? 1.0 : 0.0;
            
            glTexCoord2f( u, v );

            glVertex3f(
                cube_vertex[ vn ][ 0 ], // x
                cube_vertex[ vn ][ 1 ], // y
                cube_vertex[ vn ][ 2 ]  // z
            );

        }

        glEnd();

    }

    // Обмен буферами (задний <-> передний)
    glutSwapBuffers();

    timelapse += 0.05;

}

// Загрузка текстуры напрямик из jpeg-файла (который находится в памяти)
void load_texture(struct jpeg_file_struct image, int texture_id) {

    // Текстура создается в памяти, назначается GLuint
    glGenTextures(1, (GLuint *) &texture[ texture_id ]);
    
    // Выбираем эту текстуру, работаем с 2D: x, y.
    glBindTexture(GL_TEXTURE_2D, texture[ texture_id ]);

    // Линейная интерполяция для приближенных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // GL_NEAREST

    // Линейная интерполяция для удаленных объектов
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    /**
     * @url https://www.opengl.org/sdk/docs/man2/xhtml/gluBuild2DMipmaps.xml
     *
     * GLint gluBuild2DMipmaps(
     * GLenum       target,
     * GLint        internalFormat,
     * GLsizei      width,
     * GLsizei      height,
     * GLenum       format,
     * GLenum       type,
     * const void * data);
     */

     gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, (void *)image.data);
};

// Первичная инициализация 
void init() {
        
    // Включение режима поддержки возможностей дисплея
    // GLUT_RGBA    32-х битный цвет с прозрачностью
    // GLUT_DOUBLE  двойная буферизация
    // GLUT_ALPHA   поддержка альфа-канала
    // GLUT_DEPTH   поддержка z-буфера

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    // Размер экрана
    glutInitWindowSize(1024, 768);

    // Создать окно
    int window = glutCreateWindow("The Virtualis");
 
    // Включить Z-буфер
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Параметр: при очистке экрана использовать этот цвет (r=0, g=0.2, b=0.4, a=0)
    glClearColor(0.0f, 0.2f, 0.4f, 0.0f);

    // Включение функции Depth Test (режим GL_LESS) и значения, которым
    // будет заполнен Z-буфер

    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);

    // Назначение обработчика при изменений размера окна
    glutReshapeFunc(& reshape_gl_window );

    // Обработчик отрисовщика окна
    // При первом вызове, эта функция вызывается

    glutDisplayFunc(& display_func );

    // Обработчик, когда ничего не происходит (60 раз в секунду)
    glutIdleFunc(& display_func );
}

// Основное окно запуска
int main(int argc, char* argv[]) {

    // Передача параметров инициализатору glut
    glutInit(& argc, argv);
    
    init();
        
    // Загружать текстуру надо позднее
    load_texture( jpegload("images/texture.jpg"), 0 );
    
    // Запуск обработки, бесконечный цикл
    glutMainLoop();

    return 0;
}
