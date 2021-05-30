
            org     $5c00

            ; H
            ld      hl, $4000
            ld      de, $3D00+(48h-20h)*8
            ld      b, 8
l1:         ld      a, (de)
            ld      (hl), a
            inc     de
            inc     h
            dec     b
            jr      nz, l1

            ; E
            ld      hl, $4001
            ld      de, $3D00+(45h-20h)*8
            ld      b, 8
l2:         ld      a, (de)
            ld      (hl), a
            inc     de
            inc     h
            dec     b
            jr      nz, l2


            ; L
            ld      hl, $4002
            ld      de, $3D00+(4Ch-20h)*8
            ld      b, 8
l3:         ld      a, (de)
            ld      (hl), a
            inc     de
            inc     h
            dec     b
            jr      nz, l3

            ; L
            ld      hl, $4003
            ld      de, $3D00+(4ch-20h)*8
            ld      b, 8
l4:         ld      a, (de)
            ld      (hl), a
            inc     de
            inc     h
            dec     b
            jr      nz, l4

            ; O
            ld      hl, $4004
            ld      de, $3D00+(4Fh-20h)*8
            ld      b, 8
l5:         ld      a, (de)
            ld      (hl), a
            inc     de
            inc     h
            dec     b
            jr      nz, l5

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
