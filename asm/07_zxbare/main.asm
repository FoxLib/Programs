            include "macro.asm"

            cli
            cld
            mov     ax, cs
            mov     ds, ax
            add     ax, 0x1000
            call    load_rom
            mov     ds, ax          ; 64Кб область памяти
            xor     ax, ax
            mov     ss, ax          ; Стек и данные для стека
            mov     ax, 0xa000
            mov     es, ax          ; Видео-область
            mov     ax, $0013
            int     10h             ; Включение 320x200
            mov     bp, $7000       ; Область данных
            mov     si, $8000       ; Регистр PC

            ; ax, bx, cx, di (свободные)

            ; DH=отложеннное включение di/ei
            ; DL=количество циклов инструкции


            ; Исполнение инструкции
; ----------------------------------------------------------------------
            xor     dx, dx
@@:         call    instr
            jmp     @b

            ; Дешифратор опкода
; ----------------------------------------------------------------------
instr:      dec     dh                      ; Проверка истечения DoDelayedEIDI
            jns     @f
            mov     dh, 0xff                ; DH не может быть меньше -1
@@:         inc     [cs:reg_r7f]            ; Инкремент регистра регенерации R
            lodsb
            mov     bh, 0
            mov     bl, al
            mov     dl, [cs:cycles_main+bx] ; Определить базовое количество тактов
            add     bx, bx
brk
            call    word [cs:table_main+bx] ; Вызвать обработчик инструкции
            ret
; ----------------------------------------------------------------------

; Инструкция NOP делает вообще ничего
_nop:       ret

; HALT
_halt:      mov     [cs:_halted], 1
            ret

; 00xx0000  ld r16, nn
ldnn:       call    get53di
            call    getword
            call    put16di
            ret

; 00xxx110  ld r8, *
ldr8n:      call    hlbx
            call    get53di
            lodsb
            mov     ah, al
            call    setreg8di
            ret

; 000x0010  ld (bc|de),a
ldbca:      call    bcbx
.ld:        mov     al, [bp+reg_a]
            mov     [bx], al
            ret
lddea:      call    debx
            jmp     ldbca.ld

; 000x1010  ld a,(bc|de)
ldabc:      call    bcbx
.ld:        mov     al, [bx]
            mov     [bp+reg_a], al
            ret
ldade:      call    debx
            jmp     ldabc.ld

; 00aaabbb  ld a,b
ld88:       call    hlbx
            call    getreg8lo
            call    get53di
            call    setreg8di
            ret

; 00xx0000  inc r16
incnn:      call    get53di
            call    get16di
            inc     ax
            call    put16di
            ret

; 00xx1000  dec r16
decnn:      call    get53di
            and     di, 6
            call    get16di
            dec     ax
            call    put16di
            ret

; 00xxx100  inc r8
inc8:       call    hlbx
            call    get53di
            call    getreg8lo.direct
            call    do_inc
            call    setreg8di
            ret

; 00xxx101  dec r8
dec8:       call    hlbx
            call    get53di
            call    getreg8lo.direct
            call    do_dec
            call    setreg8di
            ret

; 00101010  ld hl, (**)
ldhlnn:     call    getword
            mov     bx, ax
            call    loadword
            mov     [bp+reg_hl],   ah       ; H
            mov     [bp+reg_hl+1], al       ; L
            ret

; 00111010  ld (**), hl
ldnnhl:     call    getword
            mov     bx, ax
            mov     ax, [bp+reg_hl]
            xchg    ah, al
            call    saveword
            ret

; 0011 0010 ld (**), a
ldnna:      call    getword
            mov     bx, ax
            mov     al, [bp+reg_a]
            mov     [bx], al
            ret

; 0011 1010 ld a, (**)
ldann:      call    getword
            mov     bx, ax
            mov     al, [bx]
            mov     [bp+reg_a], al
            ret

; ----------------------------------------------------------------------
include     "sysparam.asm"
include     "routines.asm"
include     "utils.asm"
include     "table.main.asm"
