#pragma once
#include <string>
#include <iostream>
#include <type_traits>

std::string getValidatedString(const std::string& prompt);
std::string MenuForFile();

template <typename T>
T getValidInput(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        if (input.length() > 10) {
            std::cout << "Ошибка: значение не должно превышать 10 символов. Пожалуйста, попробуйте снова.\n";
            continue;
        }
        try {
            size_t pos;
            if constexpr (std::is_same_v<T, int>) {
                value = std::stoi(input, &pos);
            }
            else if constexpr (std::is_same_v<T, double>) {
                value = std::stod(input, &pos);
            }
            else {
                throw std::invalid_argument("неподдерживаемый тип");
            }

            if (value <= 0) {
                std::cout << "Ошибка: значение должно быть положительным и больше нуля. Пожалуйста, попробуйте снова.\n";
                continue;
            }
            if (pos == input.length()) {
                return value;
            }
            else {
                throw std::invalid_argument("Остались нечисловые символы.");
            }
        }
        catch (const std::invalid_argument&) {
            std::cout << "Неверный ввод. Пожалуйста, введите корректное значение.\n" << std::endl;
        }
        catch (const std::out_of_range&) {
            std::cout << "Введенное число слишком велико.\n" << std::endl;
        }
    }
}


