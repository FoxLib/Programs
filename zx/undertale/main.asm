
        org     $5b00

        di

        ; Очистка атрибутной области
; ----------------------------------------------------------------------
        ld      bc, $0080
        ld      hl, $3838
        ld      sp, $5b00
CLS1:   push    hl
        djnz    CLS1
        ld      b, c
CLS2:   push    hl
        djnz    CLS2
        ld      sp, $ffff
; ----------------------------------------------------------------------

        ld      hl, SCR1
        ld      de, decompress
        call    dzx0

        ; Перенос байта
        ld      hl, decompress
        ld      de, $4003
        ld      a, 8
DRAW2:  ex      af, af'
        ld      a, 8
DRAW1:  ld      bc, 25
        ldir
        ld      bc, 0x00e7       ; Y++
        ex      de, hl
        add     hl, bc
        ex      de, hl
        dec     a
        jr      nz, DRAW1

        ld      bc, -8*256+32
        ex      de, hl
        add     hl, bc
        ex      de, hl

        ex      af, af'
        dec     a
        jr      nz, DRAW2



        jr      $


; ----------------------------------------------------------------------
; Компрессированные данные со скринами (около 12 кб)
; ----------------------------------------------------------------------

        include "dzx0_standard.asm"

SCR1:   incbin  "screen/screen1.bin"
SCR2:   incbin  "screen/screen2.bin"
SCR3:   incbin  "screen/screen3.bin"
SCR4:   incbin  "screen/screen4.bin"
SCR5:   incbin  "screen/screen5.bin"
SCR6:   incbin  "screen/screen6.bin"
SCR7:   incbin  "screen/screen7.bin"
SCR8:   incbin  "screen/screen8.bin"
SCR9:   incbin  "screen/screen9.bin"

decompress:
