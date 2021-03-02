
// z(i+1) = z(i)^2 + C[r,i]
int bailout(double r, double i) {
    
    int k;
    double zi = 0.0, zr = 0.0;
    
    for (k = 0; k < 256; k++) {
        
        if (zr*zr + zi*zi > 4.0) {
            return k;
        }
        
        double r_ = zr*zr - zi*zi + r;
        double i_ = 2*zi*zr + i;
        
        zr = r_; zi = i_;            
    }
    
    return -1;
}

void draw(double x1, double y1, double s) {
    
    int x, y;    
    int w2 = SCREEN_H/2;
        
    for (y = -w2; y < w2; y++) {
        for (x = -w2; x < w2; x++) {
            
            double x_ = (double)(x)/(double)SCREEN_H*s + x1; // (x - SCREEN_W*0.5) / SCREEN_H * sx + ox;
            double y_ = (double)(y)/(double)SCREEN_H*s + y1; // (y - SCREEN_H*0.5) / SCREEN_H * sy + oy;

            int c = bailout(x_, y_);
            
            if (c == -1) c = 0x00ff00; else c = c << 8;
            pset(w2 + x, w2 + y, c);
        }
    }
    
}
