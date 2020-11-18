
        org     100h

        ; Инициализация
        mov     ax, $0013
        int     10h
        mov     ax, $a000
        mov     es, ax
        mov     ds, ax
        xor     di, di

        ; Генерация случайности
        mov     cx, 64000
@@:     add     ax, di
        mul     ax
        xor     al, ah
        stosb
        loop    @b

; ----------------------------------------------------------------------

        ; for (i = 0; i < 64000; i++)
        ; for (j = i + 1; j < 64000; j++)

        xor     si, si
        mov     bx, 64000
L2:     lea     di, [si + 1]
L1:     mov     al, [si]
        cmp     al, [di]
        jb      @f
        mov     ah, [si]
        xchg    ah, [di]
        xchg    ah, [si]
@@:     inc     di
        cmp     di, bx
        jne     L1
        inc     si
        cmp     si, bx
        jne     L2

        xor     ax, ax
        int     16h
        int     20h
