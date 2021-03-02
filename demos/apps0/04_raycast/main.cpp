
/** Шаблон для рейтресинга */
int main(int argc, char* argv[]) {

    int     i, j;
    image   img(960, 540);

    context3d   d3(& img);

    // Общее освещение подчиняется точке (0,0,0)
    vec3f ambient_light = d3.ray->normal( {1.0, 1.0, -1.0} );
    vec3f point_light   = {0.0, 0.0, -2.0};

    // Универсальный шаблон для рисования
    for (i = d3.bottom; i < d3.top; i++) {
        for (j = d3.left; j < d3.right; j++) {

            uint color;

            // Вектор направления
            vec3f dir = d3.viewport(j, i);

            // Расчет дистанции до сферы
            TCollide r = d3.ray->cast_sphere(dir, { 0.0, 0.0, 4.0 }, 1.00);

            // Есть попадание на сферу
            if (r.cast) {
                
                vec3f reflect = d3.ray->reflect(r, point_light);        // Получение отраженного луча на сфере
                float sp = d3.ray->specular( reflect, r.direct, 20.0 ); // Вычисление отражения (specular)
                float am = d3.ray->dot( ambient_light, r.normal );      // Расчет силы света                
                float dc = (am + sp) / 2.0;                             // Итоговый цвет                
                color    = d3.ray->rgb( d3.ray->vec3(dc, dc, dc) );     // Цвет

            } else {
                color = 0;
            }

            // Рисование точки
            d3.pset(j, i, color);
        }
    }

    app.update(d3.canvas);
    app.loop();

    return 0;
}
