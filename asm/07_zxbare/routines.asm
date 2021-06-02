
; Соответствие флагов
;OSZ A P C-x86
; SZ5H3PNC-z80
; 76543210

;           BX = HL
hlbx:       mov     bx, [bp+4]      ; bx=hl
            xchg    bh, bl
            ret

;           DI = (AL>>3) & 7; DI = AL & 7
; ----------------------------------------------------------------------
get53di:    mov     di, ax
            shr     di, 3
            and     di, 7
            ret
get20di:    mov     di, ax
            and     di, 7
            ret

;           Извлечь AX из DI {0=BC,2=DE,4=HL,6=SP}
; ----------------------------------------------------------------------
get16di:    mov     ax, [bp+8] ; SP
            cmp     di, 6
            je      @f
            mov     ax, [bp+di]
@@:         xchg    ah, al
            ret

;           Сохраняет AX в регистр DI {0=BC,2=DE,4=HL,6=SP}
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
.direct:    and     di, 7
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

;           Инкремент AH с обновлением флагов
; ----------------------------------------------------------------------
do_inc:     inc     ah
            call    set_szhxy
            and     [cs:flags], 11111001b  ; PV=0, N=0
            cmp     ah, 0x80
            jne     @f
            or      [cs:flags], 00000100b  ; PV=1
@@:         ret

;           Декремент AH с обновлением флагов
; ----------------------------------------------------------------------
do_dec:     dec     ah
            call    set_szhxy
            and     [cs:flags], 11111001b  ; PV=0, N=0
            cmp     ah, 0x7f
            jne     @f
            or      [cs:flags], 00000100b  ; PV=1
@@:         ret

;           Установить флаги SZXY после выполнения 8 битных инструкции
; ----------------------------------------------------------------------
set_szhxy:  push    ax
            mov     al, ah
            lahf
            ;           SZ A      X Y
            and     ax, 1101000000101000b   ; SZ + XY флаги
            or      ah, al
            and     [cs:flags], 00000111b
            or      [cs:flags], ah
            pop     ax
            ret
