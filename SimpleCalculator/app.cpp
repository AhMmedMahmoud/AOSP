#include <iostream>
#include "libmath/math.h"

int main() {
    int num1, num2;

    std::cout << "Enter first number: ";
    std::cin >> num1;

    std::cout << "Enter second number: ";
    std::cin >> num2;

    int result = add(num1, num2);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
