#pragma once
#include <string>
#include <map>

void logError(const std::string& message);

class BaseFile {
protected:
    std::string filename;

public:
    BaseFile(const std::string& file) : filename(file) {}

    friend void logError(const std::string& message);

    std::map<std::string, std::string> readFileToMap() const;
    void writeMapToFile(const std::map<std::string, std::string>& data) const;
    void addRecord(const std::string& key, const std::string& value);
    void deleteRecord(const std::string& key);
};
