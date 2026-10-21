#include "EmployeeManager.h"
#include "UIUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>


std::vector<std::string> Employee::getEmployeesByRole(const std::string& role) const {
    std::vector<std::string> result;
    auto data = readFileToMap();
    for (const auto& [key, value] : data) {
        if (value == role) {
            result.push_back(key);
        }
    }
    return result;
}

std::vector<std::string> Employee::getAllEmployees() const {
    std::vector<std::string> result;
    auto data = readFileToMap();
    for (const auto& [key, value] : data) {
        result.push_back(key);
    }
    return result;
}

void Employee::rewriteFile(const std::map<std::string, std::string>& data) const {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл для записи " << filename << std::endl;
        return;
    }
    for (const auto& [fullName, roleAndSalary] : data) {
        file << fullName << "|" << roleAndSalary << std::endl;
    }
    file.close();
    std::cout << "Файл успешно обновлен." << std::endl;
}
void Employee::addEmployee(const std::string& fullName, const std::string& roleString, double salary) {
    std::map<std::string, std::string> data = readFileToMap();
    if (data.find(fullName) != data.end()) {
        std::cout << "Сотрудник с таким именем уже существует." << std::endl;
        return;
    }
    std::stringstream record;
    record << roleString << "|" << salary;
    data[fullName] = record.str();
    rewriteFile(data);
    std::cout << "Сотрудник добавлен: " << fullName << " | " << roleString << " | " << salary << std::endl;
}

void Employee::deleteEmployee(const std::string& fullName) {
    std::map<std::string, std::string> data = readFileToMap();
    if (data.empty()) {
        std::cout << "В файле нет записей." << std::endl;
        return;
    }
    if (data.find(fullName) == data.end()) {
        std::cout << "Сотрудник с таким именем не найден." << std::endl;
        return;
    }
    data.erase(fullName);
    rewriteFile(data);
    std::cout << "Сотрудник с именем " << fullName << " удален." << std::endl;
}

std::vector<std::string> Employee::getEmployeesBySalaryRange(double minSalary, double maxSalary) const {
    std::vector<std::string> result;
    auto data = readFileToMap();
    for (const auto& [fullName, roleAndSalary] : data) {
        std::stringstream ss(roleAndSalary);
        std::string role;
        double salary;
        std::getline(ss, role, '|');
        ss >> salary;
        if (salary >= minSalary && salary <= maxSalary) {
            result.push_back(fullName + " | " + role + " | " + std::to_string(salary));
        }
    }
    return result;
}

std::vector<std::string> getEmployeesByRole(const std::string& roleFilename) {
    std::vector<std::string> employees;
    std::ifstream file(roleFilename);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << roleFilename << std::endl;
        return employees;
    }
    std::string line;
    while (std::getline(file, line)) {
        employees.push_back(line);
    }
    file.close();
    return employees;
}
std::vector<std::string> getAllEmployees() {
    std::vector<std::string> allEmployees;
    for (const auto& filename : UserRoles::roleFiles) {
        std::vector<std::string> employees = getEmployeesByRole(filename);
        allEmployees.insert(allEmployees.end(), employees.begin(), employees.end());
    }
    return allEmployees;
}

std::vector<std::string> getEmployeesList(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return lines;
    }
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

std::string getFilenameByRole(UserRoles::Role role) {
    switch (role) {
    case UserRoles::RECTOR: return "rector.txt";
    case UserRoles::VICE_RECTOR: return "vice_rector.txt";
    case UserRoles::DIRECTOR: return "director.txt";
    case UserRoles::DEAN: return "dean.txt";
    case UserRoles::DEPARTMENT_HEAD: return "department_head.txt";
    case UserRoles::PROFESSOR: return "professor.txt";
    case UserRoles::ASSOCIATE_PROFESSOR: return "associate_professor.txt";
    case UserRoles::SENIOR_LECTURER: return "senior_lecturer.txt";
    case UserRoles::ASSISTANT: return "assistant.txt";
    case UserRoles::ACCOUNTANT: return "accountant.txt";
    default:
        std::cout << "Ошибка: неизвестная роль." << std::endl;
        return "";
    }
}

void filterByRole() {
    UserRoles::Role role = UserRoles::selectRole();
    std::string filename = getFilenameByRole(role);
    if (filename.empty()) return;
    std::vector<std::string> employees = getEmployeesByRole(filename);
    for (const auto& employee : employees) {
        std::cout << employee << std::endl;
    }
}

void rewriteFile(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream outFile(filename, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл для записи " << filename << std::endl;
        return;
    }
    for (const auto& line : lines) {
        outFile << line << std::endl;
    }
    outFile.close();
    std::cout << "Файл успешно обновлен.\n";
}

