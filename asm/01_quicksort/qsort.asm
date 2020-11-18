
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

        ; Начать сортировку 64k
        push    0
        push    63999
        call    qsort

        xor     ax, ax
        int     16h
        int     20h

; ----------------------------------------------------------------------
qsort:  push    bp
        mov     bp, sp

        ; Поиск центра pivot
        mov     si, [bp + 6] ; a = l
        mov     di, [bp + 4] ; b = r
        mov     bx, si
        shr     bx, 1
        mov     ax, di
        shr     ax, 1
        add     bx, ax          ; BX=(a+b)/2
        mov     al, [bx]        ; AL=pivot

wend:   ; --------------------------------------------------------------
        ; while arr[si] < pivot: si++
@@:     cmp     [si], al
        inc     si
        jb      @b
        dec     si

        ; while pivot < arr[di]: di--
@@:     cmp     al, [di]
        dec     di
        jb      @b
        inc     di

        ; if a <= b: swap arr[a], arr[b]; a++; b--
        cmp     si, di
        jnbe    @f

        mov     ah, [si]
        xchg    ah, [di]
        xchg    ah, [si]
        inc     si
        dec     di

@@:     ; loop while a <= b
        cmp     si, di
        jbe     wend

        ; --------------------------------------------------------------

        ; if l < b: qsort l, b
        cmp     [bp + 6], di
        jnb     @f
        push    word [bp + 6]
        push    di
        call    qsort

@@:     ; if a < r: qsort a, r
        cmp     si, [bp + 4]
        jnb     @f
        push    si
        push    word [bp + 4]
        call    qsort

        ; Выход из процедуры
@@:     mov     sp, bp
        pop     bp
        ret     4
