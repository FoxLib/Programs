
        org     100h

        ; swap [bx], [si]
        macro   swap a, b {

            mov     al, [a]            
            xchg    [b], al
            xchg    [a], al
        }

        ; perm(bx+1,di)
        macro invk n {

            inc     bx
            push    si
            call    n                
            pop     si
            dec     bx
        }

        ; print string$
        macro print x {

            mov     dx, x
            mov     ah, 9
            int     21h
        }

        ; Уникальные перестановки
        mov     bx, src
        mov     di, src+4-1
        call    perm
        int     20h

; Процедура перестановок [SI .. DI]
; ----------------------------------------------------------------------
        
perm:   mov     si, bx              
        cmp     bx, di
        jne     for
        print   src
        ret

        ; for si = bx to di
for:    swap    bx, si
        invk    perm
        swap    bx, si
        inc     si
        cmp     si, di
        jbe     for
        ret

; ----------------------------------------------------------------------
src     db      "ABCD $"
