#include <iostream>
#include <string>
#include <vector>

#include "big_int.hpp"

int main() {
    std::string firstOperand;
    std::string secondOperand;
    char op;
    while (std::cin >> firstOperand >> secondOperand >> op) {
        BigInt::BigInt firstNum(firstOperand);
        BigInt::BigInt secondNum(secondOperand);

        if (op == '+') {
            std::cout << firstNum + secondNum << '\n'; 
        } else if (op == '-') {
            try {
                std::cout << firstNum - secondNum << '\n';
            } catch (std::logic_error) {
                std::cout << "Error\n";
            }
        } else if (op == '*') {
            std::cout << firstNum * secondNum << "\n";
        } else if (op == '^') {
            try {
                std::cout << BigInt::BigInt::FastPow(firstNum, secondNum) << '\n';
            } catch (std::logic_error) {
                std::cout << "Error\n";
            }
        } else if (op == '/') {
            try {
                std::cout << BigInt::BigInt::FastPow(firstNum, secondNum) << '\n';
            } catch (std::logic_error) {
                std::cout << "Error\n";
            }
        } else if (op == '>') {
            (firstNum > secondNum) ? std::cout << "true\n" : std::cout << "false\n";
        } else if (op == '<') {
            (firstNum < secondNum) ? std::cout << "true\n" : std::cout << "false\n";;
        } else if (op == '=') {
            (firstNum == secondNum) ? std::cout << "true\n" : std::cout << "false\n";;
        } else if (op == '?') {
            std::cout << firstNum << "\n" << secondNum << "\n";
        } else {
            std::cout << "Invalid operator!\n";
        }
    }
    return 0;
}