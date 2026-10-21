#pragma once
#include <string>
#include <utility>

std::string encryptPassword(const std::string& password);
std::string decryptPassword(const std::string& encryptedPassword);

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << p.first << " " << p.second;
    return os;
}

class Registration {
private:
    std::string userFile;
    std::string credentialFile;
    static std::string currentLogin;

public:
    Registration(const std::string& userFile, const std::string& credentialFile);

    void registerUser();
    bool loginUser();

    static std::string getCurrentLogin();
    std::pair<std::string, std::string> getCurrentUser(const std::string& login);
};
