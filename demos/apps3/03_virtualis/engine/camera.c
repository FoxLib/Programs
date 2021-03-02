// Начало инициализациия камеры в пространстве
void camera_init_rotate() {
    
    glLoadIdentity();
    glRotatef(camera_x, -1.0, 0.0, 0.0); // Развенуть камеру по оси x
    glRotatef(camera_y, 0.0, -1.0, 0.0); // Развенуть камеру по оси Y    
    
}
