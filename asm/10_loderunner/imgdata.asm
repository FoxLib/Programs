
; Ссылки на блоки с изображениями
; ----------------------------------------------------------------------
img_table:

        dw      sp_00_zero
        dw      sp_01_block             ; Кирпичи
        dw      sp_02_ladder            ; Лестница
        dw      sp_03_gold              ; Золото
        dw      sp_04_rope              ; Веревка

; Бинарные данные
; ----------------------------------------------------------------------
sp_00_zero:     times 256 db 0x3f       ; Черный цвет
sp_01_block     file "img/01_brick.bin"
sp_02_ladder    file "img/02_ladder.bin"
sp_03_gold      file "img/03_gold.bin"
sp_04_rope      file "img/04_rope.bin"

; Палитра NES
; ----------------------------------------------------------------------

palette:

        dd 0x757575, 0x271b8f, 0x0000ab, 0x47009f
        dd 0x8f0077, 0xab0013, 0xa70000, 0x7f0b00
        dd 0x432f00, 0x004700, 0x005100, 0x003f17
        dd 0x1b3f5f, 0x000000, 0x000000, 0x000000
        dd 0xbcbcbc, 0x0073ef, 0x233bef, 0x8300f3
        dd 0xbf00bf, 0xe7005b, 0xdb2b00, 0xcb4f0f
        dd 0x8b7300, 0x009700, 0x00ab00, 0x00933b
        dd 0x00838b, 0x000000, 0x000000, 0x000000
        dd 0xffffff, 0x3fbfff, 0x5f97ff, 0xa78bfd
        dd 0xf77bff, 0xff77b7, 0xff7763, 0xff9b3b
        dd 0xf3bf3f, 0x83d313, 0x4fdf4b, 0x58f898
        dd 0x00ebdb, 0x000000, 0x000000, 0x000000
        dd 0xffffff, 0xabe7ff, 0xc7d7ff, 0xd7cbff
        dd 0xffc7ff, 0xffc7db, 0xffbfb3, 0xffdbab
        dd 0xffe7a3, 0xe3ffa3, 0xabf3bf, 0xb3ffcf
        dd 0x9ffff3, 0x000000, 0x000000, 0x000000
