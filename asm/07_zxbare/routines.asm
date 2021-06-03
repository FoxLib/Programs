
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
            mov     cx, SZHXY
            call    set_flag_overflow
            call    set_flag_mask
            and     [cs:flags], 11111101b  ; N=0
            ret

;           Декремент AH с обновлением флагов
; ----------------------------------------------------------------------
do_dec:     dec     ah
            mov     cx, SZHXY
            call    set_flag_overflow
            call    set_flag_mask
            or      [cs:flags], 00000010b  ; N=1
            ret

;           Сложение аккумулятора с AH
; ----------------------------------------------------------------------
do_add:     add     ah, [bp+7]
            mov     cx, SZHCXY
            call    set_flag_overflow
            call    set_flag_mask
            and     [cs:flags], 11111101b  ; N=0
            mov     [bp+7], ah
            ret

;           Установить флаги после выполнения 8 битных инструкции
;           SZHXY: CH=11010000, CL=00000111b
; ----------------------------------------------------------------------
set_flag_mask:

            push    ax
            mov     al, ah
            lahf
            and     al, 00101000b   ; XY
            and     ah, ch          ; Отсеять биты по маске
            or      ah, al
            and     [cs:flags], cl  ; Очистить биты, которые используются
            or      [cs:flags], ah
            pop     ax
            ret

;           Копирование OF флага в P/V
; ----------------------------------------------------------------------
set_flag_overflow:

            pushf
            jno     .clear
            or      [cs:flags], 00000010b ; PV=1
            popf
            ret
.clear:     and     [cs:flags], 11111101b ; PV=0
            popf
            ret

