#include "Menus.h"
#include "UIUtils.h"
#include "Registration.h"
#include "EmployeeManager.h" 
#include "Payroll.h"
#include "UserRoles.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif

void entranceMenu() {
    const std::string userFile = "users.txt";
    const std::string credentialFile = "credentials.txt";
    Registration reg(userFile, credentialFile);

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Регистрация\n";
        std::cout << "2. Вход\n";
        std::cout << "3. Выход\n";

        int choice = getValidInput<int>("Выберите опцию: ");
        switch (choice) {
        case 1:
            reg.registerUser();
            break;
        case 2:
            if (reg.loginUser()) {
                std::string login = Registration::getCurrentLogin();
                auto [fullName, role] = reg.getCurrentUser(login);

                if (role == "Бухгалтер") {
                    std::cout << "Добро пожаловать, Бухгалтер!\n";
                    mainMenu();
                }
                else {
                    std::cout << "Добро пожаловать, " << fullName << "!\n";
                    employeeMenu(fullName, role);
                }
            }
            break;
        case 3:
            std::cout << "Выход из системы.\n";
            return;
        default:
            std::cout << "Ошибка. Неверный выбор.\n";
        }
    }
}

void mainMenu() {
    const std::string ratesFile = "rules.txt";

    while (true) {
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cout << "   Главное меню системы" << std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cout << "1. Добавить сотрудника" << std::endl;
        std::cout << "2. Удалить сотрудника" << std::endl;
        std::cout << "3. Редактировать запись сотрудника" << std::endl;
        std::cout << "4. Просмотр сотрудников" << std::endl;
        std::cout << "5. Составление отчётов" << std::endl;
        std::cout << "6. Сортировка данных" << std::endl;
        std::cout << "7. Фильтрация данных" << std::endl;
        std::cout << "8. Редактирование переменных формулы" << std::endl;
        std::cout << "9. Составление расчётных листков" << std::endl;
        std::cout << "10. Выход" << std::endl;

        int choice = getValidInput<int>("Выберите опцию: ");

        switch (choice) {
        case 1:
            addEmployee();
            break;
        case 2:
            deleteEmployee();
            break;
        case 3:
            editEmployeeRecord();
            break;
        case 4:
            menuViewrs();
            break;
        case 5:
            menuForPayListsOrReports(choice);
            break;
        case 6:
            std::cout << "Отсортировано."<< std::endl;
            break;
        case 7:
        {
            std::cout << "Выберите предмет для фильтрации" << std::endl;
            std::cout << "1. По должности" << std::endl;
            std::cout << "2. По окладу" << std::endl;
            int choice2 = getValidInput<int>("Выберите опцию: ");
            if (choice2 == 1) {
                filterByRole();
            }
            else if (choice2 == 2) {
                filterBySalary();
            }
            else {
                std::cout << "Ошибка: неверный выбор. Попробуйте снова.\n";
            }
            break;
        }
        case 8:
            editRateFile(ratesFile);
            break;
        case 9:
            menuForPayListsOrReports(choice);
            break;
        case 10:
            return;
        default:
            std::cout << "Ошибка: неверный выбор. Попробуйте снова.\n";
        }
    }
}

void employeeMenu(const std::string& fullName, const std::string& role) {
    std::string filename = getFilenameByRole(UserRoles::roleFromString(role));
    if (filename.empty()) {
        std::cout << "Ошибка: неверная роль.\n";
        return;
    }

    std::vector<std::string> lines = getEmployeesList(filename);
    double salary = 0.0;
    bool found = false;

    for (const auto& line : lines) {
        std::stringstream ss(line);
        std::string name, roleInFile, salaryStr;

        std::getline(ss, name, '|');
        std::getline(ss, roleInFile, '|');
        std::getline(ss, salaryStr);

        if (name == fullName) {
            try {
                salary = std::stod(salaryStr);
                found = true;
                break;
            }
            catch (const std::invalid_argument&) {
                std::cout << "Ошибка: некорректное значение оклада для сотрудника " << fullName << std::endl;
                return;
            }
        }
    }

    if (!found) {
        std::cout << "Сотрудник с таким ФИО не найден в файле.\n";
        return;
    }

    std::cout << "Оклад: " << salary << std::endl;

    PayLists payLists;
    Reports reports;

    while (true) {
        std::cout << "\nМеню сотрудника: \n";
        std::cout << "1. Показать расчетный лист\n";
        std::cout << "2. Показать отчёт\n";
        std::cout << "3. Выйти\n";

        int choice = getValidInput<int>("Выберите опцию: ");
        switch (choice) {
        case 1:
            payLists.createPaySlip(fullName, salary, "rules2.txt", "rules.txt", "rules3.txt");
            break;
        case 2:
            reports.reportForEmployee(payLists, fullName, salary, "rules2.txt", "rules.txt", "rules3.txt");
            break;
        case 3:
            return;
        default:
            std::cout << "Ошибка. Неверный выбор.\n";
        }
    }
}

