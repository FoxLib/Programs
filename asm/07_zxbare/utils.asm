; Загрузка ROM-образа в память DS:0
; ----------------------------------------------------------------------
load_rom:
            push    ax
            mov     [DAP+6], ax
            mov     ah, 42h
            mov     si, DAP
            mov     dl, $80
            int     13h
            pop     ax
            ret

; Вообще это так не должно быть
DAP:        db 10h
            db 00h
            dw 0060h        ; 60h = 48kb
            dw 0000h
            dw 0800h        ; Адрес загрузки
            dq 129

; Выдача регистров на экран
; ----------------------------------------------------------------------

regdump:    ; BH-Y, BL-X, AL-значение регистра
            ret

; Быстрый рендеринг всего экрана
; ----------------------------------------------------------------------

renderall:

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
            ret
