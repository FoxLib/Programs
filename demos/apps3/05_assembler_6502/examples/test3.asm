        ; Различные методы задания числа
        org     0x8000

        ; Методы адресации
Label   lda     #-2             ; imm
        lda     #hi(FNC)        ; high(FNC)
        lda     #lo(FNC)        ; lo(FNC)
        jsr     FNC
        bmi     Label
        lda     $00             ; zp
        lda     $00,x           ; zp,x
        ldx     $00,y           ; zp,y
        lda     $0100           ; abs
FNC     lda     $0100,x         ; abs,x
        lda     $0100,y         ; abs,y
        lda     ($00,x)         ; inx
        lda     ($00),y         ; iny
        jmp     ($0000)         ; ind
        bcc     Label           ; rel
        clc                     ; imp
        asl     a               ; acc

; ----------------------------------------------------------------------
; 16-битное деление числа

; $00 -- 00 00 | 00 00 делимое
; $04 -- 00 00         делитель
; $06 -- 00 00         результат
; $08 -- 00            временно

        ; Инициализация
        ldx     #$10    ; 2T
        lda     #$00    ; 2T
        sta     $02     ; 3T
        sta     $03     ; 3T
        sta     $08     ; 3T
        sta     $09     ; 3T

        ; Сдвиг на 1 разряд
LPX:    asl     $00     ; 5T
        rol     $01     ; 5T
        rol     $02     ; 5T
        rol     $03     ; 5T

        ; Вычесть делимое - делитель
        sec             ; 2T
        lda     $02     ; 3T
        sbc     $04     ; 3T
        sta     $08     ; 3T
        lda     $03     ; 3T
        sbc     $05     ; 3T

        ; Если a >= b, то $08 -> $02
        bcc     LT      ; 2T
        sta     $03     ; 3T
        lda     $08     ; 3T
        sta     $02     ; 3T

        ; если <= то сдвиг есть
LT:     rol     $06     ; 5T
        rol     $07     ; 5T
        dex             ; 2T
        bne     LPX     ; 2T
        rts

