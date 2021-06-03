            macro   brk { xchg bx, bx }

; Сначала идет старший байт, потом младший
reg_bc      equ 0
reg_de      equ 2
reg_hl      equ 4
reg_af      equ 6
reg_a       equ 7

; Регистр SP участвует в инструкциях
reg_sp      equ 8
reg_ix      equ 10
reg_iy      equ 12

reg_prime   equ $20

; Маски флагов
;               SZ5A3PNCvvvvvvvv
SZHXY       equ 1101000000000111b
SZHCXY      equ 1101000100000110b
