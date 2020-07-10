object3d::object3d() {
    construct();
}

void object3d::construct() {

    mtl_id = 0;
    name[0] = 0;

    cnt_vertex = 0;
    cnt_texcoord = 0;
    cnt_normals = 0;
    cnt_faces = 0;

    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
        mat[i][j] = 0.0;
}
