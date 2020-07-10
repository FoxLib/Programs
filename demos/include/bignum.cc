// Можно задать извне BIGNUM_SIZE и BIGNUM_FRAC перед подключением

#ifndef BIGNUM_SIZE
#define BIGNUM_SIZE 6  // 0..5
#endif

#ifndef BIGNUM_FRAC
#define BIGNUM_FRAC 4  // Должно быть BIGNUM_FRAC < BIGNUM_SIZE-1
#endif

#ifndef ulong
#define ulong unsigned long long
#endif

class bignum {

protected:

    // Инициализацировать число
    void construct() {

        sign  = 1;
        for (int i = 0; i < BIGNUM_SIZE; i++)
            vbuf[i] = 0;
    }

    // Беззнаковое сложение
    int unsigned_add(bignum& op1, bignum& op2) {

        ulong carry = 0, val;

        // Сложение дробей и целого числа
        for (int i = 0; i < BIGNUM_SIZE; i++) {

            val     = carry + (ulong) op1.vbuf[i] + (ulong) op2.vbuf[i];
            vbuf[i] = val & 0xFFFFFFFF;
            carry   = (val >> 32);
        }

        return vbuf[BIGNUM_SIZE-1] >> 31;
    }

    // Беззнаковое вычитание
    void unsigned_sub(bignum& op1, bignum& op2) {

        ulong carry = 0, val;
        sign = 1;

        // Вычитание дробей и целого числа
        for (int i = 0; i < BIGNUM_SIZE; i++) {

            val     = (ulong) op1.vbuf[i] - (ulong) op2.vbuf[i] - carry;
            vbuf[i] = val & 0xFFFFFFFF;
            carry   = (val >> 32) ? 1 : 0;
        }

        // Сделать инверсию
        if (carry != 0) {

            sign = -sign;
            for (int i = 0; i < BIGNUM_SIZE; i++) {

                val     = ((ulong) vbuf[i] ^ 0xFFFFFFFF) + carry;
                vbuf[i] = val & 0xFFFFFFFF;
                carry   = (val >> 32) ? 1 : 0;
            }
        }
    }

    // Загрузить длинное целое число
    void load_integer(long long value) {

        int k = BIGNUM_FRAC;
        for (int i = BIGNUM_FRAC; i < BIGNUM_SIZE; i++) vbuf[i] = 0;

        // Заполнить число
        while (value) { vbuf[k++] = value & 0xFFFFFFFF; value >>= 32; }
    }

    // Загрузка числа типа double
    void load_fractional(double value) {

        for (int i = 1; i <= BIGNUM_FRAC; i++) {

            value  = value - (long long)value;
            value *= 0x100000000;
            vbuf[BIGNUM_FRAC - i] = value;
        }
    }

public:

    char          sign;                 // Знак числа (1 или -1)
    unsigned int  vbuf[BIGNUM_SIZE];    // Буфер числа

    // Конструкторы
    bignum() {
        construct();
    }

    // Загрузка 64-х битного целого
    bignum(long long value) {

        construct();
        if (value < 0) { value = -value; sign = -1; }
        load_integer(value);
    }

    // Загрузка 32-х битного целого
    bignum(int value) {

        construct();
        if (value < 0) { value = -value; sign = -1; }
        load_integer(value);
    }

    // Конвертация строки
    bignum(const char* value) {

        construct();
    }

    // Загрузка 64-х битного двойной точности
    bignum(double value) {

        construct();
        if (value < 0) { value = -value; sign = -1; }
        load_integer(value);
        load_fractional(value);
    }

    // Конструктор копирования
    bignum(const bignum& src) {

        sign = src.sign;
        for (int i = 0; i < BIGNUM_SIZE; i++) vbuf[i] = src.vbuf[i];
    }

    // Конструктор копирования
    bignum& operator=(const bignum& src) {

        sign = src.sign;
        for (int i = 0; i < BIGNUM_SIZE; i++) vbuf[i] = src.vbuf[i];
        return *this;
    }

    // Перегрузка оператора
    template <class T> bignum operator+(T b) { bignum x, y(b); x.add(*this, y); return x; }
    template <class T> bignum operator-(T b) { bignum x, y(b); x.sub(*this, y); return x; }
    template <class T> bignum operator*(T b) { bignum x, y(b); x.mul(*this, y); return x; }

    // -----------------------------------------------------------------

    // Печать целой части числа
    void print_integer() {

        int flag;
        unsigned int rem;

        int  bufcursor = (32*BIGNUM_SIZE) >> 1;
        char tmp[bufcursor];
        unsigned int tbuf[BIGNUM_SIZE];

        if (sign < 0) printf("-");

        // Оперировать во временном буфере
        memcpy(tbuf, vbuf, sizeof(unsigned int) * BIGNUM_SIZE);

        // Пишется 0 в конце строки
        tmp[--bufcursor] = 0;

        do {

            flag = 0;
            rem  = 0;

            // Начать с конца и завершить на точке
            for (int i = BIGNUM_SIZE - 1; i >= BIGNUM_FRAC; i--) {

                // Разделить число с заемом
                ulong val = ((ulong)rem << 32) + tbuf[i];

                // Сформировать новый заем
                tbuf[i] = val / 10;
                rem     = val % 10;

                // Все еще остается число
                if (tbuf[i] != 0) flag = 1;
            }

            // Число формируется в обратном порядке
            tmp[--bufcursor] = rem + '0';
        }
        while (flag);

        printf("%s", tmp + bufcursor);
    }

