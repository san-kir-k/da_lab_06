#include <iostream>
#include <string>
#include <vector>

#include "big_int.hpp"

int main() {
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
    std::string firstOperand;
    std::string secondOperand;
    char op;
    while (std::cin >> firstOperand >> secondOperand >> op) {
        BigInt::BigInt firstNum(firstOperand);
        BigInt::BigInt secondNum(secondOperand);

        if (op == '+') {
            // ...
        } else if (op == '-') {
            // ...
        } else if (op == '*') {
            // ...
        } else if (op == '^') {
            // ...
        } else if (op == '/') {
            // ...
        } else if (op == '>') {
            // ...
        } else if (op == '<') {
            // ...
        } else if (op == '=') {
            // ...
        } else {
            std::cout << "Invalid operator!\n";
        }
    }
    return 0;
}