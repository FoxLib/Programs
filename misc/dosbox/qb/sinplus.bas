SCREEN 13

pi = 3.141592

a1 = .2: b1 = 1
a2 = .3: b2 = 0

' Calc
tgx = (a1 * SIN(b1) + a2 * SIN(b2)) / (a1 * COS(b1) + a2 * COS(b2))

b = ATN(tgx)
a = (a1 * SIN(b1) + a2 * SIN(b2)) / SIN(b)

LINE (0, 100)-(319, 100), 8
LINE (160, 0)-(160, 199), 8

PRINT "b="; b

FOR x = 0 TO 319

  r = 2 * pi * (x / 319)

  y1 = a1 * SIN(r + b1)
  PSET (x, 100 - y1 * 100), 3

  y2 = a2 * SIN(r + b2)
  PSET (x, 100 - y2 * 100), 2

  ' ----- RES -----
  'y = y1 + y2
  'PSET (x, 100 - y * 100), 15
  y = a * SIN(r + b)
  PSET (x, 100 - y * 100), 15

  y = a1 * COS(r + b1) + a2 * COS(r + b2)
  PSET (x, 100 - y * 100), 12

  IF y < .005 AND y > -.005 THEN PRINT r

NEXT

