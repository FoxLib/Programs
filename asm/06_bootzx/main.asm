
            ; bp+0 B, C, D, E, H, L; AF
            ; bp+8 flags
            ; bp+10 sp

            org     7c00h
            macro   brk { xchg bx, bx}
            macro   instr a, b, c {
                db b, a
                dw c
            }
            IPS     equ 256

            cld
            cli
            mov     ax, $0230           ; 48 секторов = 6000h
            mov     bx, $8000
            mov     cx, $0002
            mov     dh, $00
            int     13h
            mov     ax, $0013
            int     10h                 ; Видеорежим 320x200
            push    word 0xA000
            pop     es                  ; ES=A000h
            mov     word [8*4+0], timer ; Переназначить IRQ
            mov     word [8*4+2], cs
            mov     ax, $0800
            mov     ds, ax              ; DS=8000h
            mov     bp, 0x4000
            mov     si, 0x5c00          ; Регистр PC=$5C00 с программой
            call    savef
            sti
            jmp     $                   ; Обработка в прерывании INT 8h


; Выполнение цикла
; ----------------------------------------------------------------------
timer:
            ; Отрисовка видео области
            push    si
            mov     di, 32+4*320    ; (32,4) Старт дисплея
            mov     si, $4000       ; Область бинарных данных
            mov     bx, $5800       ; Область атрибутов
.R5:        mov     dh, 8           ; 8 знакомест по 8
.R4:        mov     dl, 32          ; 32 столбца
.R3:        mov     ch, 8           ; 8 строк
            push    bx
            mov     al, [bx]        ; BH=передний цвет, BL=задний
            mov     bl, al
            mov     bh, al
            shr     bl, 3
            and     bx, $0707       ; BH=attr&7, BL=(attr>>3)&7
            test    al, $40         ; BRIGHT
            je      .R2
            or      bx, $0808
.R2:        lodsb                   ; Загрузить очередной байт
            mov     ah, al          ; В регистр AH
            mov     cl, 8           ; 8 пикселей
.R1:        add     ah, ah          ; Проверить следующий пиксель
            mov     al, bh          ; Если он 1, то цвет равен BH
            jb      @f
            mov     al, bl          ; Если он 0, то рисуется BL
@@:         push    bx              ; Коррекция цвета
            movzx   bx, al
            mov     al, [cs:bx+colortable]
            pop     bx
            stosb
            dec     cl
            jne     .R1             ; Повторить 8 раз
            add     si, $00FF       ; Следующая строка +100h из памяти
            add     di, 312         ; Рисуемая следующая строка
            dec     ch
            jne     .R2             ; Повторить 8 раз для рисования символа
            pop     bx
            inc     bx              ; К следующему знакоместу
            add     di, -320*8+8    ; Коррекция DI
            add     si, $F801       ; Коррекция SI Y-=8,X++
            dec     dl              ; Нарисовать таким образом 32 символа
            jne     .R3
            add     di, 320*8-256   ; К следующему знакоместу на новой строке
            dec     dh
            jne     .R4
            add     si, $0700       ; Следующий блок
            cmp     bx, $5b00       ; Пока не достиг 3-го блока
            jne     .R5
            pop     si

            ; Исполнение блока инструкции
            mov     cx, IPS               ; N инструкции за 1 кадр
.L1:        push    cx
            lodsb
            mov     bx, [bp+4]          ; bx=hl
            xchg    bh, bl              ; потому что первый идет H, потом L
            mov     di, pattern
@@:         mov     dx, [cs:di]
            add     di, 4
            and     dh, al
            cmp     dl, dh
            jne     @b

            call    word [cs:di-2]      ; Вызвать обработчик инструкции
            pop     cx
            loop    .L1

            ; EXIT
            mov     al, $20             ; Отослать EOI
            out     $20, al
            iret

            ; LD r8, r8
; ----------------------------------------------------------------------
moves:      call    getreg8lo
            call    get53di
            call    setreg8di
            ret

            ; LD r8, #
; ----------------------------------------------------------------------
ldi8:       mov     ah, [si]
            inc     si
            call    get53di
            call    setreg8di
            ret

            ; LD r16, #
; ----------------------------------------------------------------------
ldnn:       call    get53di
            lodsw
            xchg    ah, al
            call    spadjust
            mov     [bp+di], ax     ; BC,DE,HL,SP
            ret

            ; INC|DEC r16
