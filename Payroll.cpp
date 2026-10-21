#include "Payroll.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <clocale> 
#include <iomanip>

PayLists::PayLists() {}

void PayLists::process(const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) {
    std::cout << "Расчетный лист для сотрудника: " << fullName << " с окладом " << baseSalary << std::endl;
    createPaySlip(fullName, baseSalary, rules2File, rulesFile, rules3File);
}

void PayLists::loadAdditions(const std::string& rulesFile) {
    std::ifstream file(rulesFile);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << rulesFile << std::endl;
        return;
    }
    std::string line;
    size_t i = 0;
    std::vector<std::string> additionNames = { "Повышение за стаж", "Повышение по контракту", "Премия месячная", "Повышение за молодого специалиста" };

    char* old_locale = std::setlocale(LC_NUMERIC, nullptr);
    std::string saved_locale = old_locale ? old_locale : "C";
    std::setlocale(LC_NUMERIC, "C");

    while (std::getline(file, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ')) {
            line.pop_back();
        }
        if (line.empty()) continue;

        try {
            double value = std::stod(line);
            additions[additionNames[i]] = value;
            i++;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Ошибка в rules.txt" << std::endl;
        }
    }
    file.close();
    std::setlocale(LC_NUMERIC, saved_locale.c_str());
}

void PayLists::loadDeductions(const std::string& rulesFile) {
    std::ifstream file(rulesFile);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << rulesFile << std::endl;
        return;
    }
    std::string line;
    std::vector<std::string> deductionNames = { "Аванс", "Пенсионный фонд", "Налог" };
    size_t i = 0;

    char* old_locale = std::setlocale(LC_NUMERIC, nullptr);
    std::string saved_locale = old_locale ? old_locale : "C";
    std::setlocale(LC_NUMERIC, "C");

    while (std::getline(file, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ')) {
            line.pop_back();
        }
        if (line.empty()) continue;

        try {
            double value = std::stod(line);
            deductions[deductionNames[i]] = value;
            i++;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Ошибка в файле" << std::endl;
        }
    }
    file.close();
    std::setlocale(LC_NUMERIC, saved_locale.c_str());
}



std::map<std::string, bool> PayLists::loadEmployeeAdditions(const std::string& rulesFile, const std::string& fullName) {
    std::ifstream file(rulesFile);
    std::map<std::string, bool> employeeAdditions;
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << rulesFile << std::endl;
        return employeeAdditions;
    }
    std::vector<std::string> additionKeys = { "Повышение за стаж", "Повышение по контракту", "Премия месячная", "Повышение за молодого специалиста" };
    std::string line;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(iss, token, '|')) {
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            tokens.push_back(token);
        }
        if (tokens.size() != additionKeys.size() + 1) {
            continue;
        }
        if (tokens[0] == fullName) {
            for (size_t i = 1; i < tokens.size(); ++i) {
                employeeAdditions[additionKeys[i - 1]] = (tokens[i] == "Y");
            }
            break;
        }
    }
    file.close();
    return employeeAdditions;
}

double PayLists::calculateTotalSalary(double baseSalary, const std::map<std::string, bool>& employeeAdditions) {
    double totalSalary = baseSalary;
    for (const auto& [addition, isApplied] : employeeAdditions) {
        if (isApplied && additions.find(addition) != additions.end()) {
            totalSalary += baseSalary * additions[addition];
        }
    }
    return totalSalary;
}

double PayLists::calculateDeductions(double totalSalary) {
    double totalDeductions = totalSalary;
    for (const auto& [deduction, percentage] : deductions) {
        totalDeductions += totalSalary * percentage;
    }
    totalDeductions -= totalSalary * deductions["Аванс"];
    return totalDeductions;
}