    // Печать дробной части
    void print_fractional(int maxsize) {

        unsigned int tbuf[BIGNUM_SIZE];
        char obuf[ 32*BIGNUM_FRAC ];

        memcpy(tbuf, vbuf, sizeof(unsigned int) * BIGNUM_SIZE);

        int cursor = 0;
        int carry  = 0;

        for (int k = 0; k < maxsize; k++) {

            // Умножать от дальнего к ближнему
            for (int i = 0; i < BIGNUM_FRAC; i++) {

                // Следующее число
                ulong val = (ulong) tbuf[i] * 10 + carry;

                // Предыдущий перенос
                tbuf[i] = (val & 0xFFFFFFFF);

                // Новый перенос
                carry = (val >> 32);
            }

            obuf[cursor++] = carry + '0';
        }

        // Удалить нули в конце
        while (cursor > 1 && obuf[cursor - 1] == '0') cursor--;

        // Ограничить строку
        obuf[cursor] = 0;
        printf(".%s", obuf);
    }

    // Печать полного числа
    void print() {

        print_integer();
        print_fractional(11 * BIGNUM_FRAC);
    }

    // Отладка
    void debug() {

        printf("\n");
        for (int i = BIGNUM_SIZE - 1; i >= 0; i--) {

            printf(" %08x", vbuf[i]);
            if (i == BIGNUM_FRAC) printf(" | ");
            if ((i % 16) == 0) printf("\n");
        }
        printf("---\n");
    }

    // -----------------------------------------------------------------
    // Вычисления
    // -----------------------------------------------------------------

    // Сложение двух чисел
    void add(bignum& op1, bignum& op2) {

        // a < 0, b > 0
        if (op1.sign == -1 && op2.sign == 1) {
             unsigned_sub(op2, op1);
        }
        // a > 0, b < 0
        else if (op1.sign == 1 && op2.sign == -1) {
             unsigned_sub(op1, op2);
        }
        // a < 0, b < 0 || a > 0, b > 0
        else {

            sign = op1.sign;
            unsigned_add(op1, op2);
        }
    }

    // Негатив для вычисления в дополненном коде
    void neg(bignum* op) {

        int carry = 1;
        for (int i = 0; i < BIGNUM_SIZE; i++) {

            ulong val   = ((ulong)op->vbuf[i] ^ 0xFFFFFFFF) + carry;
            op->vbuf[i] = val & 0xFFFFFFFF;
            carry       = (val >> 32) ? 1 : 0;
        }
    }

    // Вычитание двух чисел
    void sub(bignum op1, bignum op2) {

        // -a-(+b) = -(a+b)
        if (op1.sign == -1 && op2.sign == 1) {

            sign = -1;
            unsigned_add(op1, op2);
        }
        // a-(-b) = a + b
        else if (op1.sign == 1 && op2.sign == -1) {

            sign = 1;
            unsigned_add(op1, op2);
        }
        // -a-(-b) = b - a; -b>0, a<0
        else if (op1.sign == -1 && op2.sign == -1) {
            unsigned_sub(op2, op1);
        }
        // a-b
        else {
            unsigned_sub(op1, op2);
        }
    }

    // Умножение
    int mul(bignum& op1, bignum& op2) {

        unsigned int nbuf[2*BIGNUM_SIZE];
        ulong carry, carry_sum, val, sum;

        // Очистить буфер
        for (int i = 0; i < 2*BIGNUM_SIZE; i++) nbuf[i] = 0;

        // Просмотр BIGNUM_SIZE столбцов
        for (int i = 0; i < BIGNUM_SIZE; i++) {

            carry = 0;
            carry_sum = 0;

            for (int j = 0; j < BIGNUM_SIZE; j++) {

                val   = (ulong) op1.vbuf[j] * (ulong) op2.vbuf[i] + carry;
                carry = val >> 32;
                val  &= 0xFFFFFFFF;

                // Итоговое сложение
                sum          = (ulong) nbuf[i + j] + val + carry_sum;
                carry_sum    = sum >> 32;
                nbuf[i + j]  = sum & 0xFFFFFFFF;
            }
        }

        // Заполнить конечный результат
        for (int i = 0; i < BIGNUM_SIZE; i++)
            vbuf[i] = nbuf[i + BIGNUM_FRAC];

        sign = op1.sign * op2.sign;
        return carry_sum ? 1 : 0;
    }

    // Сравнить число. Если число меньше op2, то -1, если больше то 1
    int cmp(bignum& op2) {

        // Однозначно отрицательное
        if (sign == -1 && op2.sign == 1)
            return -1;
        else if (sign == 1 && op2.sign == -1)
            return 1;

        // Проверка всего числа
        for (int i = BIGNUM_SIZE - 1; i >= 0; i--) {

            // Нарушение равенства
            if (vbuf[i] != op2.vbuf[i]) {

                // Если число меньше, то 1 иначе 0
                int less = vbuf[i] < op2.vbuf[i] ? -1 : 1;
                return (sign == 1 && op2.sign == 1 ? less : -less);
            }
        }

        // Число оказалось равным
        return 0;
    }
};
