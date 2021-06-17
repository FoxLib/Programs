; Для отслеживания обновления кадра
clock_time          dw      0

; Текущий уровень
current_level       dw      level_01
level_width         dw      160         ; Ширина уровня

; Сегмент фреймбуфера
seg_fb              dw      0
seg_a000            dw      0xA000

; Движения
; ----------------------------------------------------------------------

player_counter      db      0
player_anim_state   db      0
player_on_ladder    db      0

; Значение текущего скроллинга
; ----------------------------------------------------------------------

scroll_x            dw      0
scroll_y            dw      0

; Не инициализировано
; ----------------------------------------------------------------------

scroll_x_tile       dw      ?
scroll_y_tile       dw      ?
scroll_x_fine       dw      ?
scroll_y_fine       dw      ?

keyboard:           times 128 db ?

