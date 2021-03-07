#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace NBigInt {
    const int DefaultBase = 1e4;

    class TBigInt {
    private:
        std::vector<int> Data;
        static const int Base = DefaultBase;
        static const int Digits = 4;

        void LeftShift(int degree);
        static void Split(const TBigInt& num, TBigInt& lhs, TBigInt& rhs);
        static int FindBin(const TBigInt& num, const TBigInt& div);

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
        static TBigInt KaratsubaMult(TBigInt& lhs, TBigInt& rhs);
        static TBigInt WeakDivision(const TBigInt& num, int div);
        static TBigInt WeakMultiply(const TBigInt& num, int mul);
    };
};