; ----------------------------------------------------------------------
inc16:      call    getreg16
            inc     dx
.end:       call    setr16
            ret
dec16:      call    getreg16
            dec     dx
            jmp     inc16.end

            ; Получение регистра 16
getreg16:   call    get53di
            call    spadjust
            call    getr16
            ret

            ; INC/DEC r8
; ----------------------------------------------------------------------
incdec8:    call    get53di
            call    getreg8lo.dirdi
            test    al, 1
            jne     .dec
            call    loadf
            add     ah, 1
            jmp     @f
.dec:       call    loadf
            sub     ah, 1
@@:         call    savef
            call    setreg8di
            ret

            ; JR a,(BC|DE)
; ----------------------------------------------------------------------
ldade:      call    get53di
            and     di, 2
            call    getr16
            mov     bx, dx
            mov     al, [bx]
            mov     [bp+7], al
            ret

            ; JR cc, *
; ----------------------------------------------------------------------
jrcc:       call    get53di
            and     di, $03
            mov     al, [cs:jumptable+di]
            mov     [.modif], al
            call    loadf
.modif:     jne     .done
            inc     si              ; Перехода нет
            ret
.done:      call    jump8           ; Переход одобрен
            ret

; Для создания "виртуальной" машины
jphl:       mov     si, bx          ; PC=HL
            ret

; ----------------------------------------------------------------------
            ; <alu> a,imm
aluimm:     mov     ah, [si]
            inc     si
            jmp     alues.exec

            ; <alu> a,r8
alues:      call    getreg8lo       ; AH=значение регистра
.exec:      call    get53di         ; DI=номер операции АЛУ
            call    aluop
            ret

; Вычисление ALU A, AH; di - номер операции ALU; ah - операнд
; ----------------------------------------------------------------------
aluop:      mov     al, [cs:di+replacealu]
            mov     [cs:codealu], al
            call    loadf
codealu:    add     [bp+7], ah      ; Модифируемый код
            call    savef
            ret

; Трансляция для 80 -> 86
;               ADD   ADC   SUB   SBC   AND   XOR    OR    CP
replacealu: db 0x00, 0x10, 0x28, 0x18, 0x20, 0x30, 0x08, 0x38

; Прочесть регистр DI => AH
; ----------------------------------------------------------------------
getreg8lo:  mov     di, ax      ; Регистр
.dirdi:     and     di, 7
            cmp     di, 6
            mov     ah, [bp+di]
            jne     @f          ; Значение из регистра
            mov     ah, [bx]    ; Или из (HL)
@@:         ret

; DI = (AL>>3)&7
; ----------------------------------------------------------------------
get53di:    mov     di, ax
            shr     di, 3
            and     di, 7
            ret

; Сохранить AH в регистр номер DI
; ----------------------------------------------------------------------
setreg8di:  cmp     di, 6
            je      @f
            mov     [bp+di], ah
            ret
@@:         mov     [bx], ah
            ret

; DX=reg16 по DI
getr16:     mov     dx, [bp+di]
            xchg    dh, dl
            ret
setr16:     xchg    dh, dl
            mov     [bp+di], dx
            ret

; Сохранение флагов
; ----------------------------------------------------------------------
loadf:      push    word [bp+16]
            popf
            ret
savef:      pushf
            pop     word [bp+16]
            ret

            ; DI=6 -> DI=10 (Коррекция на регистр SP)
spadjust:   cmp     di, 6
            jne     @f
            add     di, 4           ; DI=10
@@:         ret

            ; Выполнение JR
jump8:      lodsb
            cbw
            add     si, ax
            ret

; Цветовая таблица
colortable: db      0, 1, 4, 5, 2, 3, 6, 7
            db      0, 9,12,13,10,11,14,15

; NZ,Z,NC,C
jumptable:  db      0x75, 0x74, 0x73, 0x72

; ----------------------------------------------------------------------
pattern:

    instr   11111111b, 11101001b, jphl
    instr   11101111b, 00001010b, ldade
    instr   11100111b, 00100000b, jrcc
    instr   11000110b, 00000100b, incdec8
    instr   11111111b, 00011000b, jump8
    instr   11001111b, 00000011b, inc16
    instr   11001111b, 00001011b, dec16
    instr   11001111b, 00000001b, ldnn
    instr   11000111b, 11000110b, aluimm
    instr   11000111b, 00000110b, ldi8
    instr   11000000b, 10000000b, alues
    instr   11000000b, 01000000b, moves
