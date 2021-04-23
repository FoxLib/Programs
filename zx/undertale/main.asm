
        org     $6580       ; Начало после Бейсик-программы
DECOMP: equ     $5b00       ; Буфер A28h (2600) байт для распаковки экрана

        di

        ; Очистка области атрибутов
; ----------------------------------------------------------------------
        ld      sp, $5b00
        ld      hl, $4747
        ld      bc, $8002
CLS1:   push    hl
        djnz    CLS1
        dec     c
        jr      nz, CLS1
; ----------------------------------------------------------------------

        ; Декомпрессия следующей картинки
        ld      sp, $ffff
        ld      hl, SCR1
        ld      de, DECOMP
        call    dzx0

        ; Залить область
        call    DRAWBG

        ; Перенос байта
        ld      a, 0
        ld      hl, DECOMP
        call    REDRAW

        ; --- печатается текст ---

        ld      de, str0

        ld      hl, $48c4       ; Начало текста
        ld      b, 21
M1:     ld      a, (de)
        inc     de
        call    PRN
        djnz    M1

halt

        jr      $

; ----------------------------------------------------------------------
; Обработка события IM 2
; ----------------------------------------------------------------------

IM2:    push    hl
        push    de
        push    bc
        push    af

        pop     af
        pop     bc
        pop     de
        pop     hl
        ret

; ----------------------------------------------------------------------
; Печать одного символа A в режиме телетайпа -> HL
; ----------------------------------------------------------------------

PRN:    push    hl
        push    de
        push    bc
        ex      de, hl          ; Сохранить HL
        ld      bc, FONTS
        ld      l, a
        ld      h, 0
        add     hl, hl
        add     hl, hl
        add     hl, hl
        add     hl, hl
        add     hl, bc
        ex      de, hl          ; DE = 16*A + FONTS
        ld      c, 2
PRN2:   ld      b, 8
PRN1:   ld      a, (de)         ; Нарисовать половину символа
        ld      (hl), a
        inc     de
        inc     h
        djnz    PRN1
        ld      a, h            ; Вернуть H на место
        sub     $08
        ld      h, a
        ld      a, l
        add     $20             ; Проверка на превышение L
        jr      nc, PRN3
        ex      af, af'
        ld      a, h
        add     $08             ; Перенос к следующему блоку
        ld      h, a
        ex      af, af'
PRN3:   ld      l, a
        dec     c
        jr      nz, PRN2
        pop     bc
        pop     de
        pop     hl
        inc     l
        ret

; ----------------------------------------------------------------------
; Отрисовка картинки HL, A-затемнение [0..8]
; ----------------------------------------------------------------------

REDRAW: push    hl              ; Сохранить для повторного использования
        push    af
        ld      iyl, a
        ld      iyh, 0
        ld      bc, DMASK0
        add     iy, iy
        add     iy, iy
        add     iy, iy
        add     iy, bc          ; IX = DMASK0 + 8*A
        ld      c, 104          ; 104 строки
        ld      (IYhold), iy    ; Сохранение указателя дизеринга
        ld      ix, YTABLE      ; Предвычисленная таблица Y-позиции
        ld      a, 8            ; Для циклического вращения IY
        ex      af, af'
DRAW2:  ld      d, (ix+1)       ; Следующий табличный адрес [0..103]
        ld      e, (ix+0)
        inc     ix
        inc     ix
        ld      a, (iy+0)       ; Маска дизеринга для затемнения
        ld      (MODIF+1), a    ; Модифицировать код для наложения маски
        ld      b, 25           ; 25 x 8 = 200 пикселей
; --- 1145T Рисование одной линии -----
DRAW1:  ld      a, (hl)         ; 7T Основной цикл рисования
MODIF:  or      $00             ; 7T Самомодицифирующийся код
        ld      (de), a         ; 7T
        inc     de              ; 6T
        inc     hl              ; 6T
        djnz    DRAW1           ; 13T/8T
; -------------------------------------
        inc     iy              ; К следующему циклу
        ex      af, af'
        dec     a
        jr      nz, DRAW3       ; Проверить, что не достигло 0
        ld      iy, (IYhold)    ; Восстановить IY
        ld      a, 8            ; Восстановить счетчик
DRAW3:  ex      af, af'
        dec     c               ; К следующей линии
        jr      nz, DRAW2
        pop     af
        pop     hl
        ret

; ----------------------------------------------------------------------
; Заливка области картинки
; ----------------------------------------------------------------------

DRAWBG: ld      hl, $5803
        ld      a, $30
        ld      de, 7
        ld      c, 13
LINE2:  ld      b, 25
LINE1:  ld      (hl), a
        inc     hl
        djnz    LINE1
        add     hl, de
        dec     c
        jr      nz, LINE2
        ret

; ----------------------------------------------------------------------
; Компрессированные данные со скринами (около 12 кб)
; ----------------------------------------------------------------------

        include "dzx0_standard.asm"
        include "data/string.asm"

YTABLE: incbin  "data/ytable.bin"
DMASK0: incbin  "data/dither.bin"
FONTS:  incbin  "data/fonts.bin"

SCR1:   incbin  "screen/screen1.bin"
SCR2:   incbin  "screen/screen2.bin"
SCR3:   incbin  "screen/screen3.bin"
SCR4:   incbin  "screen/screen4.bin"
SCR5:   incbin  "screen/screen5.bin"
SCR6:   incbin  "screen/screen6.bin"
SCR7:   incbin  "screen/screen7.bin"
SCR8:   incbin  "screen/screen8.bin"
SCR9:   incbin  "screen/screen9.bin"

; ----------------------------------------------------------------------
; Данные
; ----------------------------------------------------------------------

IYhold: defw    0
