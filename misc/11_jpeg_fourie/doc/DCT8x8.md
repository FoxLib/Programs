# Discrete Cosine Transform

Источник http://www.math.uaa.alaska.edu/~ssiewert/a385_doc/dct8x8.pdf

## Преобразование для 8x8 (DCT)

T[i,j] = c(i) с(j) sum[x=0..N-1] sum[y=0..N-1] V[y,x] cos ((2y+1)*i*pi/(2N)) * cos ((2x+1)*j*pi/(2N))

c(i,j) = 2/N, i <> 0 и j <> 0
c(i,j) = 1/N, i == 0 или j == 0

# Обратное преобразование (IDCT)

# -- перепроверять
V[y,x] = sum[i=0..N-1] sum[j=0..N-1] c(i)c(j) T[i,j] cos ((2i+1)*y*pi/(2N)) * cos ((2j+1)*x*pi/(2N))