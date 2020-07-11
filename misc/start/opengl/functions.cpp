void MainIdleFunc() {
    // ...
}

void ReshapeWindow(int width, int height) {
    
    glViewport(0, 0, width, height);
    
    // Матрица проекции
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.01, 1000);

    // Матрица модели
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.0, 1.0, -1.0);
}

void MainDisplay() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBegin(GL_TRIANGLES);
        
        glVertex3f( 0.0,  1.0, 15.5);
        glVertex3f( 1.0, -1.0, 2.5);
        glVertex3f(-1.0, -1.0, 2.5);
        
    glEnd();

    glutSwapBuffers();
    
    // glutPostRedisplay();
}

// Событие таймера 1/60
void EventFPS(int m) {
    
    MainDisplay();
    glutTimerFunc(16, & EventFPS, m + 1);
}
