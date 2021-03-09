#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace NBigInt {
    const long long DefaultBase = 1e6;

    class TBigInt {
    private:
        std::vector<long long> Data;
        static const long long Base = DefaultBase;
        static const long long Digits = 6;

        void LeftShift(long long degree);
        static void Split(const TBigInt& num, TBigInt& lhs, TBigInt& rhs);
        static long long FindBin(const TBigInt& num, const TBigInt& div);
        void RemoveNulls();

    public:
        TBigInt() = default;
        TBigInt(const std::string& str);
        TBigInt(const TBigInt& num);
        TBigInt& operator=(const TBigInt& num);
        ~TBigInt() = default;

        friend bool operator<(const TBigInt& lhs, const TBigInt& rhs);
        friend bool operator>(const TBigInt& lhs, const TBigInt& rhs);
        friend bool operator==(const TBigInt& lhs, const TBigInt& rhs);

        friend std::istream& operator>>(std::istream& is, TBigInt& num);
        friend std::ostream& operator<<(std::ostream& os, const TBigInt& num);

        friend TBigInt operator+(const TBigInt& lhs, const TBigInt& rhs);
        friend TBigInt operator-(const TBigInt& lhs, const TBigInt& rhs);
        friend TBigInt operator*(const TBigInt& lhs, const TBigInt& rhs);
        friend TBigInt operator/(const TBigInt& lhs, const TBigInt& rhs);

        static TBigInt FastPow(TBigInt base, TBigInt degree);
        static TBigInt KaratsubaMult(TBigInt&& lhs, TBigInt&& rhs);
        static TBigInt WeakDivision(const TBigInt& num, long long div);
        static TBigInt WeakMultiply(const TBigInt& num, long long mul);
    };
};