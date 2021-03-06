#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <iomanip>

/**
 * Error: вычитание из меньшего большее, переполнение, деление на ноль, возведение нуля в нулевую
 * Количество десятичных разрядов целых чисел не превышает 100000. 
 * Основание выбранной системы счисления для внутреннего представления «длинных» чисел должно быть не меньше 10000.
 */

namespace BigInt {
    const int DefaultBase = 1e4;

    class BigInt {
    private:
        std::vector<int> Data;
        static const int Base = DefaultBase;
        static const int Digits = 4;
    public:
        BigInt() = default;
        BigInt(const std::string& str);
        BigInt(const BigInt& num);
        BigInt& operator=(const BigInt& num);
        ~BigInt() = default;

        friend bool operator<(const BigInt& lhs, const BigInt& rhs);
        friend bool operator>(const BigInt& lhs, const BigInt& rhs);
        friend bool operator==(const BigInt& lhs, const BigInt& rhs);

        friend std::istream& operator>>(std::istream& is, BigInt& num);
        friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

        friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator-(const BigInt& lhs, const BigInt& rhs); // exception
        friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator/(const BigInt& lhs, const BigInt& rhs); // exception

        static BigInt FastPow(const BigInt& lhs, const BigInt& rhs); // exception
        // + karatsuba + fdiv
    };
};