
            org     $5c00

            ; H
            ld      hl, $4000
            ld      de, $3D00+(48h-20h)*8
            ld      b, 8

            ld      de, hello

            ; LD (temp), DE
chr:        ld      hl, temp
            ld      (hl), e
            inc     hl
            ld      (hl), d
            ld      a, (de)
            and     a
            jr      z, next

            sub     $20
            ld      d, 0
            ld      e, a
            ld      b, 3
m1:         ld      a, e
            add     a
            ld      e, a
            ld      a, d
            adc     a, 0
            ld      d, a
            dec     b
            jr      nz, m1
            ld      a, d
            add     $3d
            ld      d, a

            ld      hl, $4000
            ld      c, 4

            ; Пропечатать символ
            ld      b, 8
l1:         ld      a, (de)
            ld      (hl), a
            inc     de
            inc     h
            dec     b
            jr      nz, l1
            ld      a, h
            sub     $8
            ld      h, a
            inc     hl

            ld      hl, temp
            ld      e, (hl)
            inc     hl
            ld      d, (hl)
            inc     de
            jr      chr

next:       ld      hl, $4000
            ld      de, $4000
            ld      c, 24
l7:         ld      b, 0
l6:         ld      a, (de)
            xor     255
            ld      (hl), a
            inc     de
            inc     hl
            dec     b
            jr      nz, l6
            dec     c
            jr      nz, l7
            jr      next

hello:      db "Hello",0
temp:       dw 0
