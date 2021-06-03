
            org     $8000

            ld      (de), a
            ld      hl, ($1234)
            ld      (hl), $23
            dec     sp
            inc     sp
            ld      (hl), d
            ld      sp, $1234
            ld      a, (de)
