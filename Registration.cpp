#include "Registration.h"
#include "UIUtils.h"
#include "UserRoles.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string Registration::currentLogin;

std::string encryptPassword(const std::string& password) {
    std::string encrypted = password;
    for (char& c : encrypted) {
        c = c + 3;
    }
    return encrypted;
}

std::string decryptPassword(const std::string& encryptedPassword) {
    std::string decrypted = encryptedPassword;
    for (char& c : decrypted) {
        c = c - 3;
    }
    return decrypted;
}

Registration::Registration(const std::string& userFile, const std::string& credentialFile)
    : userFile(userFile), credentialFile(credentialFile) {}

void Registration::registerUser() {
    std::string login = getValidatedString("Введите логин: ");
    std::string password = getValidatedString("Введите пароль: ");
    std::string fullName = getValidatedString("Введите ФИО: ");
    UserRoles::Role role = UserRoles::selectRole();
    std::string roleString = UserRoles::roleToString(role);

    std::ofstream userStream(userFile, std::ios::app);
    if (!userStream.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << userFile << std::endl;
        return;
    }
    userStream << login << "|" << fullName << "|" << roleString << "|" << password << std::endl;
    userStream.close();

    std::ofstream credStream(credentialFile, std::ios::app);
    if (!credStream.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << credentialFile << std::endl;
        return;
    }
    credStream << login << "|" << encryptPassword(password) << std::endl;
    credStream.close();

    std::cout << "Регистрация успешна. Ваша должность: " << roleString << ".\n";
}

bool Registration::loginUser() {
    std::string login = getValidatedString("Введите логин: ");
    std::string password = getValidatedString("Введите пароль: ");

    std::ifstream credStream(credentialFile);
    if (!credStream.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << credentialFile << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(credStream, line)) {
        std::stringstream ss(line);
        std::string storedLogin, encryptedPass;
        std::getline(ss, storedLogin, '|');
        std::getline(ss, encryptedPass);

        if (storedLogin == login && decryptPassword(encryptedPass) == password) {
            currentLogin = login;
            std::cout << "Вы успешно вошли в систему.\n";
            credStream.close();
            return true;
        }
    }

    credStream.close();
    std::cout << "Неверный логин или пароль.\n";
    return false;
}

std::string Registration::getCurrentLogin() {
    return currentLogin;
}

std::pair<std::string, std::string> Registration::getCurrentUser(const std::string& login) {
    std::ifstream file(userFile);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << userFile << std::endl;
        return {};
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string storedLogin, fullName, role, password;
        std::getline(ss, storedLogin, '|');
        std::getline(ss, fullName, '|');
        std::getline(ss, role, '|');
        std::getline(ss, password);

        if (storedLogin == login) {
            file.close();
            return { fullName, role };
        }
    }

    file.close();
    return {};
}
