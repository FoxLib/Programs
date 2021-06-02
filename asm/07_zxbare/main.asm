            include "macro.asm"

            cli
            cld
            mov     ax, cs
            mov     ds, ax
            add     ax, 0x1000
            call    load_rom
            mov     ds, ax          ; 64Кб область памяти
            xor     ax, ax
            mov     ss, ax          ; Стек и данные для стека
            mov     ax, 0xa000
            mov     es, ax          ; Видео-область

            mov     bp, $7000       ; Область данных
            mov     si, $8000       ; Регистр PC
            ; ax, bx, cx, dx, di (свободные)

            ; DL=количество циклов инструкции

            brk

            ; Главная дешифрация адреса
INSTR:      dec     [cs:delayed]             ; Включение DI/EI после инструкции
            jns      @f
            mov     [cs:delayed], 0xff
@@:         xor     dx, dx
            inc     [cs:reg_r7f]
            lodsb
            mov     bh, 0
            mov     bl, al
            mov     dl, [cs:cycles_main+bx]
            add     bx, bx
            call    word [cs:table_main+bx]
            jmp     INSTR

; Инструкция NOP делает вообще ничего
instr_nop:  ret


; Системные переменные
reg_r7f             db      0       ; Младшие биты R
reg_r8              db      0       ; Старший бит R
reg_i               db      0
reg_imode           db      0
reg_iff1            db      0
reg_iff2            db      0
reg_halted          db      0
delayed             db      0xff
delayed_type        db      0       ; Биты 0=EI, 1=DI

; ----------------------------------------------------------------------
include     "utils.asm"
include     "table.main.asm"
