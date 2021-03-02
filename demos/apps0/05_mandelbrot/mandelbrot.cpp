int i, j;
float* m = (float*)malloc(1920*1080*sizeof(float));
for (i = 0; i < 1920*1080; i++) m[i] = 0.0;

float stars[4096][3];

for (i = 0; i < 4096; i++) {

    stars[i][0] = rand() % 1920;
    stars[i][1] = rand() % 1080;
    stars[i][2] = rand() % 255;
}

int id = 0;
while (id < 1000) {

    // Источник
    for (i = 0; i < 1920; i++) {

        m[1078*1920 + i] = (float)(rand()%1000)/1000.0;
        m[1079*1920 + i] = (float)(rand()%1000)/1000.0;
    }

    // Смешивание
    for (i = 1079; i >= 0; i--)
    for (j = 0; j < 1920; j++) {

        uint pivot = 1920*(i+1) + j;

        // Для создания "языков пламени"
        float r = (float)(rand()%1000)/1000.0*0.25;

        float a0 = m[pivot - 1920] * (1.0 - 2.0*r);
        float a1 = m[pivot + 1] * (1.0 + r);
        float a2 = m[pivot + 0];
        float a3 = m[pivot - 1] * (1.0 + r);

        // Функция распределения энергии
        float t = (a0 + a1 + a2 + a3) / 4.01  - 0.0002;

        if (t > 1.0) t = 1.0;
        else if (t < 0.0) t = 0.0;

        m[pivot - 1920] = t;
    }

    // Рисование
    for (i = 0; i < 1080; i++)
    for (j = 0; j < 1920; j++) {

        float energy = m[1920*i + j];

        float h = log(1.2 + 12.0*energy)*30.0;
        float v = 8.0*energy*255.0;
        float s = 0;

        img.pset(j, i, app.hsv2rgb(h < 60 ? h : 0, 100 - s, v));
    }

    // Наложить звездное небо
    for (i = 0; i < 4096; i++) {

        int x = stars[i][0];
        int y = stars[i][1];

        uint star = app.hsv2rgb(0, 0, stars[i][2]);
        uint back = img.point(x, y);

        img.pset(x, y, app.additive(star, back));
    }

    // нужно оптимизировать тут код вообще
    img.draw(& label, 32, 0);

    // Показать на экране
    app.bitblt(&img, 0, 0, 960, 540)->flip();
    app.events();

    // Сохранение нового фрейма
    img.save(& jpg); char fn[128]; sprintf(fn, "build/%06d.jpeg", id++); jpg.save(fn, 80);
}
