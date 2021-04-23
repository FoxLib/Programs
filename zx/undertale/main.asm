
        org     $6600       ; Начало после Бейсик-программы
DECOMP: equ     $5b00       ; Буфер A28h (2600) байт для распаковки экрана

        di

        ; Очистка экрана
; ----------------------------------------------------------------------
        ld      sp, $5b00
        ld      hl, $4747       ; Очистка области атрибутов
        ld      bc, $8002
CLS1:   push    hl
        djnz    CLS1
        dec     c
        jr      nz, CLS1
        ld      hl, $0000       ; Очистка области данных
        ld      bc, $000c
CLS2:   push    hl
        djnz    CLS2
        dec     c
        jr      nz, CLS2
; ----------------------------------------------------------------------

        ; Установка прерывания IM2
        ld      a, $65
        ld      i, a
        ld      a, r
        and     $7f
        ld      r, a            ; Сброс бита R[7] если он там был
        ld      hl, IM2
        ld      sp, $65ff
        ex      (sp), hl
        im      2

        ; Первичная отрисовка
        call    AYINIT
        call    NEXTSCR         ; Загрузить следующий экран
        xor     a
        call    DRAWBG          ; Чтобы затереть белые атрибуты
        ld      a, (DithV)
        ld      hl, DECOMP
        call    REDRAW          ; "Невидимое" рисование
        ld      a, $30
        call    DRAWBG          ; Сверхскоростное появление на экране

        ; Ожидание появления события
        ; -------------------------------
MAIN:   ei
        ld      a, r
        and     $80
        jr      z, MAIN
        ld      a, r
        and     $7f
        ld      r, a

        ; -------------------------------
        ; Обновление, загрузка данных
        ; -------------------------------

        ld      a, (DithV)
        cp      $ff
        jr      nz, DRAWS
        call    NEXTSCR         ; Декомпрессия нового экрана
        jr      MAIN
DRAWS:  ld      hl, DECOMP
        call    REDRAW
        jr      MAIN

; ----------------------------------------------------------------------
; Обработчики и данные
; ----------------------------------------------------------------------

        ; Обработчики
        include "routines.asm"

        ; Декомпрессор
        include "dzx0_standard.asm"

        ; Список строк
STRG:   include "data/string.asm"

        ; Список событий
EVTLST: include "events.asm"

YTABLE: incbin  "data/ytable.bin"
DMASK0: incbin  "data/dither.bin"
FONTS:  incbin  "data/fonts.bin"

        ; Компрессированные данные со скринами (около 12 кб)
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

IYhold:     defw    0           ; Временное значение для IY (дизеринг)
DithV:      defb    0           ; Текущее значение затемнения (от 0 до 8)
EVAddr:     defw    EVTLST      ; Адрес последнего байта события в потоке

; Текущее событие
KEvent:     defb    0           ; Время (1/50 сек) до следующего события

; Таблица с экранами
SCRPTR:     defw    SCRLST
SCRLST:     defw    SCR1, SCR2, SCR3, SCR4, SCR5, SCR6, SCR7, SCR8, SCR9
NEXTSYM:    defw    str0        ; Адрес печатаемого символа
NEXTPOS:    defw    $5004       ; Первая позиция текста

