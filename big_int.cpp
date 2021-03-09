#include "big_int.hpp"

namespace NBigInt {
    TBigInt::TBigInt(const std::string& str) {
        long long startPosition = 0; // убираем ведущие нули
        while (startPosition < str.size() && str[startPosition] == '0') {
            ++startPosition;
        }

        if (startPosition == str.size()) { // если только ноль
            Data.push_back(0);
            return;
        }

        for (long long i = str.size() - 1; i >= startPosition; i -= Digits) {
            long long nextDigit = 0;
            long long j = Digits - 1;
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
        for (long long i = 0; i < num.Data.size(); ++i) {
            Data.push_back(num.Data[i]);
        }
    }

    TBigInt& TBigInt::operator=(const TBigInt& num) {
        Data.resize(num.Data.size());
        for (long long i = 0; i < num.Data.size(); ++i) {
            Data[i] = num.Data[i];
        }
        return *this;
    }

    bool operator<(const TBigInt& lhs, const TBigInt& rhs) {
        if (lhs.Data.size() != rhs.Data.size()) {
            return lhs.Data.size() < rhs.Data.size();
        }
        for (long long i = lhs.Data.size() - 1; i >= 0; --i) {
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
        for (long long i = lhs.Data.size() - 1; i >= 0; --i) {
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
        for (long long i = lhs.Data.size() - 1; i >= 0; --i) {
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
        for (long long i = num.Data.size() - 2; i >= 0; --i) {
            os << std::setfill('0') << std::setw(TBigInt::Digits) << num.Data[i]; 
        }
        return os;
    }

    TBigInt operator+(const TBigInt& lhs, const TBigInt& rhs) {
        // больше или меньше по размеру вектора, а не по значению
        const TBigInt& smaller = (lhs.Data.size() < rhs.Data.size()) ? lhs : rhs;
        const TBigInt& bigger = (lhs.Data.size() >= rhs.Data.size()) ? lhs : rhs;
        TBigInt res(bigger);
        long long carry = 0;

        for (long long i = 0; i < res.Data.size(); ++i) {
            long long added = (i < smaller.Data.size()) ? smaller.Data[i] + carry : carry;
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

        TBigInt res(lhs);
        long long carry = 0;
        for (long long i = 0; i < res.Data.size(); ++i) {
            long long subtrahend = (i < rhs.Data.size()) ? carry + rhs.Data[i] : carry;
            res.Data[i] -= subtrahend;
            if (res.Data[i] < 0) {
                carry = 1;
                res.Data[i] += TBigInt::Base;
            } else {
                carry = 0;
            }
        }

        res.RemoveNulls();

        return res;
    }

    TBigInt operator*(const TBigInt& lhs, const TBigInt& rhs) {
        if (rhs.Data.size() == 1) {
            return TBigInt::WeakMultiply(lhs, rhs.Data[0]);
        } else if (lhs.Data.size() == 1) {
            return TBigInt::WeakMultiply(rhs, lhs.Data[0]);
        }

        TBigInt res;
        res.Data.resize(lhs.Data.size() + rhs.Data.size());
        for (long long i = 0; i < lhs.Data.size() + rhs.Data.size(); ++i) {
            res.Data.push_back(0);
        }

        for (long long i = 0; i < lhs.Data.size(); ++i) {
            for (long long j = 0, carry = 0; j < rhs.Data.size() || carry; ++j) {
                long long curr = res.Data[i+j] + lhs.Data[i] * (j < rhs.Data.size() ? rhs.Data[j] : 0) + carry;
                res.Data[i+j] = curr % TBigInt::Base;
                carry = curr / TBigInt::Base;
            }
        }

        res.RemoveNulls();

        return res;
    }

    TBigInt TBigInt::WeakDivision(const TBigInt& num, long long div) {
        TBigInt res(num);
        long long carry = 0;
        for (long long i = res.Data.size() - 1; i >= 0; --i) {
            long long curr = res.Data[i] + carry * TBigInt::Base;
            res.Data[i] = curr / div;
            carry = curr % div;
        }

        res.RemoveNulls();

        return res;
    }

    TBigInt TBigInt::WeakMultiply(const TBigInt& num, long long mul) {
        TBigInt res(num);
        long long carry = 0;
        for (long long i = 0; i < res.Data.size() || carry > 0; ++i) {
            if (i == res.Data.size()) {
                res.Data.push_back(0);
            }
            long long curr = carry + res.Data[i] * mul;
            res.Data[i] = curr % TBigInt::Base;
            carry = curr / TBigInt::Base;
        }

        res.RemoveNulls();

        return res;
    }

    long long TBigInt::FindBin(const TBigInt& num, const TBigInt& div) {
        long long down = 0;
        long long up = Base;
        TBigInt tmpRes;
        long long res = 0;
        long long mid = 0;
        while (down <= up) {
            mid = (up + down) / 2;
            tmpRes = WeakMultiply(div, mid);
            if (num < tmpRes) {
                up = mid - 1;
            } else {
                res = mid;
                down = mid + 1;
            }
        }
        return res;
    }

    TBigInt operator/(const TBigInt& lhs, const TBigInt& rhs) {
        if (rhs.Data.size() == 1 && rhs.Data[0] == 0) {
            throw std::logic_error("Division by zero.");
        }

        if (rhs.Data.size() == 1) {
            return TBigInt::WeakDivision(lhs, rhs.Data[0]);
        }

        TBigInt res;
        TBigInt tmpQuotient;
        for (long long i = lhs.Data.size() - 1; i >= 0; --i) {
            tmpQuotient.Data.insert(tmpQuotient.Data.begin(), lhs.Data[i]);
            long long tmpDivider = TBigInt::FindBin(tmpQuotient, rhs);
            res.Data.insert(res.Data.begin(), tmpDivider);
            tmpQuotient = tmpQuotient - TBigInt::WeakMultiply(rhs, tmpDivider);
        }

        res.RemoveNulls();

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

    void TBigInt::LeftShift(long long degree) { 
        if (Data.size() == 1 && Data[0] == 0) {
            return;
        }

        long long oldEnd = Data.size() - 1;
        Data.resize(Data.size() + degree);
        for (long long i = oldEnd; i >= 0; --i) {
            Data[i + degree] = Data[i];
        }
        for (long long i = 0; i < degree; ++i) {
            Data[i] = 0;
        }
    }

    void TBigInt::Split(const TBigInt& num, TBigInt& lhs, TBigInt& rhs) {
        long long half = num.Data.size() / 2;
        lhs.Data.resize(half);
        rhs.Data.resize(half);
        for (long long i = 0; i < lhs.Data.size(); ++i) {
            lhs.Data[i] = num.Data[i + half];
            rhs.Data[i] = num.Data[i];
        }
    }

    TBigInt TBigInt::KaratsubaMult(TBigInt&& lhs, TBigInt&& rhs) {
        // дополняем числа до одного размера, и чтобы размер делился на 2
        long long len = std::max(lhs.Data.size(), rhs.Data.size());
        if (len == 1) {
            TBigInt res = lhs;
            res.Data[0] *= rhs.Data[0];
            if (res.Data[0] >= Base) {
                long long carry = res.Data[0] / Base;
                res.Data[0] %= Base;
                res.Data.push_back(carry);
            }
            return res;
        }
        len = (len % 2 != 0) ? len + 1 : len;

        for (long long i = lhs.Data.size(); i < len; ++i) {
            lhs.Data.push_back(0);
        }
        for (long long i = rhs.Data.size(); i < len; ++i) {
            rhs.Data.push_back(0);
        }

        TBigInt firstTermLHS;
        TBigInt secondTermLHS;
        Split(lhs, firstTermLHS, secondTermLHS);

        TBigInt firstTermRHS;
        TBigInt secondTermRHS;
        Split(rhs, firstTermRHS, secondTermRHS);

        TBigInt resFirstTerm = KaratsubaMult(std::move(firstTermLHS), std::move(firstTermRHS));

        TBigInt resThirdTerm = KaratsubaMult(std::move(secondTermLHS), std::move(secondTermRHS));

        TBigInt resSecondTerm = (KaratsubaMult(firstTermLHS + secondTermLHS, firstTermRHS + secondTermRHS) - resFirstTerm) - resThirdTerm;

        resFirstTerm.LeftShift(len);
        resSecondTerm.LeftShift(len / 2);

        TBigInt res = resFirstTerm + resSecondTerm + resThirdTerm;

        long long carry = 0;
        for (long long i = 0; i < res.Data.size(); ++i) {
            res.Data[i] += carry;
            carry = res.Data[i] / TBigInt::Base;
            res.Data[i] %= TBigInt::Base;
        }

        if (carry != 0) {
            res.Data.push_back(carry);
        }

        return res;
    }

    void TBigInt::RemoveNulls() {
        while (Data.size() > 1 && Data.back() == 0) {
            Data.pop_back();
        }
    }
};