void PayLists::createPaySlip(const std::string& fullName, double& baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) {
    std::cout << "Расчетный лист для: " << fullName << " с окладом " << baseSalary << std::endl;
    std::map<std::string, bool> employeeAdditions = loadEmployeeAdditions(rules2File, fullName);
    if (employeeAdditions.empty()) {
        std::cout << "Ошибка: нет данных о надбавках для сотрудника " << fullName << std::endl;
        return;
    }
    loadAdditions(rulesFile);
    loadDeductions(rules3File);
    if (additions.empty() || deductions.empty()) {
        std::cout << "Ошибка: не удалось загрузить надбавки или вычеты." << std::endl;
        return;
    }
    double totalSalary = calculateTotalSalary(baseSalary, employeeAdditions);
    std::cout << "Надбавки:" << std::endl;
    for (const auto& [addition, isApplied] : employeeAdditions) {
        if (isApplied) {
            std::cout << addition << ": " << baseSalary * additions[addition] << std::endl;
        }
    }
    double totalWedge = totalSalary * deductions["Аванс"];
    double totalDeductions = calculateDeductions(totalWedge);
    double nSalary = totalSalary - totalDeductions;

    std::cout << "Итоговая зарплата (с учетом надбавок): " << totalSalary << std::endl;
    std::cout << "Аванс: " << totalWedge << std::endl;
    std::cout << "Пенсионный фонд: " << totalWedge * deductions["Пенсионный фонд"] << std::endl;
    std::cout << "Налог: " << totalWedge * deductions["Налог"] << std::endl;
    std::cout << "Общие вычеты: " << totalDeductions << std::endl;
    std::cout << "К выплате: " << nSalary << std::endl;
}

double PayLists::getNetSalary(const std::string& fullName, double& baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) {
    std::map<std::string, bool> employeeAdditions = loadEmployeeAdditions(rules2File, fullName);
    if (employeeAdditions.empty()) {
        return 0.0;
    }
    loadAdditions(rulesFile);
    loadDeductions(rules3File);
    if (additions.empty() || deductions.empty()) {
        return 0.0;
    }
    double totalSalary = calculateTotalSalary(baseSalary, employeeAdditions);
    double totalWedge = totalSalary * deductions["Аванс"];
    double totalDeductions = calculateDeductions(totalWedge);
    netSalary = totalSalary - totalDeductions;
    return netSalary;
}

void Reports::reportForEmployee(PayLists& payLists, const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) {
    double nSalary = payLists.getNetSalary(fullName, baseSalary, rules2File, rulesFile, rules3File);
    if (nSalary > 0.0) {
        std::cout << "\nОтчет для сотрудника: " << fullName << std::endl;
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "Оклад: " << baseSalary << std::endl;
        std::cout << "К выплате: " << nSalary << std::endl;
    }
    else {
        std::cout << "Не удалось рассчитать итоговую зарплату для сотрудника " << fullName << std::endl;
    }
}

void Reports::process(const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) {
    PayLists payLists;
    reportForEmployee(payLists, fullName, baseSalary, rules2File, rulesFile, rules3File);
}

Generator::Generator(std::unique_ptr<HandlerBase> h) : handler(std::move(h)) {}

void Generator::generateForRole(const std::string& roleFile, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) {
    std::ifstream file(roleFile);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << roleFile << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string fullName, role, salaryStr;
        std::getline(ss, fullName, '|');
        std::getline(ss, role, '|');
        std::getline(ss, salaryStr);
        try {
            double baseSalary = std::stod(salaryStr);
            handler->process(fullName, baseSalary, rules2File, rulesFile, rules3File);
        }
        catch (const std::invalid_argument&) {
            std::cout << "Ошибка: некорректное значение оклада для сотрудника " << fullName << std::endl;
        }
    }
    file.close();
}

void Generator::generateForAllRoles(const std::vector<std::string>& roleFiles, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) {
    for (const auto& roleFile : roleFiles) {
        std::cout << "\nОбрабатываем файл роли: " << roleFile << std::endl;
        generateForRole(roleFile, rules2File, rulesFile, rules3File);
    }
}
