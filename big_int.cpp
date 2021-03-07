#include "big_int.hpp"

namespace BigInt {
    BigInt::BigInt(const std::string& str) {
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

    BigInt::BigInt(const BigInt& num) {
        for (int i = 0; i < num.Data.size(); ++i) {
            Data.push_back(num.Data[i]);
        }
    }

    BigInt& BigInt::operator=(const BigInt& num) {
        Data.resize(num.Data.size());
        for (int i = 0; i < num.Data.size(); ++i) {
            Data[i] = num.Data[i];
        }
        return *this;
    }

    bool operator<(const BigInt& lhs, const BigInt& rhs) {
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

    bool operator>(const BigInt& lhs, const BigInt& rhs) {
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

    bool operator==(const BigInt& lhs, const BigInt& rhs) {
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

    std::istream& operator>>(std::istream& is, BigInt& num) {
        std::string str;
        is >> str;
        num = BigInt(str);
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const BigInt& num) {
        os << num.Data[num.Data.size() - 1];
        for (int i = num.Data.size() - 2; i >= 0; --i) {
            os << std::setfill('0') << std::setw(BigInt::Digits) << num.Data[i]; 
        }
        return os;
    }

    BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
        // больше или меньше по размеру вектора, а не по значению
        const BigInt& smaller = (lhs.Data.size() < rhs.Data.size()) ? lhs : rhs;
        const BigInt& bigger = (lhs.Data.size() >= rhs.Data.size()) ? lhs : rhs;
        BigInt res = bigger;
        int carry = 0;

        for (int i = 0; i < res.Data.size(); ++i) {
            int added = (i < smaller.Data.size()) ? smaller.Data[i] + carry : carry;
            res.Data[i] += added;
            carry = res.Data[i] / BigInt::Base;
            res.Data[i] %= BigInt::Base;
        }

        if (carry != 0) {
            res.Data.push_back(carry);
        }

        return res;
    }

    BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
        if (lhs < rhs) {
            throw std::logic_error("Subtract the larger from the smaller.");
        }

        BigInt res = lhs;
        int carry = 0;
        for (int i = 0; i < res.Data.size(); ++i) {
            int subtrahend = (i < rhs.Data.size()) ? carry + rhs.Data[i] : carry;
            res.Data[i] -= subtrahend;
            if (res.Data[i] < 0) {
                carry = 1;
                res.Data[i] += BigInt::Base;
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

    BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
        BigInt tmpRhs = rhs;
        BigInt tmpLhs = lhs;
        return BigInt::KaratsubaMult(tmpLhs, tmpRhs);
    }

    BigInt BigInt::WeakDivision(const BigInt& num, int div) {
        BigInt res(num);
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

    BigInt BigInt::WeakMultiply(const BigInt& num, int mul) {
        BigInt res;
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

    int BigInt::FindBin(const BigInt& num, const BigInt& div) {
        int down = 0;
        int up = Base;
        BigInt tmpRes;
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

    BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
        if (rhs.Data.size() == 1 && rhs.Data[0] == 0) {
            throw std::logic_error("Division by zero.");
        }

        BigInt res;
        int shift = lhs.Data.size() - rhs.Data.size();
        BigInt tmpRHS;
        BigInt tmpLHS;
        BigInt remainder = lhs;
        for (int i = shift; i >= 0; --i) {
            tmpRHS = rhs;
            tmpLHS = remainder;
            tmpRHS.BasePow(i);
            int quotient = BigInt::FindBin(tmpLHS, tmpRHS);
            res.Data.push_back(quotient);
            if (!(tmpLHS < BigInt::WeakMultiply(tmpRHS, quotient))) {
                remainder = tmpLHS - BigInt::WeakMultiply(tmpRHS, quotient);
            }
            // std::cout << "???: " << tmpLHS << " " << tmpRHS << " " << quotient << " " << remainder << "\n";
        }
        std::reverse(res.Data.begin(), res.Data.end());

        int i = res.Data.size() - 1;
        while (res.Data[i] == 0 && res.Data.size() > 1) {
            res.Data.pop_back();
            i = res.Data.size() - 1;
        }

        return res;
    }

    BigInt BigInt::FastPow(BigInt base, BigInt degree) {
        BigInt null("0");
        if (base == null && degree == null) {
            throw std::logic_error("Uncertainty, 0^0.");
        }
        BigInt res("1");
        // BigInt two("2");
        while (degree > null) {
            if (degree.Data.back() % 2 == 1) {
                res = res * base; 
            }
            base = base * base;
            degree = WeakDivision(degree, 2);;
            // std::cout << "\nBase: " << base << "\nDegree: " << degree << "\nRes: " << res << "\n";
        }
        return res;
    }

    void BigInt::BasePow(int degree) { // переименоватьб а то нихуя не ясно
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

    void BigInt::Split(const BigInt& num, BigInt& lhs, BigInt& rhs) {
        int half = num.Data.size() / 2;
        lhs.Data.resize(half);
        rhs.Data.resize(half);
        for (int i = 0; i < lhs.Data.size(); ++i) {
            lhs.Data[i] = num.Data[i + half];
            rhs.Data[i] = num.Data[i];
        }
    }

    BigInt BigInt::KaratsubaMult(BigInt& lhs, BigInt& rhs) {
        // дополняем числа до одного размера, и чтобы размер делился на 2
        int len = std::max(lhs.Data.size(), rhs.Data.size());
        if (len == 1) {
            BigInt res = lhs;
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

        BigInt firstTermLHS;
        BigInt secondTermLHS;
        Split(lhs, firstTermLHS, secondTermLHS);

        BigInt firstTermRHS;
        BigInt secondTermRHS;
        Split(rhs, firstTermRHS, secondTermRHS);

        // std::cout << "Split??: " << firstTermLHS << " " << secondTermLHS << "\n" <<
        // firstTermRHS << " " << secondTermRHS << "\n";

        BigInt resFirstTerm = KaratsubaMult(firstTermLHS, firstTermRHS);

        BigInt resSecondTerm1 = firstTermLHS + secondTermLHS;
        BigInt resSecondTerm2 = firstTermRHS + secondTermRHS;
        BigInt resSecondTerm = KaratsubaMult(resSecondTerm1, resSecondTerm2);

        BigInt resThirdTerm = KaratsubaMult(secondTermLHS, secondTermRHS);

        resSecondTerm = resSecondTerm - resFirstTerm - resThirdTerm;

        // std::cout << "??: " << resFirstTerm << " " << resSecondTerm << " " << resThirdTerm << "\n"; 

        resFirstTerm.BasePow(len);
        resSecondTerm.BasePow(len / 2);

        BigInt res = resFirstTerm + resSecondTerm + resThirdTerm;

        int carry = 0;
        for (int i = 0; i < res.Data.size(); ++i) {
            res.Data[i] += carry;
            carry = res.Data[i] / BigInt::Base;
            res.Data[i] %= BigInt::Base;
        }

        if (carry != 0) {
            res.Data.push_back(carry);
        }

        return res;
    }
};