void addEmployee() {
    std::string fullName = getValidatedString("Введите ФИО сотрудника: ");
    UserRoles::Role role = UserRoles::selectRole();
    std::string roleString = UserRoles::roleToString(role);
    double salary = getValidInput<double>("Введите базовый оклад: ");

    Employee employeeManager(getFilenameByRole(role));
    employeeManager.addEmployee(fullName, roleString, salary);

    std::string rules2File = "rules2.txt";
    std::cout << "Выберите надбавки для сотрудника " << fullName << ":\n";
    std::vector<std::string> additions = {
        "Повышение за стаж",
        "Повышение по контракту",
        "Премия месячная",
        "Повышение за молодого специалиста"
    };
    std::vector<std::string> selectedAdditions(4, "N");
    std::cout << "Выберите надбавки (Y - да, N - нет):\n";
    for (size_t i = 0; i < additions.size(); ++i) {
        char choice = getValidatedString(additions[i] + " (Y/N):")[0];
        selectedAdditions[i] = (choice == 'Y' || choice == 'y') ? "Y" : "N";
    }
    std::string ruleEntry = fullName + " | " + selectedAdditions[0] + " | " + selectedAdditions[1] + " | " +
        selectedAdditions[2] + " | " + selectedAdditions[3];

    std::ofstream file(rules2File, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Ошибка: невозможно открыть файл " << rules2File << std::endl;
        return;
    }
    file << ruleEntry << std::endl;
    file.close();
    std::cout << "Сотрудник успешно добавлен, информация о надбавках сохранена в " << rules2File << std::endl;
}
void findEmployeeInAllFiles(const std::vector<std::string>& filenames, const std::string& fullName) {
    bool found = false;
    for (const auto& file : filenames) {
        Employee employee(file);
        auto data = employee.readFileToMap();
        auto it = data.find(fullName);
        if (it != data.end()) {
            std::cout << "Сотрудник найден: " << fullName << ", информация: " << it->second
                << " (файл: " << file << ")" << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Сотрудник с именем " << fullName << " не найден во всех файлах." << std::endl;
    }
}

void deleteEmployee() {
    std::string fullNameToDelete = getValidatedString("Введите ФИО сотрудника для удаления: ");
    UserRoles::Role roleToDelete = UserRoles::selectRole();
    Employee employeeManager(getFilenameByRole(roleToDelete));
    employeeManager.deleteEmployee(fullNameToDelete);

    std::string rules2File = "rules2.txt";
    std::ifstream file(rules2File);
    if (!file.is_open()) {
        std::cout << "Ошибка: невозможно открыть файл " << rules2File << std::endl;
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    std::ofstream outFile(rules2File, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Ошибка: невозможно открыть файл для записи " << rules2File << std::endl;
        return;
    }
    bool found = false;
    for (const auto& l : lines) {
        if (l.find(fullNameToDelete) != std::string::npos) {
            found = true;
            continue;
        }
        outFile << l << std::endl;
    }
    outFile.close();

    if (found) {
        std::cout << "Сотрудник " << fullNameToDelete << " был успешно удален, включая данные о надбавках.\n";
    }
    else {
        std::cout << "Сотрудник с таким ФИО не найден в " << rules2File << ".\n";
    }
}

void editEmployeeRecord() {
    UserRoles::Role roleToEdit = UserRoles::selectRole();
    std::string filename = getFilenameByRole(roleToEdit);
    if (filename.empty()) return;

    std::vector<std::string> lines = getEmployeesList(filename);
    if (lines.empty()) return;
    int i = 1;
    for (auto& line : lines) {
        std::cout << i << ". " << line << std::endl;
        i++;
    }
    int lineToEdit = getValidInput<int>("Введите номер записи для редактирования: ");
    if (lineToEdit < 1 || lineToEdit > static_cast<int>(lines.size())) {
        std::cout << "Ошибка: неверный номер записи.\n";
        return;
    }

    std::stringstream ss(lines[lineToEdit - 1]);
    std::string fullName, role, salary;
    std::getline(ss, fullName, '|');
    std::getline(ss, role, '|');
    std::getline(ss, salary);

    std::cout << "Текущие данные сотрудника: \n";
    std::cout << "1. ФИО: " << fullName << std::endl;
    std::cout << "2. Зарплата: " << salary << std::endl;

    int partToEdit = getValidInput<int>("Введите номер поля для редактирования (1 - ФИО, 2 - Зарплата, 3 - Надбавки): ");

    std::string newValue;
    switch (partToEdit) {
    case 1:
        newValue = getValidatedString("Введите новое ФИО: ");
        fullName = newValue;
        break;
    case 2:
        newValue = std::to_string(getValidInput<double>("Введите новую зарплату: "));
        salary = newValue;
        break;
    case 3:
    {
        std::cout << "Надбавки для сотрудника:\n";
        std::string rules2File = "rules2.txt";
        std::ifstream rulesFile(rules2File);
        if (!rulesFile.is_open()) {
            std::cout << "Ошибка: невозможно открыть файл с надбавками " << rules2File << std::endl;
            return;
        }

        std::vector<std::string> rulesLines;
        std::string ruleLine;
        while (std::getline(rulesFile, ruleLine)) {
            if (ruleLine.find(fullName) != std::string::npos) {
                rulesLines.push_back(ruleLine);
            }
        }
        rulesFile.close();

        if (rulesLines.empty()) {
            std::cout << "Ошибка: сотрудник не найден в списке надбавок.\n";
            return;
        }

        std::cout << "Текущие надбавки для " << fullName << ":\n";
        std::cout << rulesLines[0] << std::endl;

        char choice;
        std::cout << "Хотите изменить надбавки? (y/n): ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::string newAdditions = "";
            std::cout << "Введите новую информацию для надбавок (Y - добавить, N - не добавлять), например:\n";
            std::cout << "1. Повышение за стаж, 2. Повышение по контракту, 3. Премия месячная, 4. Повышение за молодого специалиста\n";

            for (int k = 0; k < 4; ++k) {
                char addChoice;
                std::cout << "Добавить для позиции " << k + 1 << "? (y/n): ";
                std::cin >> addChoice;
                newAdditions += (addChoice == 'y' || addChoice == 'Y') ? "Y" : "N";
                newAdditions += " | ";
            }
            rulesLines[0] = fullName + " | " + newAdditions.substr(0, newAdditions.size() - 2);
            std::ofstream outFile(rules2File, std::ios::trunc);
            if (!outFile.is_open()) {
                std::cout << "Ошибка: невозможно открыть файл для записи " << rules2File << std::endl;
                return;
            }

            for (const std::string& l : rulesLines) {
                outFile << l << std::endl;
            }
            outFile.close();
            std::cout << "Надбавки для сотрудника успешно обновлены.\n";
        }
        break;
    }
    default:
        std::cout << "Ошибка: неверный номер поля.\n";
        return;
    }

    std::stringstream updatedRecord;
    updatedRecord << fullName << "|" << role << "|" << salary;

    lines[lineToEdit - 1] = updatedRecord.str();

    rewriteFile(filename, lines);
    std::cout << "Запись успешно обновлена.\n";
}
void findEmployeeByName(const std::string& fullName) {
    bool found = false;

    for (const auto& roleFile : UserRoles::roleFiles) {
        Employee employee(roleFile);

        auto data = employee.readFileToMap();
        auto it = data.find(fullName);
        if (it != data.end()) {
            std::cout << "\nСотрудник найден в файле " << roleFile << ":" << std::endl;
            std::cout << "ФИО: " << fullName << ", информация: " << it->second << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Сотрудник с именем " << fullName << " не найден в базе данных." << std::endl;
    }
}

void filterBySalary() {
    double minSalary = getValidInput<double>("Введите минимальный оклад: ");
    double maxSalary = getValidInput<double>("Введите максимальный оклад: ");

    for (const auto& roleFile : UserRoles::roleFiles) {
        Employee employee(roleFile);
        std::vector<std::string> employees = employee.getEmployeesBySalaryRange(minSalary, maxSalary);

        if (!employees.empty()) {
            std::cout << "\nСотрудники из файла " << roleFile << " с окладом в диапазоне [" << minSalary << ", " << maxSalary << "]:" << std::endl;
            for (const auto& emp : employees) {
                std::cout << emp << std::endl;
            }
        }
        else {
            std::cout << "\nВ файле " << roleFile << " нет сотрудников с указанным диапазоном окладов." << std::endl;
        }
    }
}
void editRateFile(const std::string& ratesFile) {
    std::ifstream file(ratesFile);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << ratesFile << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();

    std::cout << "Содержимое файла:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << (i + 1) << ". " << lines[i] << std::endl;
    }

    int lineNumber = getValidInput<int>("Введите номер строки для редактирования: ");

    if (lineNumber < 1 || lineNumber > static_cast<int>(lines.size())) {
        std::cout << "Неверный номер строки. Попробуйте снова.\n";
        return;
    }

    std::cout << "Текущее значение: " << lines[lineNumber - 1] << "\n";

    std::string newRateStr = getValidatedString("Введите новое значение: ");

    lines[lineNumber - 1] = newRateStr;

    std::ofstream outFile(ratesFile, std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл для записи " << ratesFile << std::endl;
        return;
    }

    outFile << "\xEF\xBB\xBF";

    for (const std::string& l : lines) {
        outFile << l << std::endl;
    }
    outFile.close();

    std::cout << "Редактирование файла завершено.\n";
}
