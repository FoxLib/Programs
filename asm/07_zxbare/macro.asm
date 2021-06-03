            macro   brk { xchg bx, bx }

; Сначала идет старший байт, потом младший
reg_b       equ [bp+0]
reg_c       equ [bp+1]
reg_d       equ [bp+2]
reg_e       equ [bp+3]
reg_h       equ [bp+4]
reg_l       equ [bp+5]
reg_f       equ [bp+6]
reg_a       equ [bp+7]

; Регистр SP участвует в инструкциях
reg_sp      equ [bp+8]

reg_prime   equ [bp+$40]

; Маски флагов
;               SZ5A3PNCvvvvvvvv
SZHXY       equ 1101000000000111b
SZHCXY      equ 1101000100000110b
