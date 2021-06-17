; ----------------------------------------------------------------------
; Обработка движений персонажа
; ----------------------------------------------------------------------

player_move:

            ; Кнопка "Вправо"
            cmp     [keyboard+0x4D], byte 0
            jne     move_right

            ; Кнопка "Влево"
            cmp     [keyboard+0x4B], byte 0
            jne     move_left

            mov     [player_anim_state], 0      ; Сброс анимации бега
            mov     [sprites+3], byte 18        ; IDLE
            ret

; Движение вправо
; ----------------------------------------------------------------------
move_right:

            call    anim_run
            mov     [sprites+3], al

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
            or      [sprites+4], byte 0x10
            ret

; Движение влево
; ----------------------------------------------------------------------
move_left:

            call    anim_run
            mov     [sprites+3], al

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
            and     [sprites+4], byte 0xEF  ; Очистить бит 4
            ret

; Анимация бега для персонажа
anim_run:   mov     al, [player_anim_state]
            inc     al
            cmp     al, 4
            jne     @f
            mov     al, 0
@@:         mov     [player_anim_state], al
            ret
