#include "big_int.hpp"

namespace NBigInt {
    TBigInt::TBigInt(const std::string& str) {
        int startPosition = 0; // убираем ведущие нули
        while (startPosition < str.size() && str[startPosition] == '0') {
            ++startPosition;
        }

        if (startPosition == str.size()) { // если только ноль
            Data.push_back(0);
            return;
        }

        for (int i = str.size() - 1; i >= startPosition; i -= Digits) {
            int nextDigit = 0;
            int j = Digits - 1;
            while (j >= 0) {
                if (i - j >= startPosition) {
                    nextDigit = nextDigit * 10 + str[i - j] - '0';
                }
                --j;
            }
            Data.push_back(nextDigit);
        }
    }

    TBigInt::TBigInt(const TBigInt& num) {
        for (int i = 0; i < num.Data.size(); ++i) {
            Data.push_back(num.Data[i]);
        }
    }

    TBigInt& TBigInt::operator=(const TBigInt& num) {
        Data.resize(num.Data.size());
        for (int i = 0; i < num.Data.size(); ++i) {
            Data[i] = num.Data[i];
        }
        return *this;
    }

    bool operator<(const TBigInt& lhs, const TBigInt& rhs) {
        if (lhs.Data.size() != rhs.Data.size()) {
            return lhs.Data.size() < rhs.Data.size();
        }
        for (int i = lhs.Data.size() - 1; i >= 0; --i) {
            if (lhs.Data[i] != rhs.Data[i]) {
                return lhs.Data[i] < rhs.Data[i];
            }
        }
        return false; // lhs == rhs
    }

    bool operator>(const TBigInt& lhs, const TBigInt& rhs) {
        if (lhs.Data.size() != rhs.Data.size()) {
            return lhs.Data.size() > rhs.Data.size();
        }
        for (int i = lhs.Data.size() - 1; i >= 0; --i) {
            if (lhs.Data[i] != rhs.Data[i]) {
                return lhs.Data[i] > rhs.Data[i];
            }
        }
        return false; // lhs == rhs
    }

    bool operator==(const TBigInt& lhs, const TBigInt& rhs) {
        if (lhs.Data.size() != rhs.Data.size()) {
            return false; // lhs != rhs
        }
        for (int i = lhs.Data.size() - 1; i >= 0; --i) {
            if (lhs.Data[i] != rhs.Data[i]) {
                return false; // lhs != rhs
            }
        }
        return true; // lhs == rhs
    }

    std::istream& operator>>(std::istream& is, TBigInt& num) {
        std::string str;
        is >> str;
        num = TBigInt(str);
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const TBigInt& num) {
        os << num.Data[num.Data.size() - 1];
        for (int i = num.Data.size() - 2; i >= 0; --i) {
            os << std::setfill('0') << std::setw(TBigInt::Digits) << num.Data[i]; 
        }
        return os;
    }

    TBigInt operator+(const TBigInt& lhs, const TBigInt& rhs) {
        // больше или меньше по размеру вектора, а не по значению
        const TBigInt& smaller = (lhs.Data.size() < rhs.Data.size()) ? lhs : rhs;
        const TBigInt& bigger = (lhs.Data.size() >= rhs.Data.size()) ? lhs : rhs;
        TBigInt res = bigger;
        int carry = 0;

        for (int i = 0; i < res.Data.size(); ++i) {
            int added = (i < smaller.Data.size()) ? smaller.Data[i] + carry : carry;
            res.Data[i] += added;
            carry = res.Data[i] / TBigInt::Base;
            res.Data[i] %= TBigInt::Base;
        }

        if (carry != 0) {
            res.Data.push_back(carry);
        }

        return res;
    }

    TBigInt operator-(const TBigInt& lhs, const TBigInt& rhs) {
        if (lhs < rhs) {
            throw std::logic_error("Subtract the larger from the smaller.");
        }

        TBigInt res = lhs;
        int carry = 0;
        for (int i = 0; i < res.Data.size(); ++i) {
            int subtrahend = (i < rhs.Data.size()) ? carry + rhs.Data[i] : carry;
            res.Data[i] -= subtrahend;
            if (res.Data[i] < 0) {
                carry = 1;
                res.Data[i] += TBigInt::Base;
            } else {
                carry = 0;
            }
        }

        // второе условие чтобы 0 оставить, если числа равны
        int i = res.Data.size() - 1;
        while (res.Data[i] == 0 && res.Data.size() > 1) {
            res.Data.pop_back();
            i = res.Data.size() - 1;
        }
        return res;
    }

    TBigInt operator*(const TBigInt& lhs, const TBigInt& rhs) {
        TBigInt tmpRhs = rhs;
        TBigInt tmpLhs = lhs;
        return TBigInt::KaratsubaMult(tmpLhs, tmpRhs);
    }

    TBigInt TBigInt::WeakDivision(const TBigInt& num, int div) {
        TBigInt res(num);
        int carry = 0;
        int j;
        if (res.Data[res.Data.size() - 1] < div) {
            j = res.Data.size() - 2;
            carry = res.Data[res.Data.size() - 1];
            res.Data.pop_back();
        } else {
            j = res.Data.size() - 1;
        }
        for (int i = j; i >= 0; --i) {
            res.Data[i] = (carry * Base + num.Data[i]) / div;
            carry = num.Data[i] % div;
        }
        return res;
    }

