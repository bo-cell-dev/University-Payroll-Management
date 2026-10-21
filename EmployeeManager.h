#pragma once
#include <string>
#include <vector>
#include <map>
#include "BaseFile.h"
#include "UserRoles.h"

class Employee : public BaseFile {
public:
    Employee(const std::string& file) : BaseFile(file) {}

    std::vector<std::string> getEmployeesByRole(const std::string& role) const;
    std::vector<std::string> getAllEmployees() const;
    void rewriteFile(const std::map<std::string, std::string>& data) const;
    void addEmployee(const std::string& fullName, const std::string& roleString, double salary);
    void deleteEmployee(const std::string& fullName);
    std::vector<std::string> getEmployeesBySalaryRange(double minSalary, double maxSalary) const;
};

std::vector<std::string> getEmployeesByRole(const std::string& roleFilename);
std::vector<std::string> getAllEmployees();
std::vector<std::string> getEmployeesList(const std::string& filename);
std::string getFilenameByRole(UserRoles::Role role);
void filterByRole();
void rewriteFile(const std::string& filename, const std::vector<std::string>& lines);
void addEmployee();
void findEmployeeInAllFiles(const std::vector<std::string>& filenames, const std::string& fullName);
void deleteEmployee();
void editEmployeeRecord();
void findEmployeeByName(const std::string& fullName);
void filterBySalary();
void editRateFile(const std::string& ratesFile);
