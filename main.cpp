#include <iostream>
#include <stdexcept>
#include <string>

#include "model/KhachHang.h"

void check(const std::string& line) {
    try {
        KhachHang::fromLine(line);
        std::cout << "OK: " << line << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "THROWS: " << line << " -> " << e.what() << '\n';
    }
}

int main() {
    check("1|079203001234|Nguyen Van A");
    check("1|079203001234|Nguyen|Van A");
    check("1|07920300123|Nguyen Van A");
    check("1|07920300123A|Nguyen Van A");
    check("0|079203001234|Nguyen Van A");
    check("x|079203001234|Nguyen Van A");
    check("1|079203001234|   ");
    check("1|079203001234");
    return 0;
}