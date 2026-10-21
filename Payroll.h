#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

class HandlerBase {
public:
    virtual ~HandlerBase() = default;
    virtual void process(const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) = 0;
};

template <typename T>
class Handler : public HandlerBase {
private:
    T& instance;

public:
    explicit Handler(T& obj) : instance(obj) {}

    void process(const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File) override {
        instance.process(fullName, baseSalary, rules2File, rulesFile, rules3File);
    }
};

class PayLists {
private:
    std::map<std::string, double> additions;
    std::map<std::string, double> deductions;
    double netSalary = 0.0;

public:
    PayLists();
    void process(const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File);
    void loadAdditions(const std::string& rulesFile);
    void loadDeductions(const std::string& rulesFile);
    std::map<std::string, bool> loadEmployeeAdditions(const std::string& rulesFile, const std::string& fullName);
    double calculateTotalSalary(double baseSalary, const std::map<std::string, bool>& employeeAdditions);
    double calculateDeductions(double totalSalary);
    void createPaySlip(const std::string& fullName, double& baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File);
    double getNetSalary(const std::string& fullName, double& baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File);
};

class Reports {
public:
    void reportForEmployee(PayLists& payLists, const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File);
    void process(const std::string& fullName, double baseSalary, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File);
};

class Generator {
private:
    std::unique_ptr<HandlerBase> handler;

public:
    explicit Generator(std::unique_ptr<HandlerBase> handler);
    void generateForRole(const std::string& roleFile, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File);
    void generateForAllRoles(const std::vector<std::string>& roleFiles, const std::string& rules2File, const std::string& rulesFile, const std::string& rules3File);
};
