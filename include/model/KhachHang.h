// KhachHang.h
// constraints: order_ >= 1, idNumber_.size() = 12,
// fullName_ not blank && no |

#pragma once

#include <string>

class KhachHang {
private:
    int order_;
    std::string idNumber_;
    std::string fullName_;
public:
    KhachHang(int order, const std::string& idNumber, const std::string& fullName);

    int getOrder() const;
    const std::string& getIdNumber() const;
    const std::string& getFullName() const;

    // no setters, getters marked const
    std::string toLine() const;
    static KhachHang fromLine(const std::string& line);

    static void validateIDNumber(const std::string& idNumber);
    static void validateFullName(const std::string& fullName);
};