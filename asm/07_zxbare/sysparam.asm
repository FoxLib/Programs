; Системные переменные
reg_r7f     db 0        ; Младшие биты R
reg_r8      db 0        ; Старший бит R
reg_i       db 0
reg_imode   db 0
reg_iff1    db 0
reg_iff2    db 0
_halted     db 0
delayed     db 0        ; Биты 0=EI, 1=DI

flags       db 0        ; Состояние флагов

; Цветовая таблица
colortable: db      0, 1, 4, 5, 2, 3, 6, 7
            db      0, 9,12,13,10,11,14,15

