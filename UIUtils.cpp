#include "UIUtils.h"
#include <iostream>


std::string getValidatedString(const std::string& prompt) {
    std::string name;
    while (true) {
        std::cout << prompt;
        getline(std::cin, name);
        if (!name.empty() && name.length() <= 100) {
            break;
        }
        std::cout << "Ошибка: строка должна содержать от 1 до 100 символов. Пожалуйста, попробуйте снова.\n";
    }
    return name;
}

std::string MenuForFile() {
    std::string filename = getValidatedString("Введите название файла (без .txt): ");
    filename += ".txt";
    return filename;
}
