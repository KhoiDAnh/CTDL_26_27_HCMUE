//
// Created by KHOI DO on 14/10/2026.
//

#include <model/KhachHang.h>

#include <cstddef>
#include <stdexcept>
#include <string>

#include <core/StringUtils.h>

namespace {
    const std::size_t idLength = 12;

    bool isAllDigits(const std::string& text) {
        return text.find_first_not_of("0123456789") == std::string::npos;
    }
}

// yeu cau oop
int KhachHang::getOrder() const {
    return order_;
}

const std::string& KhachHang::getIdNumber() const {
    return idNumber_;
}

const std::string& KhachHang::getFullName() const {
    return fullName_;
}

KhachHang::KhachHang(int order, const std::string& idNumber,
                    const std::string& fullName)
    : order_(order), idNumber_(idNumber), fullName_(fullName)
{
    // order check
    if (order < 1) {
        throw std::invalid_argument("KhachHang: Don hang phai it nhat la 1.");
    }
    // id size check
    if (idNumber.size() != idLength || !isAllDigits(idNumber)) {
        throw std::invalid_argument("KhachHang: CMND phai chinh xac 12 chu so.");
    }
    // check isBlank() for name
    if (StringUtils::isBlank(fullName)) {
        throw std::invalid_argument("KhachHang: Ten day du khong duoc trong.");
    }
    // check if '|' appears
    if (fullName.find('|') != std::string::npos) {
        throw std::invalid_argument("KhachHang: Ten day du khong duoc chua ky tu '|'.");
    }
}

// chuan hoa Line -> 3 field values
std::string KhachHang::toLine() const {
    return std::to_string(order_) + "|" + idNumber_ + "|" + fullName_;
}

// extracting values from toLine() string
KhachHang KhachHang::fromLine(const std::string& line) {
    std::string fields[3];
    // splitExact(string text, char sep, string* fields, size_t cnt)
    if (!StringUtils::splitExact(line, '|', fields, 3)) {
        throw std::invalid_argument("KhachHang: Du lieu nhap vao phai co dung 3 vung.");
    }
    // parseInt(const std::string& text)
    return KhachHang{StringUtils::parseInt(fields[0]), fields[1], fields[2]};
}