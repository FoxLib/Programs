
            org     $8000

            dec     (hl)
            ld      (hl), $23
            dec     sp
            inc     sp
            ld      (hl), d
            ld      sp, $1234
            ld      a, (de)
