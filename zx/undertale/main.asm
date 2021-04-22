
        org     $5b00

        di

        ; Декомпрессия следующей картинки
        ld      hl, SCR1
        ld      de, decompress
        call    dzx0

        ; Залить область
        call    DRAWBG

        ; Перенос байта
        ld      hl, decompress
        ld      iy, DMASK0+8*0
        call    REDRAW
        jr      $

; ----------------------------------------------------------------------
; Отрисовка картинки
; ----------------------------------------------------------------------

REDRAW: push    hl              ; Чтобы не писать заново
        ld      c, 104          ; 104 строки
        ld      (IYhold), iy    ; Сохранение указателя дизеринга
        ld      ix, YTABLE      ; Предвычисленная таблица Y-позиции
        ld      a, 8            ; Для циклического вращения IY
        ex      af, af'

        ; Следующий табличный адрес [0..103]
DRAW2:  ld      d, (ix+1)
        ld      e, (ix+0)
        inc     ix
        inc     ix
        ld      a, (iy+0)       ; Маска покрытия
        ld      (MODIF+1), a    ; Модифицировать код для наложения маски
        ld      b, 25           ; 25 x 8 = 200 пикселей
        ; --- 1145T Рисование одной линии
DRAW1:  ld      a, (hl)         ; 7T Основной цикл рисования
MODIF:  or      $00             ; 7T Самомодицифирующийся код
        ld      (de), a         ; 7T
        inc     de              ; 6T
        inc     hl              ; 6T
        djnz    DRAW1           ; 13T/8T
        ; ---
        inc     iy              ; К следующему циклу
        ex      af, af'
        dec     a
        jr      nz, DRAW3       ; Проверить, что не достигло 0
        ld      iy, (IYhold)    ; Восстановить IY
        ld      a, 8            ; Восстановить счетчик
DRAW3:  ex      af, af'
        dec     c               ; К следующей линии
        jr      nz, DRAW2
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
; Данные
; ----------------------------------------------------------------------

IYhold: defw    0

; ----------------------------------------------------------------------
; Компрессированные данные со скринами (около 12 кб)
; ----------------------------------------------------------------------

        include "dzx0_standard.asm"
YTABLE: incbin  "data/ytable.bin"
DMASK0: incbin  "data/dither.bin"

SCR1:   incbin  "screen/screen1.bin"
SCR2:   incbin  "screen/screen2.bin"
SCR3:   incbin  "screen/screen3.bin"
SCR4:   incbin  "screen/screen4.bin"
SCR5:   incbin  "screen/screen5.bin"
SCR6:   incbin  "screen/screen6.bin"
SCR7:   incbin  "screen/screen7.bin"
SCR8:   incbin  "screen/screen8.bin"
SCR9:   incbin  "screen/screen9.bin"

decompress:
