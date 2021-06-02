; Загрузка ROM-образа в память DS:0
load_rom:
            push    ax
            mov     [DAP+6], ax
            mov     ah, 42h
            mov     si, DAP
            mov     dl, $80
            int     13h
            pop     ax
            ret

; Вообще это так не должно быть
DAP:        db 10h
            db 00h
            dw 0060h        ; 60h = 48kb
            dw 0000h
            dw 0800h        ; Адрес загрузки
            dq 129
