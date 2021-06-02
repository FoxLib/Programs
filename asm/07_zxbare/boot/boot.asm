
            org     7c00h
            macro   brk { xchg bx, bx }

            cli
            cld
            xor     ax, ax
            mov     ds, ax
            mov     es, ax
            mov     ss, ax
            mov     sp, $7c00
            mov     ah, 42h
            mov     si, DAP
            int     13h
            jb      cannot
            jmp     $0800:$0000
cannot:     jmp     $


; Загрузка главной программы с диска [потом переделать загрузку с fat32]
DAP:        db 10h
            db 00h
            dw 0040h        ; 60h = 32kb
            dw 0000h
            dw 0800h        ; Адрес загрузки
            dq 1

times       7c00h + (512-2)-$ db 0x00
dw          0xAA55
