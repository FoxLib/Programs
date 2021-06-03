
CircleColor     db 0x3F

; ----------------------------------------------------------------------
; Рисование заполненную цветом окружность
; Legend:
;
;   si - x_center, di - y_center, cx - radius
;   ax = temp, bx = x, cx = y, dx = d 
;
; SIZE: 96 B
; ----------------------------------------------------------------------

CircleFill:

        xor     bx, bx              ; x = 0
        mov     ax, 3
        imul    dx, cx, 2           ; cx = radius
        sub     ax, dx              ; d = 3 - 2*radius       
.while: cmp     bx, cx              ; while (x <= y)
        jg      CircleLine.fin        
        call    Circle_LineX        ; line(xc-x,yc-y, xc+x,yc-y)
        xchg    bx, cx              ; line(xc-x,yc+y, xc+x,yc+y)
        call    Circle_LineX        ; line(xc-y,yc-x, xc+y,yc-x)
        xchg    bx, cx              ; line(xc-y,yc+x, xc+y,yc+x)                    
        imul    ax, bx, 4           
        add     ax, 6                
        add     dx, ax              ; d += 4*x + 6        
        and     dx, dx
        js      .nx                 ; if d >= 0:
        imul    ax, cx, 4
        add     dx, 4               ; d += 4*(1 - y)
        sub     dx, ax
        dec     cx                  ; y--
.nx:    inc     bx
        jmp     .while

; -------- Рисование двух линии для круга ------------------------------
Circle_LineX:

        mov     bp, di              ; Хитрый план ^_^ 
        sub     bp, cx              ; 1. call    CircleLine
        call    CircleLine          ; 2. ret     из Circle_LineX
        add     bp, cx              ; 3. ????
        add     bp, cx              ; 4. PROFIT! (Спасём 4 байта)

; -------- Линия, которая не выходит за границы экрана -----------------
CircleLine:

        pusha
        cmp     bp, 200
        jnb     .cskip              ; (y < 0) OR (y >= 200) ? EXIT
        imul    cx, bx, 2           ; cx = 2 x R
        sub     si, bx              ; x = xc - x        
        imul    bx, bp, 320         ; bx = 320 * y        
        mov     al, [CircleColor]        
        inc     cx
.loop:  cmp     si, 320
        jnl     .cskip              ; si > 320?
        jnb     .pix                ; si < 0?
        mov     [es:bx + si], al
.pix:   inc     si
        loop    .loop        
.cskip: popa
.fin:   ret
