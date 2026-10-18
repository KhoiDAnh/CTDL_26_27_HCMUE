#pragma once

#include <string>

class Admin {
private:
    std::string username_;
    std::string password_;
public:
    // constructor
    Admin(const std::string& username, const std::string& password);

    // username getter, no password getter cuz no one needs it
    const std::string& getUsername() const;

    bool matches(const std::string& username, const std::string& password) const;

    // chuan hoa
    std::string toLine() const;
    static Admin fromLine(const std::string& line);
};