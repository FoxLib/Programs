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
            mov     dx, $0004
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

; Нарисовать все спрайты на экране
; ----------------------------------------------------------------------
redraw_sprite:

            mov     si, sprites
            mov     cx, 5

.next:      mov     ax, [si+3]      ; ATTR
            test    ah, $80
            jne     .nodraw         ; [7] Не рисовать спрайт

            mov     bp, ax
            shr     bp, 8
            and     bp, 15          ; BP=Номер палитры [3:0]

            mov     dh, ah
            shr     dh, 4           ; [4] Зеркальность по X
            mov     dl, 2           ; Здесь прозрачный цвет =2

            mov     ah, 0
            or      al, $40         ; ID тайла

            push    si cx
            mov     di, [si+2]      ; Y
            mov     si, [si]        ; X
            and     di, 255
            call    draw_sprite
            pop     cx si

.nodraw:    add     si, 5
            loop    .next

            ret

; ----------------------------------------------------------------------
; Нарисовать тайл 16 x 16
; * al      - номер тайла (0..511)
; * bp      - номер палитры
; * dl      - номер прозрачного цвета
; * dh[0]   - слева направо рисуется
; * si, di  - позиция на экране
; ----------------------------------------------------------------------
draw_sprite:

            pusha
            push    es
            mov     es, [seg_fb]

            mov     [.conf], dh ; Конфигурация рисования
            test    dh, 1
            je      @f
            add     si, 16      ; Рисование справа налево
@@:         shl     bp, 2       ; Получение палитры
            add     bp, current_palette
            mov     ah, 0
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

            ; Проверка границ
            cmp     si, 320
            jnb     .skip
            cmp     di, 200
            jnb     .skip

            ; Вычисление реального цвета
            add     ax, bp
            mov     bx, ax
            mov     al, [bx]

            ; Рисование пикселя
            push    di
            imul    di, 320
            add     di, si
            stosb
            pop     di

            ; Отрисовка 4 пикселей
.skip:      pop     bx ax
            inc     si
            test    [.conf], byte 1
            je      @f
            dec     si
            dec     si
@@:         dec     cl
            jne     .next1

            ; Следующий BYTE (4 пикселей)
            inc     bx
            dec     dh
            jne     .next2

            ; Следующая строка
            sub     si, 16
            test    [.conf], byte 1
            je      @f
            add     si, 32
@@:         inc     di
            dec     ch
            jne     .next3

            pop     es
            popa
            ret

.conf:      db      0

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

; Переназначить клавиатуру
; ----------------------------------------------------------------------
irq1_set:
            ; Назначить новое прерывание
            push    ds
            mov     ax, 0
            mov     ds, ax
            mov     [4*9], word kbd
            mov     [4*9+2], cs
            pop     ds

            ; Очистить буфер
            mov     di, keyboard
            mov     cx, 64
            xor     ax, ax
            rep     stosw
            ret

; Получение кодов нажатых клавиш в данный момент
; ----------------------------------------------------------------------

kbd:        push    ax bx
            in      al, $60
test al, $80
jne  @f
;int3
@@:
            ; Состояние нажатых сканкодов
            mov     bh, 0
            mov     bl, al
            and     bl, 0x7F
            add     al, al      ; CF=1 если старший флаг 1
            sbb     al, al      ; A=1 если нажата, A=0 если отпущена
            xor     al, $ff
            mov     [keyboard+bx], al

            mov     al, $20
            out     $20, al
            pop     bx ax
            iret
