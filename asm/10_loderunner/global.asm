
; Текущий уровень
current_level       dw      level_01

; Сегмент фреймбуфера
seg_fb              dw      0
seg_a000            dw      0xA000

; Значение текущего скроллинга
; ----------------------------------------------------------------------

scroll_x            dw      0
scroll_y            dw      0

scroll_x_tile       dw      ?
scroll_y_tile       dw      ?
scroll_x_fine       dw      ?
scroll_y_fine       dw      ?
