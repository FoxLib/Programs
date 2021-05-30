
            org     $5c00

            ld      hl, $4002
            ld      c, 24
l2:         ld      b, 0
l1:         ld      a, (hl)
            xor     255
            ld      (hl), a
            inc     hl
            dec     b
            jr      nz, l1
            dec     c
            jr      nz, l2
            jr      $
