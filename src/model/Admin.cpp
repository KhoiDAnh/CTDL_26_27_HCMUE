// Admin.cpp
#include "model/Admin.h"

#include <stdexcept>
#include <string>

#include <core/StringUtils.h>

Admin::Admin(const std::string& username, const std::string& password)
    : username_(username), password_(password){
    // username checks
    if (StringUtils::isBlank(username)) {
        throw std::invalid_argument("Admin: Ho ten khong duoc trong.");
    }
    if (username.find('|') != std::string::npos) {
        throw std::invalid_argument("Admin: Ho ten khong duoc chua dau '|'.");
    }

    // password checks
    if (StringUtils::isBlank(password)) {
        throw std::invalid_argument("Admin: Mat khau khong duoc de trong.");
    }
    if (password.find('|') != std::string::npos) {
        throw std::invalid_argument("Admin: Mat khau khong duoc chua dau '|'.");
    }
}

const std::string& Admin::getUsername() const {
    return username_;
}

bool Admin::matches(const std::string& username, const std::string& password) const {
    return (username_ == username) && (password_ == password);
}

std::string Admin::toLine() const {
    return username_ + "|" + password_;
}

Admin Admin::fromLine(const std::string& line) {
    std::string fields[2];
    // splitExact(str line, char sep, str array, int cnt)
    if (!StringUtils::splitExact(line, '|', fields, 2)) {
        throw std::invalid_argument("Admin: Chuoi phai co it nhat 2 vung du lieu.");
    }
    return Admin{fields[0], fields[1]};
}