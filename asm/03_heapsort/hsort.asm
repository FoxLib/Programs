
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
        ; for i = 32767; i >= 0; i--: sink i, n
        mov     cx, 31999
@@:     push    cx
        push    64000
        call    sink
        dec     cx
        jns     @b

        ; for i = 64000; i > 0; i--:
        mov     bx, 64000

        ; swap 0, i
@@:     mov     al, [bx]
        xchg    al, [0]
        xchg    al, [bx]

        ; sink 0, i
        push    0
        push    bx
        call    sink
        dec     bx
        jne     @b

        xor     ax, ax
        int     16h
        int     20h

; ----------------------------------------------------------------------

        ; heap(6=i, 4=n)
; ----------------------------------------------------------------------
sink:   push    bp
        mov     bp, sp
        push    bx

        mov     bx, [bp+6]      ; max=i
        mov     ax, bx

        ; if l < n:
        add     ax, ax
        inc     ax
        mov     si, ax          ; l=2*i + 1
        jb      L3              ; Переполнение
        cmp     si, [bp+4]
        jnc     L3

        ; then if arr(l) >= arr(max):
        mov     al, [si]
        cmp     al, [bx]
        jc      L1
        mov     bx, si

        ; if r < n:
L1:     add     si, 1
        jc      L2
        cmp     si, [bp+4]      ; r < n
        jnc     L2

        ; then if arr(r) >= arr(max):
        mov     al, [si]
        cmp     al, [bx]
        jc      L2
        mov     bx, si

        ; if bx != i:
L2:     cmp     bx, [bp+6]
        je      L3

        ; swap arr(i), arr(max)
        mov     si, [bp+6]
        mov     al, [si]
        xchg    al, [bx]
        xchg    al, [si]

        ; heap(max, n)
        push    bx
        push    word [bp+4]
        call    sink

L3:     pop     bx
        mov     sp, bp
        pop     bp
        ret     4
