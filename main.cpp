#include <iostream>
#include <string>
#include <vector>

#include "big_int.hpp"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0); 

    std::string firstOperand;
    std::string secondOperand;
    char op;
    while (std::cin >> firstOperand >> secondOperand >> op) {
        NBigInt::TBigInt firstNum(firstOperand);
        NBigInt::TBigInt secondNum(secondOperand);

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
                std::cout << NBigInt::TBigInt::FastPow(firstNum, secondNum) << '\n';
            } catch (std::logic_error) {
                std::cout << "Error\n";
            }
        } else if (op == '/') {
            try {
                std::cout << firstNum / secondNum << '\n';
            } catch (std::logic_error) {
                std::cout << "Error\n";
            }
        } else if (op == '>') {
            (firstNum > secondNum) ? std::cout << "true\n" : std::cout << "false\n";
        } else if (op == '<') {
            (firstNum < secondNum) ? std::cout << "true\n" : std::cout << "false\n";;
        } else if (op == '=') {
            (firstNum == secondNum) ? std::cout << "true\n" : std::cout << "false\n";;
        } else {
            std::cout << "Invalid operator!\n";
        }
    }
    return 0;
}

/*
10376077992263680000
2468425984
/
*/
