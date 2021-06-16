
include "macro.asm"

        org     100h

        mov     ax, $0013
        int     10h

        ; Перехват таймера
        call    int8_timer

        mov     ax, cs
        add     ax, $1000
        mov     [seg_fb], ax        ; Установка фреймбуфера
        ;mov     [seg_fb], word $a000
        call    set_palette

        ; Пока что ничего тут не делается
@@:     jmp     @b
        int     20h

; Обработка событий каждые 1/18 секунды
; ----------------------------------------------------------------------
clock:  int     255
        pusha

        ; Обновление анимации лестницы
        mov     cx, 0x0410
        mov     bx, ladder_phase
        mov     dx, anim_color_ladder
        call    update_tile_animation
        mov     [current_palette + 3*4 + 1], al ; 3-лестница

        ; Обновление анимации золота на уровне
        mov     cx, 0x0404
        mov     bx, gold_phase
        mov     dx, anim_color_gold
        call    update_tile_animation
        mov     [current_palette + 4*4 + 2], al ; 4-золото

        ; Перерисовка уровня
        call    refresh
        call    redraw_sprite
        call    flip

        ;add     word [scroll_x], 1

        popa
        iret

; ----------------------------------------------------------------------
include "proc.asm"
include "imgdata.asm"

; Карта уровней
include "levels/01.asm"

; Разные временные переменные
include "global.asm"
