
            org 100h

            ; Включение видеорежима
            mov     ax, $0013
            int     10h
            mov     ax, $a000
            mov     es, ax

            mov     [CircleColor], 0x0f
            mov     si, 160
            mov     di, 100
            mov     cx, 150
            ;call    CircleFill
            ;ret

            ; === Тестовый способ ===

            ; Рисование окружности r=50
            mov     di, (100-100)*(320) + 160
            mov     ax, 0       ; ax=0
            mov     bx, 100     ; bx=y

            ; init
            mov     dx, bx
            add     dx, 3
            sub     dx, bx
            sub     dx, bx      ; dx+=(3-2*y)
circle:     mov     [es:di], byte $0f       ; si, di, cx
            mov     cx, ax
            shl     cx, 2
            add     dx, cx      ; dx += 4*x+6
            add     dx, 6
            js      skipy
            mov     cx, 1
            sub     cx, bx
            shl     cx, 2
            add     dx, cx      ; d += 4*(1-y)
            add     di, 320     ; y--
            dec     bx
skipy:      inc     di
            inc     ax
            cmp     ax, bx
            jbe     circle
            ret

include     "circle.asm"
