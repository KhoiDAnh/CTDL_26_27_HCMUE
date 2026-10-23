// Ve.cpp
#include "model/Ve.h"

#include <stdexcept>
#include <string>

#include "core/StringUtils.h"
#include "model/Date.h"
#include "model/KhachHang.h"

namespace {
    // banned words
    const char* const kBannedInFlightCode = "\\/:*\"<>| \t\r\n";
}

void Ve::validateFlightCode(const std::string& flightCode) {
    if (StringUtils::isBlank(flightCode)) {
        throw std::invalid_argument("Ve: Ma chuyen bay khong duoc trong.");
    }
    if (flightCode.find_first_of(kBannedInFlightCode) != std::string::npos) {
        throw std::invalid_argument("Ve: Ma chuyen bay chua ky tu khong hop le.");
    }
}

Ve::Ve(const std::string& flightCode, int seatNumber,
        const std::string& idNumber, const std::string& fullName)
    : flightCode_(flightCode), seatNumber_(seatNumber), idNumber_(idNumber), fullName_(fullName)
{
    // 4 things to worry about: flightCode, seatNumber, idNumber, fullName
    validateFlightCode(flightCode);
    if (seatNumber < 1) {
        throw std::invalid_argument("Ve: So ghe phai it nhat la 1.");
    }
    KhachHang::validateIDNumber(idNumber);
    KhachHang::validateFullName(fullName);
}

const std::string& Ve::getFlightCode() const {
    return flightCode_;
}

int Ve::getSeatNumber() const {
    return seatNumber_;
}

const std::string& Ve::getIdNumber() const{
    return idNumber_;
}

const std::string& Ve::getFullName() const {
    return fullName_;
}

std::string Ve::getCode() const {
    return flightCode_ + "-" + std::to_string(seatNumber_);
}

std::string Ve::toLine() const {
    return flightCode_ + "|" + std::to_string(seatNumber_)
            + "|" + idNumber_ + "|" + fullName_;
}

Ve Ve::fromLine(const std::string& line) {
    // Ve(str flightCode, int seatNumber, str idNumber, str fullName)
    std::string fields[4];
    if (!StringUtils::splitExact(line, '|', fields, 4)) {
        throw std::invalid_argument("Ve: Du lieu nhap vao phai dung 4 vung.");
    }
    return Ve{fields[0], StringUtils::parseInt(fields[1]), fields[2], fields[3]};
}