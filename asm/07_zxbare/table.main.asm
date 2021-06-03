; https://clrhome.org/table/
table_main:

        ; 00-0f
        dw _nop         ; 00
        dw ldnn         ; 01
        dw ldbca        ; 02
        dw incnn        ; 03
        dw inc8         ; 04
        dw dec8         ; 05
        dw ldr8n        ; 06
        dw _nop         ; 07
        dw _nop         ; 08
        dw _nop         ; 09
        dw ldabc        ; 0A
        dw decnn        ; 0B
        dw inc8         ; 0C
        dw dec8         ; 0D
        dw ldr8n        ; 0E
        dw _nop         ; 0F

        ; 10-1f
        dw _nop         ; 10
        dw ldnn         ; 11
        dw lddea        ; 12
        dw incnn        ; 13
        dw inc8         ; 14
        dw dec8         ; 15
        dw ldr8n        ; 16
        dw _nop         ; 17
        dw _nop         ; 18
        dw _nop         ; 19
        dw ldade        ; 1A
        dw decnn        ; 1B
        dw inc8         ; 1C
        dw dec8         ; 1D
        dw ldr8n        ; 1E
        dw _nop         ; 1F

        ; 20-2f
        dw _nop         ; 20
        dw ldnn         ; 21
        dw ldnnhl       ; 22
        dw incnn        ; 23
        dw inc8         ; 24
        dw dec8         ; 25
        dw ldr8n        ; 26
        dw _nop         ; 27
        dw _nop         ; 28
        dw _nop         ; 29
        dw ldhlnn       ; 2A
        dw decnn        ; 2B
        dw inc8         ; 2C
        dw dec8         ; 2D
        dw ldr8n        ; 2E
        dw _nop         ; 2F

        ; 30-3f
        dw _nop         ; 30
        dw ldnn         ; 31
        dw ldnna        ; 32
        dw incnn        ; 33
        dw inc8         ; 34
        dw dec8         ; 35
        dw ldr8n        ; 36
        dw _nop         ; 37
        dw _nop         ; 38
        dw _nop         ; 39
        dw ldann        ; 3A
        dw decnn        ; 3B
        dw inc8         ; 3C
        dw dec8         ; 3D
        dw ldr8n        ; 3E
        dw _nop         ; 3F

        ; 40-4f
        dw ld88 ; ld b, b
        dw ld88 ; ld b, c
        dw ld88 ; ld b, d
        dw ld88 ; ld b, e
        dw ld88 ; ld b, h
        dw ld88 ; ld b, l
        dw ld88 ; ld b, (hl)
        dw ld88 ; ld b, a
        dw ld88 ; ld c, b
        dw ld88 ; ld c, c
        dw ld88 ; ld c, d
        dw ld88 ; ld c, e
        dw ld88 ; ld c, h
        dw ld88 ; ld c, l
        dw ld88 ; ld c, (hl)
        dw ld88 ; ld c, a
        ; 50-5f
        dw ld88 ; ld d, b
        dw ld88 ; ld d, c
        dw ld88 ; ld d, d
        dw ld88 ; ld d, e
        dw ld88 ; ld d, h
        dw ld88 ; ld d, l
        dw ld88 ; ld d, (hl)
        dw ld88 ; ld d, a
        dw ld88 ; ld e, b
        dw ld88 ; ld e, c
        dw ld88 ; ld e, d
        dw ld88 ; ld e, e
        dw ld88 ; ld e, h
        dw ld88 ; ld e, l
        dw ld88 ; ld e, (hl)
        dw ld88 ; ld e, a
        ; 60-6f
        dw ld88 ; ld h, b
        dw ld88 ; ld h, c
        dw ld88 ; ld h, d
        dw ld88 ; ld h, e
        dw ld88 ; ld h, h
        dw ld88 ; ld h, l
        dw ld88 ; ld h, (hl)
        dw ld88 ; ld h, a
        dw ld88 ; ld l, b
        dw ld88 ; ld l, c
        dw ld88 ; ld l, d
        dw ld88 ; ld l, e
        dw ld88 ; ld l, h
        dw ld88 ; ld l, l
        dw ld88 ; ld l, (hl)
        dw ld88 ; ld l, a
        ; 70-7f
        dw ld88 ; ld (hl), b
        dw ld88 ; ld (hl), c
        dw ld88 ; ld (hl), d
        dw ld88 ; ld (hl), e
        dw ld88 ; ld (hl), h
        dw ld88 ; ld (hl), l
        dw _halt
        dw ld88 ; ld a, a
        dw ld88 ; ld a, b
        dw ld88 ; ld a, c
        dw ld88 ; ld a, d
        dw ld88 ; ld a, e
        dw ld88 ; ld a, h
        dw ld88 ; ld a, l
        dw ld88 ; ld a, (hl)
        dw ld88 ; ld a, a
        ; 80-8f
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        ; 90-9f
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        ; a0-af
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        ; b0-bf
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        ; c0-cf
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        ; d0-df
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        ; e0-ef
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        ; f0-ff
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop
        dw _nop

cycles_main:

        db 4, 10,  7,  6,  4,  4,  7,  4,  4, 11,  7,  6,  4,  4,  7,  4
        db 8, 10,  7,  6,  4,  4,  7,  4, 12, 11,  7,  6,  4,  4,  7,  4
        db 7, 10, 16,  6,  4,  4,  7,  4,  7, 11, 16,  6,  4,  4,  7,  4
        db 7, 10, 13,  6, 11, 11, 10,  4,  7, 11, 13,  6,  4,  4,  7,  4
        db 4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4
        db 4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4
        db 4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4
        db 7,  7,  7,  7,  7,  7,  4,  7,  4,  4,  4,  4,  4,  4,  7,  4
        db 4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4
        db 4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4
        db 4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4
        db 4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4
        db 5, 10, 10, 10, 10, 11,  7, 11,  5, 10, 10,  0, 10, 17,  7, 11
        db 5, 10, 10, 11, 10, 11,  7, 11,  5,  4, 10, 11, 10,  0,  7, 11
        db 5, 10, 10, 19, 10, 11,  7, 11,  5,  4, 10,  4, 10,  0,  7, 11
        db 5, 10, 10,  4, 10, 11,  7, 11,  5,  6, 10,  4, 10,  0,  7, 11
