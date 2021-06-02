
            org     7c00h

            cli
            cld
            mov     sp, $7c00
            mov     ah, 42h
            mov     si, DAP
            int     13h
            jb      cannot
            jmp     $8000:$0000

cannot:     jmp     $


DAP:        db 10h, 00h
            dw 0040h ; 32kb
            dw 0000h, 0800h ; Адрес загрузки
            dq 1
