
;           DI = (AL>>3) & 7
; ----------------------------------------------------------------------
get53di:    mov     di, ax
            shr     di, 3
.and7:      and     di, 7
            ret
get20di:    mov     di, ax
            jmp     get53di.and7

;           Извлечь AX из di {0=BC,2=DE,4=HL,6=SP}
; ----------------------------------------------------------------------
get16di:    mov     ax, [bp+8]
            cmp     di, 6
            je      @f
            mov     ax, [bp+di]
@@:         xchg    ah, al
            ret

;           Сохраняет AX в регистр di {0=BC,2=DE,4=HL,6=SP}
; ----------------------------------------------------------------------
put16di:    xchg    ah, al
            cmp     di, 6
            jne     @f
            mov     di, 8 ; SP
@@:         mov     [bp+di], ax
            ret

;           Из AL[2:0] -> AH
; ----------------------------------------------------------------------
getreg8lo:  mov     di, ax      ; Регистр
.dirdi:     and     di, 7
            cmp     di, 6
            mov     ah, [bp+di]
            jne     @f          ; Значение из регистра
            mov     ah, [bx]    ; Или из (HL)
@@:         ret

;           Сохранить AH в регистр DI
; ----------------------------------------------------------------------
setreg8di:  cmp     di, 6
            je      @f
            mov     [bp+di], ah
            ret
@@:         mov     [bx], ah
            ret
