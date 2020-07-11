    
        ; x1            $00
        ; y1            $01
        ; x2            $02
        ; y2            $03
        
        ; deltax        $04, $05
        ; deltay        $06, $07
        ; error         $08, $09
        ; errort        $0A, $0B
        
proc    DRAWLINE

        local   X1 = $00, Y1 = $01, X2 = $02, Y2 = $03

        lda     #$00    ; обнулим старший байт deltax, deltay
        sta     $05
        sta     $07
        sec
        lda     X2
        sbc     X1
        sta     $04     ; deltax = x2 - x1
        sec
        lda     Y2
        sbc     Y1
        sta     $06     ; deltay = y2 - y1

        "NEW ERROR"     ; -- это такой своеобразный отладчик будет тут
        
        clc             ; 1 error = error + deltay (13 байт на сложение 2 чисел)
        lda     $08     ; 2
        adc     $06     ; 2
        sta     $08     ; 2
        lda     $09     ; 2
        adc     $07     ; 2
        sta     $09     ; 2
        
        "SIGNED MUL x2 / 10b"
        
        ; Знаковое errort = error * 2
        lda     $08     ; 2
        asl     a       ; 1
        sta     $0A     ; 2
        lda     $09     ; 2
        rol     a       ; 1   
        sta     $0B     ; 2
        
        ; ... 2*error >= deltax
        sec
        lda     $0A
        sbc     $04
        lda     $0B
        sbc     $05     ; если CF=1, то это >=
        bcc     less
        
        ; ... Y++ or Y--
        
        ; error -= deltax
        sec             ; 1
        lda     $08     ; 2
        sbc     $04     ; 2
        sta     $08     ; 2
        lda     $09     ; 2
        sbc     $05     ; 2
        sta     $09     ; 2
        
less:   ; проверить, выходит ли за пределы X?

endproc
