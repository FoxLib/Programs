SDL_Event  event;

int get_key(SDL_Event event) {
     
    /* Получение ссылки на структуру с данными о нажатой клавише */
    SDL_KeyboardEvent * eventkey = &event.key;
         
    /* Получить скан-код клавиш */
    return eventkey->keysym.scancode;
}

// Таймер
uint Timer(uint interval, void *param) {
    
    SDL_Event     event;
    SDL_UserEvent userevent;
    
    /* Создать новый Event */
    userevent.type  = SDL_USEREVENT;
    userevent.code  = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);    
}

void init_event(uint interval) {
    SDL_AddTimer(interval, Timer, NULL);
}
