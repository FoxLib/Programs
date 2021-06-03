
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
            call    CircleFill
            ret

            ; === Тестовый способ ===

            ; Рисование окружности r=50
            mov     di, (100-50)*(320) + 160
            mov     ax, 0       ; ax=0
            mov     bx, 50     ; bx=y

            ; init
            mov     dx, bx
            add     dx, dx      ; dx=2*y
            sub     dx, 3
            neg     dx          ; dx=3-2*y
circl:      cmp     ax, bx
            jnb     exit
            mov     cx, ax
            shl     cx, 2
            add     dx, 6
            add     dx, cx      ; d += 4*x+6
            js      skipy
            mov     cx, 1
            sub     cx, bx
            shl     cx, 2
            add     dx, cx      ; d += 4*(1-y)
            add     di, 320     ; y--
            dec     bx
skipy:      inc     di
            inc     ax

            mov     [es:di], byte $0f
            jmp     circl

exit:       int3
            ret

include     "circle.asm"
