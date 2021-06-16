
include "macro.asm"

        org     100h

        mov     ax, $0013
        int     10h

        ; Перехват таймера
        call    int8_timer

        mov     ax, cs
        add     ax, $1000
        mov     [seg_fb], ax        ; Установка фреймбуфера
        call    set_palette

        ; Пока что ничего тут не делается
@@:     jmp     @b
        int     20h

; Обработка событий каждые 1/18 секунды
; ----------------------------------------------------------------------
clock:  int     255
        pusha

        ; Скорость обновления (каждые CL=16 кадра), CH=4 тайла
        mov     bx, ladder_phase
        mov     cx, 0x0410
        call    update_tile_animation

        ; Обновление анимации золота на уровне
        mov     bx, gold_phase
        mov     cx, 0x0404
        call    update_tile_animation

        ; Перерисовка уровня
        call    refresh
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
