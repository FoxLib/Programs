; ----------------------------------------------------------------------
; Обработка движений персонажа
; ----------------------------------------------------------------------

player_move:

            mov     bx, keyboard

            ; Кнопка "Вправо"
            cmp     [bx+0x4D], byte 0
            jne     move_right

            ; Кнопка "Влево"
            cmp     [bx+0x4B], byte 0
            jne     move_left

            ; Вверх по лестнице
            cmp     [bx+0x48], byte 0
            jne     move_up

.idle:      ; Персонаж стоит находится на лестнице?
            cmp     [player_on_ladder], 0
            jne     .exit

            mov     [player_anim_state], 0      ; Сброс анимации бега
            mov     [sprites+3], byte 18        ; IDLE
.exit:
            ret

; Движение вправо
; ----------------------------------------------------------------------
move_right:
int3
            ; Получение текущего блока, где стоит персонаж
            call    get_player_xy

            ; Сначала проверим, что впереди ничего нет
            cmp     [bx+1], byte TILE_BRICK
            je      .stop
            cmp     [bx+1], byte TILE_IRON
            je      .stop

            ; Если же сейчас по Y[3:0] != 0, то проверить блок снизу
            mov     al, [sprites+2]
            and     al, 15
            je      .exacty
            ; -- если тут 0, то проверяем точно
            ; -- если <4, то при наличии блока под


.exacty:
            ; Включение анимации бега
            call    anim_run
            mov     [sprites+3], al

            ; При любом движении слезает с лестницы
            mov     [player_on_ladder], 0

            ; Сдвиг спрайта вправо
            mov     dx, [scroll_x]
            mov     ax, word [sprites+0]
            add     ax, 3
            cmp     ax, MIDDLE_X
            jb      .end

            ; Превышены пределы скроллинга для уровня?
            mov     bx, [level_width]
            add     dx, 3
            cmp     dx, bx
            jb      .dox
            sub     dx, 3
            jmp     .end
.dox:       mov     ax, MIDDLE_X                ; Оставить персонажа на месте
.end:       mov     [scroll_x], dx
            mov     [sprites+0], ax
.stop:      or      [sprites+4], byte 0x10
            ret

; Движение влево
; ----------------------------------------------------------------------
move_left:

            ; Проверка на наличие предметов впереди
            call    get_player_xy
            cmp     [bx], byte TILE_BRICK
            je      .stop
            cmp     [bx], byte TILE_IRON
            je      .stop

            call    anim_run
            mov     [sprites+3], al

            ; При любом движении слезает с лестницы
            mov     [player_on_ladder], 0

            ; Сдвиг спрайта вправо
            mov     dx, [scroll_x]
            mov     ax, word [sprites+0]
            sub     ax, 3
            cmp     ax, MIDDLE_X
            jnb     .end

            ; Превышены пределы скроллинга для уровня?
            sub     dx, 3
            jns     .dox
            xor     dx, dx
            jmp     .end
.dox:       mov     ax, MIDDLE_X            ; Оставить персонажа на месте
.end:       mov     [scroll_x], dx
            mov     [sprites+0], ax
.stop:      and     [sprites+4], byte 0xEF  ; Очистить бит 4
            ret

; Анимация бега для персонажа
anim_run:   mov     al, [player_anim_state]
            inc     al
            cmp     al, 4
            jne     @f
            mov     al, 0
@@:         mov     [player_anim_state], al
            ret

; Вверх по лестнице
; ----------------------------------------------------------------------

move_up:    ; Проверка текущего положения

            call    get_player_xy
            cmp     [bx], byte TILE_LADDER
            je      .up
            ret

            ; Путь на лестницу есть
.up:        call    anim_run
            add     al, 8
            mov     [sprites+3], al
            mov     [player_on_ladder], 1

            ; Y--
            sub     [sprites+2], byte 2

            ret
