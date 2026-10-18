//
// Created by KHOI DO on 18/10/2026.
//

#include "model/Date.h"

#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>

#include "core/StringUtils.h"

namespace {
    const int kMinYear = 1900;
    const int kMaxYear = 2100;

    bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    int daysInMonth(int month, int year) {
        if (month == 2) {
            return isLeapYear(year) ? 29 : 28;
        }
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            return 30;
        }
        return 31;
    }
}

Date::Date(int day, int month, int year)
    : day_(day), month_(month), year_(year) {
    if (month < 1 || month > 12) {
        throw std::invalid_argument("Date: Thang khong hop le.");
    }
    if (year < kMinYear || year > kMaxYear) {
        throw std::invalid_argument("Date: Nam khong hop le.");
    }
    if (day < 1 || day > daysInMonth(month, year)) {
        throw std::invalid_argument("Date: Ngay khong hop le.");
    }
}

int Date::getDay() const {
    return day_;
}

int Date::getMonth() const {
    return month_;
}

int Date::getYear() const {
    return year_;
}

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << day_ << '/'
        << std::setw(2) << month_ << '/'
        << year_;
    return oss.str();
}

Date Date::fromString(const std::string &text) {
    // force strict formatting dd/mm/yyyy (no symbols, letters, spaces)
    const bool format =
        text.size() == 10 && text[2] == '/' && text[5] == '/' &&
        StringUtils::isAllDigits(text.substr(0, 2)) &&
        StringUtils::isAllDigits(text.substr(3, 2)) &&
        StringUtils::isAllDigits(text.substr(6, 4));
    if (!format) {
        throw std::invalid_argument("Date: Ngay phai co dinh dang dd/mm/yyyy.");
    }
    return Date{
        StringUtils::parseInt(text.substr(0, 2)),
        StringUtils::parseInt(text.substr(3, 2)),
        StringUtils::parseInt(text.substr(6, 4)),
    };
}