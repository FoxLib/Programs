
        cld
        sei        
        lda     #$20
        sta     $2006
        lda     #$00
        sta     $2006       ; ставим видеокурсор куда надо
        tay
        lda     #lo(DATA)   ; указываем адрес, откуда грузится строка
        sta     $00
        lda     #hi(DATA)
        sta     $01        
ROT     lda     ($00),y     ; след. символ
STOP    beq     STOP    
        iny
        sta     $2007       ; записываем в память
        jmp     ROT
        
DATA    
        db "This program is Trial Shareware!", 0
