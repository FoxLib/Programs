class TScene {

protected:

    std::vector<TCamera> Cameras;           // Список камер
    std::vector<TObject> Meshes;            // Объекты в памяти
    TCamera              ActiveCamera;

public:

    /* Создать камеру */
    TCamera* CreateCamera() {

        TCamera Camera;
        Cameras.insert(Cameras.end(), Camera);
        ActiveCamera = Camera;
        return & Cameras.at( Cameras.size() - 1 );
    }

    /* Добавить объект на сцену */
    void AddMesh(const char* filename) {

    }


};