void menuViewrs() {
    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Найти сотрудника\n";
        std::cout << "2. Показать список сотрудников по должности\n";
        std::cout << "3. Показать всех сотрудников\n";
        std::cout << "4. Выйти\n";

        int choice = getValidInput<int>("Выберите опцию: ");

        switch (choice) {
        case 1: {
            std::string fullName = getValidatedString("Введите ФИО сотрудника для поиска: ");
            findEmployeeByName(fullName);
            break;
        }
        case 2:
            filterByRole();
            break;
        case 3: {
            std::vector<std::string> allEmployees = getAllEmployees();
            if (allEmployees.empty()) {
                std::cout << "Нет сотрудников в базе данных.\n";
            }
            else {
                for (const auto& employee : allEmployees) {
                    std::cout << employee << std::endl;
                }
            }
            break;
        }
        case 4:
            return;
        default:
            std::cout << "Ошибка: неверный выбор. Попробуйте снова.\n";
        }
    }
}

void menuForPayListsOrReports(int& baseChoice) {
    while (true) {
        std::cout << "Составить:\n";
        std::cout << "1. Для одного сотрудника\n";
        std::cout << "2. Для сотрудников одной должности\n";
        std::cout << "3. Для всех сотрудников\n";
        std::cout << "4. Выход\n";
        int choice = getValidInput<int>("Выберите опцию: ");
        std::string rules2File = "rules2.txt";
        std::string rulesFile = "rules.txt";
        std::string rules3File = "rules3.txt";
        PayLists payLists;
        Reports reports;
        switch (choice) {
        case 1: {
            std::string fullName = getValidatedString("Введите ФИО: ");
            UserRoles::Role role = UserRoles::selectRole();
            std::string filename = getFilenameByRole(role);

            if (filename.empty()) {
                std::cout << "Ошибка: неверная роль.\n";
                return;
            }

            std::vector<std::string> lines = getEmployeesList(filename);
            double salary = 0.0;
            bool found = false;

            for (const auto& line : lines) {
                std::stringstream ss(line);
                std::string name, roleInFile, salaryStr;

                std::getline(ss, name, '|');
                std::getline(ss, roleInFile, '|');
                std::getline(ss, salaryStr);

                name.erase(0, name.find_first_not_of(" \t"));
                name.erase(name.find_last_not_of(" \t") + 1);

                if (name == fullName) {
                    try {
                        salary = std::stod(salaryStr);
                        found = true;
                        break;
                    }
                    catch (const std::invalid_argument&) {
                        std::cout << "Ошибка: некорректное значение оклада для сотрудника " << fullName << std::endl;
                        return;
                    }
                }
            }

            if (!found) {
                std::cout << "Сотрудник с таким ФИО не найден в файле.\n";
                return;
            }

            std::cout << "Оклад: " << salary << std::endl;

            if (baseChoice == 5) {
                reports.reportForEmployee(payLists, fullName, salary, rules2File, rulesFile, rules3File);
            }
            else {
                payLists.createPaySlip(fullName, salary, rules2File, rulesFile, rules3File);
            }
            break;
        }
        case 2: {
            std::cout << "Выберите должность: \n";
            for (size_t i = 0; i < UserRoles::roleFiles.size(); ++i) {
                std::cout << i + 1 << ". " << UserRoles::roleToString(static_cast<UserRoles::Role>(i)) << std::endl;
            }
            int roleChoice = getValidInput<int>("Введите номер должности: ");
            if (roleChoice < 1 || roleChoice > UserRoles::roleFiles.size()) {
                std::cout << "Ошибка: неверный выбор должности.\n";
                break;
            }

            std::string roleFile = UserRoles::roleFiles[roleChoice - 1];

            if (baseChoice == 9) {

                std::unique_ptr<HandlerBase> payListsHandler = std::make_unique<Handler<PayLists>>(payLists);
                Generator payListsGenerator(move(payListsHandler));
                payListsGenerator.generateForRole(roleFile, rules2File, rulesFile, rules3File);
            }
            else {
                std::unique_ptr<HandlerBase> reportsHandler = std::make_unique<Handler<Reports>>(reports);
                Generator reportsGenerator(move(reportsHandler));
                reportsGenerator.generateForRole(roleFile, rules2File, rulesFile, rules3File);
            }
            break;
        }
        case 3: {

            if (baseChoice == 9) {
                std::unique_ptr<HandlerBase> payListsHandler = std::make_unique<Handler<PayLists>>(payLists);
                Generator payListsGenerator(std::move(payListsHandler));
                payListsGenerator.generateForAllRoles(UserRoles::roleFiles, rules2File, rulesFile, rules3File);
            }
            else {
                std::unique_ptr<HandlerBase> reportsHandler = std::make_unique<Handler<Reports>>(reports);
                Generator reportsGenerator(move(reportsHandler));
                reportsGenerator.generateForAllRoles(UserRoles::roleFiles, rules2File, rulesFile, rules3File);

            }
            break;
        }

        case 4:
            return;
        default:
            std::cout << "Ошибка: неверный выбор. Попробуйте снова.\n";
        }
    }
}