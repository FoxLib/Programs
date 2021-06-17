
include "macro.asm"

        org     100h

        cli
        mov     ax, $0013
        int     10h

        ; Переназначить IRQ#1 (Клавиатура)
        call    irq1_set

        ; Перепрограммирование таймера
        ; Модулятор 1193181 / HZ = число
        ; 47727 = 25 герц
        mov     al, 0x6F
        out     $40, al         ; low
        mov     al, 0xBA
        out     $40, al         ; high

        mov     ax, cs
        add     ax, $1000
        mov     [seg_fb], ax        ; Установка фреймбуфера
        call    set_palette
        sti

        ; Главная процедура отлова прерываний
; ----------------------------------------------------------------------
main:   mov     ah, 0
        int     1Ah
        cmp     [clock_time], dx
        je      .next
        mov     [clock_time], dx
        call    clock

.next:  ; Нажатие на клавишу ESC
        cmp     [keyboard+1], byte 0
        jne     exit

        ;int3
        jmp     main

exit:   ; Восстановить таймер
        xor     ax, ax
        out     $40, al
        out     $40, al

        ; Восстановить IRQ#1
        int     20h

; Обработка событий каждые 1/18 секунды
; ----------------------------------------------------------------------
clock:

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

        ; Движения
        call    player_move

        ; Перерисовка уровня
        call    refresh
        call    redraw_sprite
        call    flip

        ;add     word [scroll_x], 1
        ret

; ----------------------------------------------------------------------
include "proc.asm"
include "move.asm"
include "imgdata.asm"

; Карта уровней
include "levels/01.asm"

; Разные временные переменные
include "global.asm"
