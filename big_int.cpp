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

        if (res.Data[res.Data.size() - 1] == 0 && res.Data.size() > 1) {
            res.Data.pop_back();
        }
        return res;
    }

    BigInt operator*(const BigInt& lhs, const BigInt& rhs) {

    }

    BigInt operator/(const BigInt& lhs, const BigInt& rhs) {

    }

    BigInt BigInt::FastPow(const BigInt& lhs, const BigInt& rhs) {

    }
};