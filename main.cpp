#include <iostream>
#include <stdexcept>
#include <string>

#include "model/KhachHang.h"
#include "model/Ve.h"
#include "model/Date.h"

void checkKhachHang(const std::string& line) {
    try {
        KhachHang::fromLine(line);
        std::cout << "OK: " << line << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "THROWS: " << line << " -> " << e.what() << '\n';
    }
}

void checkVe(const std::string& line) {
    try {
        const Ve ticket = Ve::fromLine(line);
        std::cout << "OK : " << line << "   code=" << ticket.getCode()
                  << "   roundtrip=" << (ticket.toLine() == line ? "yes" : "NO") << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "THROWS: " << line << " -> " << e.what() << '\n';
    }
}

void checkDate(const std::string& line) {
    try {
        Date::fromString(line);
        std::cout << "OK: " << line << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "THROWS: " << line << " -> " << e.what() << '\n';
    }
}

int main() {

    return 0;
}