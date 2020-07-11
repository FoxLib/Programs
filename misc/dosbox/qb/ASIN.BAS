DECLARE FUNCTION nod! (a!, b!)
CLS

' a/b = (2n)! / (4^n * (n!)^2 * (2n + 1)
' 2! = 1*2      = 2    = 1
'    4*(1!)*3 = 4*3  = 6

x = .75

c = 1
k = x
m = x

' a *= x^2(2i-1)^2 Top
' b *= 2i(2i+1) Bottom

FOR i = 1 TO 8

  ' --------------------------
  k = k * c / (2 + c) ' Step 1
  k = k * c / (2 * i) ' Step 2
  k = k * x * x
  c = c + 2 ' Increment
  m = m + k
  ' --------------------------

  PRINT i; "="; k; m
             
NEXT

FUNCTION nod (at!, bt!)

  a = at: b = bt

  ' a = bq + r  ==> NOD(a,b) = NOD(b, r)

  WHILE (b > 0)
     ax = b: b = a MOD b: a = ax
  WEND

  nod = a

END FUNCTION

