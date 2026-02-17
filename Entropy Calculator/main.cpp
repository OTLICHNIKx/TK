#include <iostream>
#include <clocale>
#include "EntropyCalculator.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    EntropyCalculator calculator;
    calculator.run();

    return 0;
}