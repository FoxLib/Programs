// Текущее значение псевдослучайного числа
uint random_num;

void random_seed(uint n) {
    
    n = (n == 0) ? 0x125F2244 : n;
    random_num = n;
}

// LFSR | https://ru.wikipedia.org/wiki/Регистр_сдвига_с_линейной_обратной_связью
uint random_get() {
    
    uint S = random_num;
    random_num = ((((S >> 31) ^ (S >> 30) ^ (S >> 29) ^ (S >> 27) ^ (S >> 25) ^ S ) & 1) << 31 ) | (S >> 1);        
    return random_num;
}
