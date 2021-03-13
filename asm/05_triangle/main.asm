
        macro   brk { db 0xcc }
        org     100h

; Макрос умножения 2x2 матрицы (a*b - c*d) #1
macro   mat2a   a, b, c, d {
        mov     ax, [si+c]
        mov     bx, [si+a]
        mul     word [bp-d]
        xchg    ax, bx
        mul     word [bp-b]
        sub     ax, bx
        stosw
}
; Макрос умножения 2x2 матрицы (a*b - c*d) #2
macro   mat2b   a, b, c, d {
        mov     ax, [bp-c]
        mov     bx, [bp-a]
        mul     word [bp-d]
        xchg    ax, bx
        mul     word [bp-b]
        sub     ax, bx
        stosw
}
; Вычисление разности
macro   subvec  a, b, c {
        mov     ax, [si+b+c]
        sub     ax, [si+c]
        mov     [bp-a], ax
}
macro   increm  a, b {
        movsx   eax, word [b]
        add     [a], eax
}
; ----------------------------------------------------------------------

        mov     ax, 0x0013
        int     10h
        push    0xA000
        pop     es

@@:
        ; Создание треугольника
        mov     si, data_tri
        mov     di, data_com
        call    create_parameter_tri
        call    draw_screen

        ; Управление
        xor     ax, ax
        int     16h
        inc     word [data_tri + 4]    ; A.z++
        inc     word [data_tri + 4+6]  ; B.z++
        dec     word [data_tri + 4+12] ; C.z--
        cmp     al, 27
        jne     @b
        int     20h

; ----------------------------------------------------------------------
; Рисование треугольника
; ----------------------------------------------------------------------

draw_screen:

        xor     di, di
        mov     [.cursor], word 0
        mov     [calcuvd.dy], dword 100

        ; for (y = 0; y < 200; y++)
        mov     cx, 200
.L2:    push    cx

        ; Расчет координат
        mov     di, data_com
        call    calcuvd
        mov     cx, 320

        ; for (x = 0; x < 320; x++)
.L1:    mov     eax, [calcuvd.u]
        mov     ebx, [calcuvd.v]
        mov     edx, [calcuvd.D]
        mov     [.cl], byte 0

        ; if (u + v < D && u >= 0 && v >= 0) then
        sub     edx, eax
        sbb     edx, ebx
        js      @f
        or      eax, ebx
        js      @f

        ; (BL) u = 16*(u / D)
        ; --------------------------------------
        mov     eax, [calcuvd.v]
        mov     ebx, [calcuvd.D]
        shr     ebx, 4
        cdq
        div     ebx

        ; (AL) v = 16*(v / D)
        ; --------------------------------------
        push    ax
        mov     eax, [calcuvd.u]
        cdq
        div     ebx
        pop     bx

        ; Расчет цвета
        xor     al, bl
        or      al, 0x10
        mov     [.cl], al

        ; Рисование пикселя
@@:     mov     al, [.cl]
        mov     di, [.cursor]
        stosb                   ; Рисование пикселя
        mov     [.cursor], di

        ; Приращение u,v,D параметров
        increm  calcuvd.u, data_com+0
        increm  calcuvd.v, data_com+6
        increm  calcuvd.D, data_com+12

        ; Обработка одной строки
        dec     cx
        jne     .L1
        dec     [calcuvd.dy]
        pop     cx

        ; Обработка всего экрана
        dec     cx
        jne     .L2
        ret

.cursor dw      0
.cl     db      0

; ----------------------------------------------------------------------
calcuvd:

        call    .mult
        mov     [.u], eax   ; u = dx*A1 + dy*A2 + dz*A3
        call    .mult
        mov     [.v], eax   ; v = dx*B1 + dy*B2 + dz*B3
        call    .mult
        mov     [.D], eax   ; D = dx*C1 + dy*C2 + dz*C3
        ret

.mult:  ; Скалярное произведение d*vec3
        movsx   eax, word [di]          ; A.i
        mul     dword [.dx]
        xchg    eax, ebx
        movsx   eax, word [di+2]        ; B.i
        mul     dword [.dy]
        add     ebx, eax
        movsx   eax, word [di+4]        ; C.i
        mul     dword [.dz]
        add     eax, ebx
        add     di, 6
        ret

.dx     dd      -160
.dy     dd      100
.dz     dd      100
.u      dd      0
.v      dd      0
.D      dd      0

; ----------------------------------------------------------------------
; ВЫЧИСЛЕНИЕ ПАРАМЕТРОВ ТРЕУГОЛЬНИКА В ПРОСТРАНСТВЕ
; ----------------------------------------------------------------------
; Локальные переменные на стеке
; [bp-2] AB.x   [bp-4]  AB.y    [bp-6]  AB.z
; [bp-8] AC.x   [bp-10] AC.y    [bp-12] AC.z

; Запрошенные параметры треугольника
; [si+0]  A.x   [si+2]  A.y     [si+4]  A.z
; [si+6]  B.x   [si+8]  B.y     [si+10] B.z
; [si+12] B.x   [si+14] C.y     [si+16] C.z
; ----------------------------------------------------------------------

create_parameter_tri:

        push    di es
        push    ds
        pop     es
        push    bp
        mov     bp, sp
        sub     sp, 6*2             ; Выделение места на стеке

        ; Вычисление векторов AB, AC
        subvec   2,  6, 0           ; AB.x = B.x - A.x
        subvec   4,  6, 2           ; AB.y = B.y - A.y
        subvec   6,  6, 4           ; AB.z = B.z - A.z
        subvec   8, 12, 0           ; AC.x = C.x - A.x
        subvec  10, 12, 2           ; AC.y = C.y - A.y
        subvec  12, 12, 4           ; AC.z = C.z - A.z

        ; Вычисление и запись параметров
        mat2a   2, 12,  4, 10       ; A1 = (Ay*ACz - Az*ACy)
        mat2a   4,  8,  0, 12       ; A2 = (Az*ACx - Ax*ACz)
        mat2a   0, 10,  2,  8       ; A3 = (Ax*ACy - Ay*ACx)
        mat2a   4,  4,  2,  6       ; B1 = (Az*ABy - Ay*ABz)
        mat2a   0,  6,  4,  2       ; B2 = (Ax*ABz - Az*ABx)
        mat2a   2,  2,  0,  4       ; B3 = (Ay*ABx - Ax*ABy)
        mat2b   6, 10,  4, 12       ; C1 = (ABz*ACy - ABy*ACz)
        mat2b   2, 12,  6,  8       ; C2 = (ABx*ACz - ABz*ACx)
        mat2b   4,  8,  2, 10       ; C3 = (ABy*ACx - ABx*ACy)

        mov     sp, bp
        pop     bp es di
        ret

; ----------------------------------------------------------------------
data_tri:

        dw      -5,  5,  5      ; Точка A
        dw       5,  5,  6      ; Точка B
        dw      -5, -5,  7      ; Точка C

; Итоговые параметры треугольника (sizeof = 18 байт)
; ----------------------------------------------------------------------
data_com:       times 9 dw ?
