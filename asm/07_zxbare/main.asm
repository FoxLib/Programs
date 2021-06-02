            macro   brk { xchg bx, bx }

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

            brk

            ; Главная дешифрация адреса
@@:         lodsb
            mov     bl, al
            mov     bh, 0
            call    word [table_main+bx]
            jmp     @b

; Инструкция NOP делает вообще ничего
instr_nop:  ret

include     "utils.asm"
include     "table.main.asm"
