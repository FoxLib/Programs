; Отрисовать экран
; si,di - смещение в пикселях (x,y), bp-данные для рисования
; ----------------------------------------------------------------------
refresh:

            pusha
            push    es

            ; Сегмент для рисования
            mov     es,  [seg_fb]

            ; Расчет скролла по X
            mov     ax,  [scroll_x]
            mov     word [scroll_x_fine], ax
            mov     word [scroll_x_tile], ax
            and     word [scroll_x_fine], 15
            shr     word [scroll_x_tile], 4

            ; Расчет скролла по Y
            mov     ax,  [scroll_y]
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
            add     bx, [current_level]
            mov     bl, [bx]
            mov     bh, 0

            ; AL-номер спрайта (0..255); DL=4 (все непрозрачное)
            add     bx, bx
            mov     ax, [tile_convert + bx]
            mov     bp, ax
            shr     bp, 8
            and     ax, 0x00ff

            ; si = cx*16 - scrollx_fine
            mov     si, cx
            shl     si, 4
            sub     si, [scroll_x_fine]

            ; di = dx*16 - scrolly_fine
            mov     di, dx
            shl     di, 4
            sub     di, [scroll_y_fine]

            ; Рисование тайла на экране
            push    dx
            mov     dl, 4
            call    draw_sprite
            pop     dx

            inc     cx
            cmp     cx, 20
            jbe     .tilex

            inc     dx
            cmp     dx, 12
            jbe     .tiley

            pop     es
            popa
            ret

; Нарисовать:
; * ax - номер тайла (0..511)
; * si, di - позиция на экране
; * bp - номер палитры
; * dl - номер прозрачного цвета
; ----------------------------------------------------------------------
draw_sprite:

            pusha
            push    es
            mov     es, [seg_fb]

            shl     bp, 2       ; Получение палитры
            add     bp, current_palette
            mov     bx, ax
            shl     bx, 6       ; bx = ax*64
            add     bx, tilemap ; Спрайты и тайлы

            mov     ch, 16
.next3:     mov     dh, 4
.next2:     mov     cl, 4
            mov     al, [bx]    ; 8 пикселей

            ; Просмотр следующего индексного цвета
.next1:     rol     al, 2

            ; Сохранение текущего положения цвета и указателя
            push    ax bx
            and     ax, 3

            ; Прозрачный цвет?
            cmp     al, dl
            je      .skip

            ; Вычисление реального цвета
            add     ax, bp
            mov     bx, ax
            mov     al, [bx]

            ; Проверка границ
            cmp     si, 320
            jnb     .skip
            cmp     di, 200
            jnb     .skip

            ; Рисование пикселя
            push    di
            imul    di, 320
            add     di, si
            stosb
            pop     di

            ; Отрисовка 4 пикселей
.skip:      pop     bx ax
            inc     si
            dec     cl
            jne     .next1

            ; Следующий BYTE (4 пикселей)
            inc     bx
            dec     dh
            jne     .next2

            ; Следующая строка
            sub     si, 16
            inc     di
            dec     ch
            jne     .next3

            pop     es
            popa
            ret

; Отрисовка фреймбуфера на экране
; ----------------------------------------------------------------------
flip:       push    ds es
            mov     ds, [cs:seg_fb]
            mov     es, [cs:seg_a000]
            xor     si, si
            xor     di, di
            mov     cx, 32000
            rep     movsw
            pop     es ds
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

; Процедура для обновления анимации тайлов на уровне
; CH-количество отсчетов, CL-скорость, DX-ссылка на палитру, BX-счетчик
; ----------------------------------------------------------------------
update_tile_animation:

            inc     byte [bx]           ; Младший счетчик
            cmp     byte [bx], cl
            jne     @f
            mov     byte [bx], 0
            inc     byte [bx+1]         ; Старший счетчик
            cmp     byte [bx+1], ch
            jne     @f
            mov     byte [bx+1], 0
@@:         mov     bl, [bx+1]
            mov     bh, 0
            add     bx, dx
            mov     al, [bx]
            ret

; Перехват таймера
; ----------------------------------------------------------------------

int8_timer:

            cli

            ; Перепрограммирование таймера
            ; Модулятор 1193181 / HZ = число
            ; 47727 = 25 герц
            mov     al, 0x6F
            out     $40, al         ; low
            mov     al, 0xBA
            out     $40, al         ; high

            push    ds
            xor     ax, ax
            mov     ds, ax
            mov     ax, [4*8]
            mov     [0xff*4], ax
            mov     ax, [4*8+2]
            mov     [0xff*4+2], ax      ; Переместить таймер на INT FFh
            mov     [4*8], word clock
            mov     [4*8+2], cs         ; Назначить новый таймер
            pop     ds
            sti
            ret
