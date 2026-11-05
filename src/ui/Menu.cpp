// Menu.cpp
// Module M2: Hien thuc cac menu lua chon cua he thong
#include "ui/Menu.h"
#include "ui/Console.h"

#include <iostream>

namespace Menu {

    int showMain() {
        Console::printHeader("QUAN LY CHUYEN BAY NOI DIA");

        std::cout << Console::CYAN << "  1. " << Console::WHITE << "Hien thi danh sach cac chuyen bay\n"
                  << Console::CYAN << "  2. " << Console::WHITE << "Dat ve\n"
                  << Console::CYAN << "  3. " << Console::WHITE << "Quan ly\n"
                  << Console::CYAN << "  0. " << Console::RED   << "Thoat\n" << Console::RESET;

        Console::printLine(60);
        return Console::readInt("Lua chon: ");
    }

    int showAdmin() {
        Console::printHeader("MENU QUAN LY");

        std::cout << Console::CYAN << "  1. " << Console::WHITE << "Xu ly dat ve\n"
                  << Console::CYAN << "  2. " << Console::WHITE << "Xu ly tra ve\n"
                  << Console::CYAN << "  3. " << Console::WHITE << "Thong ke\n"
                  << Console::CYAN << "  0. " << Console::YELLOW << "Dang xuat\n" << Console::RESET;

        Console::printLine(60);
        return Console::readInt("Lua chon: ");
    }

    int showStatistics() {
        Console::printHeader("MENU THONG KE");

        std::cout << Console::CYAN << "  1. " << Console::WHITE << "Hien thi danh sach khach cua mot chuyen bay\n"
                  << Console::CYAN << "  2. " << Console::WHITE << "Hien thi danh sach ghe con trong cua mot chuyen bay\n"
                  << Console::CYAN << "  3. " << Console::WHITE << "Thong ke so luong thuc hien chuyen bay cua mot may bay\n"
                  << Console::CYAN << "  0. " << Console::YELLOW << "Quay lai\n" << Console::RESET;

        Console::printLine(60);
        return Console::readInt("Lua chon: ");
    }

}  // namespace Menu
