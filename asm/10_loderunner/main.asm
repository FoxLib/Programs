
include "macro.asm"

        org     100h

        mov     ax, $0013
        int     10h
        call    set_palette

        mov     bp, level_01
@@:     call    refresh
        add     word [scroll_x], 1
        jmp     @b

        int     20h

; ----------------------------------------------------------------------
include "proc.asm"
include "imgdata.asm"

; Карта уровней
include "levels/01.asm"

; Разные временные переменные
include "global.asm"
