
vec2f sinrad(float f, vec2f radius) {
    return {radius.x * cos(f), radius.y * sin(f)};
}

// Рисование стола
uint paint_table(uint id, uint color, vec2f radius) {    

    vec5f vtx[4];

    int i;
    int segments = 16;

    vec2f s1[3][ segments ];

    // Инициализировать сегменты стола
    for (i = 0; i < segments; i++) {

        double s = 3.14 / (segments - 1);
        double k = i * s;

        s1[0][i] = sinrad(k, radius);
        s1[1][i] = sinrad(k, {radius.x * 0.65, radius.y * 0.65});
        s1[1][i].y -= 0.25;
    }

    // Отрисовка самого стола
    dos.stencil_id = id++;
    for (i = 0; i < segments - 1; i++) {

        vtx[0] = { s1[1][i  ].x, 0.00, s1[1][i  ].y, 0, 0 };
        vtx[1] = { s1[0][i  ].x, 0.00, s1[0][i  ].y, 1, 0 };
        vtx[2] = { s1[0][i+1].x, 0.00, s1[0][i+1].y, 0, 1 };
        vtx[3] = { s1[1][i+1].x, 0.00, s1[1][i+1].y, 1, 1 };

        dos.quad(color, vtx[0], vtx[1], vtx[2], vtx[3]);
    }
    
    // Отрисовка низовой части стола
    dos.stencil_id = id++;
    for (i = 0; i < segments - 1; i++) {

        vtx[0] = { s1[1][i  ].x,  0.00, s1[1][i  ].y, 0, 0 };
        vtx[1] = { s1[1][i+1].x,  0.00, s1[1][i+1].y, 1, 0 };
        vtx[2] = { s1[1][i+1].x, -0.50, s1[1][i+1].y, 0, 1 };
        vtx[3] = { s1[1][i  ].x, -0.50, s1[1][i  ].y, 1, 1 };

        dos.quad(color, vtx[0], vtx[1], vtx[2], vtx[3]);
    }

    dos.stencil_id = id++;

    dos.quad(color, // Правый бок
        { s1[0][0].x, -0.50, s1[0][0].y, 0, 0 },
        { s1[0][0].x,  0.00, s1[0][0].y, 1, 0 },
        { s1[1][0].x,  0.00, s1[1][0].y, 0, 1 },
        { s1[1][0].x, -0.50, s1[1][0].y, 1, 1 }
    );

    dos.quad(color, // Левый бок
        { s1[0][segments-1].x, -0.50, s1[0][segments-1].y, 0, 0 },
        { s1[0][segments-1].x,  0.00, s1[0][segments-1].y, 1, 0 },
        { s1[1][segments-1].x,  0.00, s1[1][segments-1].y, 0, 1 },
        { s1[1][segments-1].x, -0.50, s1[1][segments-1].y, 1, 1 }
    );
           
    return id;
}

void paint_block(vec3f coord, vec3f scale, uint color[6]) {

    vec5f vt[8] = {

        {coord.x,           coord.y,            coord.z, 0, 0},
        {coord.x + scale.x, coord.y,            coord.z, 0, 0},
        {coord.x + scale.x, coord.y + scale.y,  coord.z, 0, 0},
        {coord.x,           coord.y + scale.y,  coord.z, 0, 0},
        {coord.x,           coord.y,            coord.z + scale.z, 0, 0},
        {coord.x + scale.x, coord.y,            coord.z + scale.z, 0, 0},
        {coord.x + scale.x, coord.y + scale.y,  coord.z + scale.z, 0, 0},
        {coord.x,           coord.y + scale.y,  coord.z + scale.z, 0, 0},

    };

    int faces[6][4] = {
        {0, 1, 2, 3}, // Передняя грань
        {4, 5, 6, 7}, // Задняя
        {2, 6, 7, 3}, // Верхняя
        {1, 5, 6, 2}, // Правая
        {0, 1, 5, 4}, // Нижняя
        {0, 4, 7, 3}, // Левая
    };

    for (int i = 0; i < 6; i++) {

        if (color[i]) { 

            dos.quad(color[i], vt[ faces[i][0] ], vt[ faces[i][1] ], vt[ faces[i][2] ], vt[ faces[i][3] ]);
        }
    }

    

}
