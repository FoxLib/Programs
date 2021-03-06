
; Соответствие флагов
;OSZ A P C-x86
; SZ5H3PNC-z80
; 76543210

;           BX = BC,DE,HL
bcbx:       mov     bx, [bp+reg_bc]
            xchg    bh, bl
            ret
debx:       mov     bx, [bp+reg_de]
            xchg    bh, bl
            ret
hlbx:       mov     bx, [bp+reg_hl]
            xchg    bh, bl
            ret

;           Считывание WORD из памяти. LODSW может вызвать #GP ошибку
getword:    lodsb
            mov     ah, al
            lodsb
            xchg    ah, al
            ret

            ; Загрузка mov ax, [bx] | mov [bx], ax
loadword:   mov     al, [bx]
            inc     bx
            mov     ah, [bx]
            ret
saveword:   mov     [bx], al
            inc     bx
            mov     [bx], ah
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
get16di:    mov     ax, [bp+reg_sp]
            cmp     di, reg_af
            je      @f
            mov     ax, [bp+di]
@@:         xchg    ah, al
            ret

;           Сохраняет AX в регистр DI {0=BC,2=DE,4=HL,6=SP}
; ----------------------------------------------------------------------
put16di:    xchg    ah, al
            cmp     di, reg_af
            jne     @f
            mov     di, reg_sp
@@:         mov     [bp+di], ax
            ret

;           Из AL[2:0] -> AH
; ----------------------------------------------------------------------
getreg8lo:  mov     di, ax      ; Регистр
.direct:    and     di, 7
            cmp     di, 6       ; 6=(hl)
            mov     ah, [bp+di]
            jne     @f          ; Значение из регистра
            mov     ah, [bx]    ; Или из (HL)
@@:         ret

;           Сохранить AH в регистр DI
; ----------------------------------------------------------------------
setreg8di:  cmp     di, 6       ; 6=(hl)
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

;           Сложение аккумулятора с AH и с переносом
; ----------------------------------------------------------------------
do_add:     add     [bp+7], ah
.flags:     mov     ah, [bp+7]
            mov     cx, SZHCXY
            call    set_flag_overflow
            call    set_flag_mask
            and     [cs:flags], 11111101b  ; N=0
            ret
do_adc:     call    loadcarry
            adc     [bp+7], ah
            jmp     do_add.flags

;           Вычитание и вычитание с переносом, сравнение
; ----------------------------------------------------------------------
do_sub:     sub     [bp+7], ah
.flags:     mov     ah, [bp+7]
            mov     cx, SZHCXY
            call    set_flag_overflow
            call    set_flag_mask
            or      [cs:flags], 00000010b  ; N=1
            ret
do_sbc:     call    loadcarry
            sbb     [bp+7], ah
            jmp     do_sub.flags
do_cp:      push    word [bp+6]
            call    do_sub
            pop     word [bp+6]
            ret

;           XOR/OR/AND A, <AH>
; ----------------------------------------------------------------------
do_xor:     xor     [bp+7], ah
.flags:     mov     ah, [bp+7]
            mov     cx, SZPXY
            call    set_flag_mask
            and     [cs:flags], 11101100b  ; H=0,N=0,C=0
            ret
do_and:     and     [bp+7], ah
            mov     ah, [bp+7]
            mov     cx, SZPXY
            call    set_flag_mask
            and     [cs:flags], 11101100b   ; N=0,C=0
            or      [cs:flags], 00000010b   ; H=1
            ret
do_or:      or      [bp+7], ah
            jmp     do_xor.flags

;           ADC hl, AX
; ----------------------------------------------------------------------
do_hl_adc:  call    loadcarry
            mov     bx, [bp+reg_hl]     ; BX=HL
            xchg    bh, bl
            push    ax
            add     ax, bx
            mov     cx, SZCXYnh
            call    set_flag_overflow
            call    set_flag_mask       ; SZC+XY, H=0, N=0
            xchg    ah, al
            mov     [bp+reg_hl], ax
            pop     cx                  ; CH=операнд, BH=H, AH=res
            xor     al, bh
            xor     al, ch
            test    al, 0x10
            je      @f
            or      [cs:flags], 00010000b   ; Установка AF
@@:         ret

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

;           Загрузка CF из flags
; ----------------------------------------------------------------------
loadcarry:  test    [cs:flags], byte 1      ; Очистка CF=0
            je      .ret                    ; Если ZF=1, то CF=0 остается
            stc
.ret:       ret