    TBigInt TBigInt::WeakMultiply(const TBigInt& num, int mul) {
        TBigInt res;
        int carry = 0;
        for (int i = 0; i < num.Data.size(); ++i) {
            res.Data.push_back(num.Data[i] * mul % Base + carry);
            carry = num.Data[i] * mul / Base;
        }
        if (carry != 0) {
            res.Data.push_back(carry);
        }
        return res;
    }

    int TBigInt::FindBin(const TBigInt& num, const TBigInt& div) {
        int down = 0;
        int up = Base;
        TBigInt tmpRes;
        while (up - 1 > down) {
            int mid = (up + down) / 2;
            tmpRes = WeakMultiply(div, mid);
            if (num > tmpRes) {
                down = mid;
            } else if (num < tmpRes) {
                up = mid;
            } else {
                return mid;
            }
        }
        return (down + up) / 2;
    }

    TBigInt operator/(const TBigInt& lhs, const TBigInt& rhs) {
        if (rhs.Data.size() == 1 && rhs.Data[0] == 0) {
            throw std::logic_error("Division by zero.");
        }

        TBigInt res;
        int shift = lhs.Data.size() - rhs.Data.size();
        TBigInt tmpRHS;
        TBigInt tmpLHS;
        TBigInt remainder = lhs;
        for (int i = shift; i >= 0; --i) {
            tmpRHS = rhs;
            tmpLHS = remainder;
            tmpRHS.LeftShift(i);
            int quotient = TBigInt::FindBin(tmpLHS, tmpRHS);
            res.Data.push_back(quotient);
            if (!(tmpLHS < TBigInt::WeakMultiply(tmpRHS, quotient))) {
                remainder = tmpLHS - TBigInt::WeakMultiply(tmpRHS, quotient);
            }
        }
        std::reverse(res.Data.begin(), res.Data.end());

        int i = res.Data.size() - 1;
        while (res.Data[i] == 0 && res.Data.size() > 1) {
            res.Data.pop_back();
            i = res.Data.size() - 1;
        }

        return res;
    }

    TBigInt TBigInt::FastPow(TBigInt base, TBigInt degree) {
        TBigInt null("0");
        if (base == null && degree == null) {
            throw std::logic_error("Uncertainty, 0^0.");
        }
        TBigInt res("1");
        while (degree > null) {
            if (degree.Data.back() % 2 == 1) {
                res = res * base; 
            }
            base = base * base;
            degree = WeakDivision(degree, 2);;
        }
        return res;
    }

    void TBigInt::LeftShift(int degree) { 
        if (Data.size() == 1 && Data[0] == 0) {
            return;
        }
        int oldEnd = Data.size() - 1;
        Data.resize(Data.size() + degree);
        for (int i = oldEnd; i >= 0; --i) {
            Data[i + degree] = Data[i];
        }
        for (int i = 0; i < degree; ++i) {
            Data[i] = 0;
        }
    }

    void TBigInt::Split(const TBigInt& num, TBigInt& lhs, TBigInt& rhs) {
        int half = num.Data.size() / 2;
        lhs.Data.resize(half);
        rhs.Data.resize(half);
        for (int i = 0; i < lhs.Data.size(); ++i) {
            lhs.Data[i] = num.Data[i + half];
            rhs.Data[i] = num.Data[i];
        }
    }

    TBigInt TBigInt::KaratsubaMult(TBigInt& lhs, TBigInt& rhs) {
        // дополняем числа до одного размера, и чтобы размер делился на 2
        int len = std::max(lhs.Data.size(), rhs.Data.size());
        if (len == 1) {
            TBigInt res = lhs;
            res.Data[0] *= rhs.Data[0];
            if (res.Data[0] > Base) {
                int carry = res.Data[0] / Base;
                res.Data[0] %= Base;
                res.Data.push_back(carry);
            }
            return res;
        }
        len = (len % 2 != 0) ? len + 1 : len;

        for (int i = lhs.Data.size(); i < len; ++i) {
            lhs.Data.push_back(0);
        }
        for (int i = rhs.Data.size(); i < len; ++i) {
            rhs.Data.push_back(0);
        }

        TBigInt firstTermLHS;
        TBigInt secondTermLHS;
        Split(lhs, firstTermLHS, secondTermLHS);

        TBigInt firstTermRHS;
        TBigInt secondTermRHS;
        Split(rhs, firstTermRHS, secondTermRHS);

        TBigInt resFirstTerm = KaratsubaMult(firstTermLHS, firstTermRHS);

        TBigInt resSecondTerm1 = firstTermLHS + secondTermLHS;
        TBigInt resSecondTerm2 = firstTermRHS + secondTermRHS;
        TBigInt resSecondTerm = KaratsubaMult(resSecondTerm1, resSecondTerm2);

        TBigInt resThirdTerm = KaratsubaMult(secondTermLHS, secondTermRHS);

        resSecondTerm = resSecondTerm - resFirstTerm - resThirdTerm;

        resFirstTerm.LeftShift(len);
        resSecondTerm.LeftShift(len / 2);

        TBigInt res = resFirstTerm + resSecondTerm + resThirdTerm;

        int carry = 0;
        for (int i = 0; i < res.Data.size(); ++i) {
            res.Data[i] += carry;
            carry = res.Data[i] / TBigInt::Base;
            res.Data[i] %= TBigInt::Base;
        }

        if (carry != 0) {
            res.Data.push_back(carry);
        }

        return res;
    }
};
