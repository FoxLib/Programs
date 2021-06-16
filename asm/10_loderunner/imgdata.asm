
; Для различной подсветки элементов
; Младший байт - счетчик, старший байт - номер тайла
ladder_phase        dw      0
gold_phase          dw      0

tile_convert:

        db      24, 2                   ; 0 Пустота
        db      25, 1                   ; 1 Кирпичи
        db      27, 3                   ; 2 Лестница
        db      31, 4                   ; 3 Золото
        db      28, 3                   ; 4 Веревка
        db      26, 1                   ; 5 Железный блок

; Бинарные данные
; ----------------------------------------------------------------------
tilemap:        file "img/tilemap.bin"      ; 4kb
                file "img/sprites.bin"      ; 4kb

; ----------------------------------------------------------------------
current_palette:

        db 0x16, 0x12, 0x0F, 0x27           ; 0 Расцветка игрока
        db 0x0F, 0x17, 0x07, 0x26           ; 1 Тайлы уровня
        db 0x0F, 0x0F, 0x0F, 0x0F           ; 2 Пустота
        db 0x0F, 0x16, 0x30, 0x00           ; 3 Лестница
        db 0x0F, 0x28, 0x00, 0x38           ; 4 Золотишко
        ;        0x38  0x28  0x00
        db 0x30, 0x16, 0x0F, 0x2B           ; 5 Враг

; Анимация золота
anim_color_gold     db 0x00, 0x10, 0x20, 0x10
anim_color_ladder   db 0x06, 0x16, 0x26, 0x16

; Палитра NES
; ----------------------------------------------------------------------

palette:

        dd 0x757575, 0x271b8f, 0x0000ab, 0x47009f ; 00
        dd 0x8f0077, 0xab0013, 0xa70000, 0x7f0b00 ; 00
        dd 0x432f00, 0x004700, 0x005100, 0x003f17 ; 00
        dd 0x1b3f5f, 0x000000, 0x000000, 0x000000 ; 00
        dd 0xbcbcbc, 0x0073ef, 0x233bef, 0x8300f3 ; 10
        dd 0xbf00bf, 0xe7005b, 0xdb2b00, 0xcb4f0f ; 10
        dd 0x8b7300, 0x009700, 0x00ab00, 0x00933b ; 10
        dd 0x00838b, 0x000000, 0x000000, 0x000000 ; 10
        dd 0xffffff, 0x3fbfff, 0x5f97ff, 0xa78bfd ; 20
        dd 0xf77bff, 0xff77b7, 0xff7763, 0xff9b3b ; 20
        dd 0xf3bf3f, 0x83d313, 0x4fdf4b, 0x58f898 ; 20
        dd 0x00ebdb, 0x000000, 0x000000, 0x000000 ; 20
        dd 0xffffff, 0xabe7ff, 0xc7d7ff, 0xd7cbff ; 30
        dd 0xffc7ff, 0xffc7db, 0xffbfb3, 0xffdbab ; 30
        dd 0xffe7a3, 0xe3ffa3, 0xabf3bf, 0xb3ffcf ; 30
        dd 0x9ffff3, 0x000000, 0x000000, 0x000000 ; 30
