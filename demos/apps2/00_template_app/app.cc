class App {
protected:
    SDL2framework* win;
public:

    App(SDL2framework* _win) {
        win = _win;
    }

    int update() {

        for (int y = 0; y < win->h(); y++)
        for (int x= 0; x < win->w(); x++)
            win->pset(x, y, x*y);

        return 1;
    }
};
