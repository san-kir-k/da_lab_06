#include <string>
#include <vector>

/**
 * +
 * -
 * *
 * ^
 * /
 * > (true/false)
 * <
 * =
 * Error: вычитание из меньшего большее, переполнение, деление на ноль, возведение нуля в нулевую
 * Количество десятичных разрядов целых чисел не превышает 100000. 
 * Основание выбранной системы счисления для внутреннего представления «длинных» чисел должно быть не меньше 10000.
 */

namespace BigInt {
    const int DefaultBase = 1e4;

    // дополнить полями
    class BigInt {
    private:
        std::vector<int> Data;
        int Base = DefaultBase;
    public:
        BigInt() = default;
        BigInt(const std::string& str);
        ~BigInt() = default;

        friend bool operator<(const BigInt& lhs, const BigInt& rhs);
        friend bool operator>(const BigInt& lhs, const BigInt& rhs);
        friend bool operator==(const BigInt& lhs, const BigInt& rhs);

        friend std::istream& operator>>(std::istream& is, BigInt& val);
        friend std::istream& operator<<(std::istream& is, const BigInt& val);

        friend BigInt operator+(const BigInt& lhs, const BigInt& rhs); // ? чекнуть сигнатуру
        friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator/(const BigInt& lhs, const BigInt& rhs);
        friend BigInt operator^(const BigInt& lhs, const BigInt& rhs);
    };
};