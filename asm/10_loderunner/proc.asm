; Отрисовать экран
; si,di - смещение в пикселях (x,y), bp-данные для рисования
; ----------------------------------------------------------------------
refresh:

            push    ax bx cx dx si di bp es

            ; Сегмент для рисования
            mov     es, [seg_fb]

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
            add     bx, [current_level]
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

; bx-номер тайла, si-X, di-Y
; ----------------------------------------------------------------------
draw_tile:

            push    ax bx cx dx si di

            ; Поиск бинарных данных
            cmp     bx, TILE_LADDER
            mov     dx, ladder_phase + 1
            mov     ax, ladder_anim
            je      .set_animation

            cmp     bx, TILE_GOLD
            mov     dx, gold_phase + 1
            mov     ax, gold_anim
            je      .set_animation

            jmp     .find

.set_animation:

            ; Текущая анимация лестницы
            ; DX-фаза, AX-ссылка на таблицу
            mov     bx, dx
            mov     bl, byte [bx]
            mov     bh, 0
            add     bx, bx
            add     bx, ax
            mov     bx, [bx]
            jmp     .found

.find:      ; Поиск в таблице тайлов
            add     bx, bx              ; bx *= 2
            mov     bx, [img_table + bx]

.found:     ; Рисование тайла
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
@@:         ret

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
