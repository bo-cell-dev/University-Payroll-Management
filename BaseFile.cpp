#include "BaseFile.h"
#include <iostream>
#include <fstream>
#include <sstream>

void logError(const std::string& message) {
    std::cout << "Ошибка: " << message << std::endl;
}

std::map<std::string, std::string> BaseFile::readFileToMap() const {
    std::map<std::string, std::string> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        logError("Не удалось открыть файл " + filename);
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, value;
        std::getline(ss, key, '|');
        std::getline(ss, value);
        data[key] = value;
    }
    file.close();
    return data;
}

void BaseFile::writeMapToFile(const std::map<std::string, std::string>& data) const {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        return;
    }

    file << "\xEF\xBB\xBF";

    for (const auto& [key, value] : data) {
        file << key << "|" << value << std::endl;
    }
    file.close();
}

void BaseFile::addRecord(const std::string& key, const std::string& value) {
    std::map<std::string, std::string> data = readFileToMap();
    if (data.find(key) != data.end()) {
        std::cout << "Ошибка: В файле уже существует запись с ключом: " << key << std::endl;
        return;
    }
    data[key] = value;

    std::ifstream checkFile(filename);
    bool isNewFile = (checkFile.peek() == std::ifstream::traits_type::eof());
    checkFile.close();

    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл для добавления записи" << std::endl;
        return;
    }

    if (isNewFile) {
        file << "\xEF\xBB\xBF";
    }

    file << key << "|" << value << std::endl;
    file.close();
    std::cout << "Ключ: " << key << std::endl;
}

void BaseFile::deleteRecord(const std::string& key) {
    std::fstream file(filename, std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл для добавления записи" << std::endl;
        return;
    }

    std::string line;
    std::streampos prevPos;
    bool found = false;
    while (getline(file, line)) {
        prevPos = file.tellg();
        std::stringstream ss(line);
        std::string currentKey;
        std::getline(ss, currentKey, '|');

        if (currentKey == key) {
            found = true;
            file.seekp(prevPos - static_cast<std::streampos>(line.length()) - 1);
            file << std::string(line.length(), ' ') << std::endl;
            break;
        }
    }

    if (!found) {
        std::cout << "Запись с ключом " << key << " не найдена." << std::endl;
    }
    else {
        std::cout << "Запись удалена: " << key << std::endl;
    }

    file.close();
}

