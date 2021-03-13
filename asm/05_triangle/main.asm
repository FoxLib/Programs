
        macro   brk { db 0xcc }
        org     100h

; Макрос умножения 2x2 матрицы (a*b - c*d) #1
macro   mat2a a, b, c, d {
        mov     ax, [si+c]
        mov     bx, [si+a]
        mul     word [bp-d]
        xchg    ax, bx
        mul     word [bp-b]
        sub     ax, bx
        stosw
}
; Макрос умножения 2x2 матрицы (a*b - c*d) #2
macro   mat2b a, b, c, d {
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
; ----------------------------------------------------------------------

        ; Создание треугольника
        mov     si, data_tri
        mov     di, data_com
        call    create_parameter_tri
brk
        ; Расчет координат

        movzx   eax, word [di]
        mul     dword [.dx]
        xchg    eax, ebx
        movzx   eax, word [di+2]
        mul     dword [.dy]
        add     ebx, eax
        movzx   eax, word [di+4]
        mul     dword [.dz]
        add     eax, ebx

        int     20h

.dx     dd      -160
.dy     dd      100
.dz     dd      100

; ----------------------------------------------------------------------
calculate_uvd:

        ; u = dx*A1 + dy*A2 + dz*A3
        ; v = dx*B1 + dy*B2 + dz*B3
        ; D = dx*C1 + dy*C2 + dz*C3

        ret

; Скалярное произведение d*vec3
.scalar_mul:

        movzx   eax, word [di]
        mul     dword [.dx]
        xchg    eax, ebx
        movzx   eax, word [di+2]
        mul     dword [.dy]
        add     ebx, eax
        movzx   eax, word [di+4]
        mul     dword [.dz]
        add     eax, ebx
        add     di, 6
        ret

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

        dw      -5,  5, 15      ; Точка A
        dw       5,  5,  6      ; Точка B
        dw      -5, -5,  6      ; Точка C

; Итоговые параметры треугольника (sizeof = 18 байт)
; ----------------------------------------------------------------------
data_com:       times 9 dw ?

