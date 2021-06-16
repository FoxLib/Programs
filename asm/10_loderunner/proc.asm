; Отрисовать экран
; si,di - смещение в пикселях (x,y), bp-данные для рисования
; ----------------------------------------------------------------------
refresh:

            push    ax bx cx dx si di bp es

            ; Сегмент для рисования
            mov     ax, $a000
            mov     es, ax

            ; Расчет скролла по X
            mov     ax, [scroll_x]
            mov     word [scroll_x_fine], ax
            mov     word [scroll_x_tile], ax
            and     word [scroll_x_fine], 15
            shr     word [scroll_x_tile], 4

            ; Расчет скролла по Y
            mov     ax, [scroll_y]
            mov     word [scroll_y_fine], ax
            mov     word [scroll_y_tile], ax
            and     word [scroll_y_fine], 15
            shr     word [scroll_y_tile], 4

            mov     dx, 0                   ; Y for(y=0; y<=12;y++)
.tiley:     mov     cx, 0                   ; X for(x=0; x<=16;x++)
.tilex:     mov     ax, [scroll_y_tile]     ; Расчитать положение в уровне
            add     ax, dx                  ; Тайловая плитка
            shl     ax, 5                   ; x32
            push    ax
            mov     ax, [scroll_x_tile]
            add     ax, cx
            pop     bx
            add     bx, ax                  ; bx=(scrollx+cx) + (scrolly+dx)*32

            ; Номер тайла
            add     bx, bp
            mov     bl, [bx]
            mov     bh, 0

            ; si = cx*16 - scrollx_fine
            mov     si, cx
            shl     si, 4
            sub     si, [scroll_x_fine]

            ; di = dx*16 - scrolly_fine
            mov     di, dx
            shl     di, 4
            sub     di, [scroll_y_fine]
            call    draw_tile

            inc     cx
            cmp     cx, 20
            jbe     .tilex

            inc     dx
            cmp     dx, 12
            jbe     .tiley

            pop     es bp di si dx cx bx ax
            ret

; bx-номер спрайта, si-X, di-Y
; ----------------------------------------------------------------------
draw_tile:

            push    ax bx cx dx si di

            ; Поиск бинарных данных
            add     bx, bx              ; bx *= 2
            mov     bx, [img_table + bx]

            mov     ch, 16
.psety:     mov     cl, 16
.psetx:     mov     al, [bx]            ; Извлечь следующий пиксель
            inc     bx

            ; Рисуем ли на экране?
            cmp     si, 320
            jnb     .skip
            cmp     di, 200
            jnb     .skip

            ; Рисуем пиксель
.ok:        push    di
            imul    di, 320             ; DI = Y*320 + X
            add     di, si
            stosb
            pop     di

.skip:      inc     si                  ; X++
            dec     cl
            jne     .psetx

            sub     si, 16              ; X -= 16
            inc     di                  ; Y++
            dec     ch
            jne     .psety

            pop     di si dx cx bx ax
            ret

; Установка NES-палитры
; ----------------------------------------------------------------------
set_palette:

            mov     dx, 968
            xor     ax, ax
            out     dx, al
            inc     dx

            mov     si, palette
            mov     cx, 40h
@@:         mov     al, [si+2]  ; R
            shr     al, 2
            out     dx, al
            mov     al, [si+1]  ; G
            shr     al, 2
            out     dx, al
            mov     al, [si+0]  ; B
            shr     al, 2
            out     dx, al
            add     si, 4
            loop    @b
            ret
