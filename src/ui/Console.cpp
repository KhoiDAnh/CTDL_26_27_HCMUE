// Console.cpp
// Module M2: Hien thuc cac tien ich giao dien console
#include "ui/Console.h"

#include <conio.h>
#include <io.h>
#include <iostream>
#include <string>

#include "core/StringUtils.h"

namespace {
    const int KEY_ENTER     = 13;
    const int KEY_BACKSPACE = 8;
    const int KEY_ESC       = 27;

    // Kiem tra stdin co dang duoc chay truc tiep tu terminal hay khong
    bool isInteractive() {
        return _isatty(_fileno(stdin)) != 0;
    }
}

namespace Console {

    void printColor(const char* color, const std::string& text) {
        std::cout << color << text << RESET;
    }

    void printLine(int width, char ch) {
        for (int i = 0; i < width; ++i) {
            std::cout << ch;
        }
        std::cout << '\n';
    }

    void printHeader(const std::string& title, int width) {
        std::cout << '\n';
        printColor(CYAN, std::string(width, '*') + "\n");
        int padding = (width - static_cast<int>(title.size())) / 2;
        if (padding < 0) padding = 0;
        printColor(CYAN, std::string(padding, ' ') + title + "\n");
        printColor(CYAN, std::string(width, '*') + "\n");
        std::cout << '\n';
    }

    void printError(const std::string& message) {
        std::cout << RED << "[LOI] " << message << RESET << '\n';
    }

    void printSuccess(const std::string& message) {
        std::cout << GREEN << "[OK] " << message << RESET << '\n';
    }

    std::string readLine(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << YELLOW << prompt << RESET;
            if (!std::getline(std::cin, input)) {
                return "";
            }
            if (!StringUtils::isBlank(input)) {
                return input;
            }
            printError("Khong duoc de trong hoac chi chua khoang trang. Vui long nhap lai.");
        }
    }

    int readInt(const std::string& prompt) {
        while (true) {
            const std::string text = readLine(prompt);
            if (text.empty()) {
                return 0;
            }
            try {
                return StringUtils::parseInt(text);
            } catch (const std::invalid_argument&) {
                printError("Vui long nhap mot so nguyen hop le.");
            }
        }
    }

    std::string readPassword(const std::string& prompt) {
        std::cout << YELLOW << prompt << RESET;

        // Ho tro ca moi truong script/pipe
        if (!isInteractive()) {
            std::string pass;
            if (std::getline(std::cin, pass)) {
                std::cout << "********\n";
                return pass;
            }
            return "";
        }

        std::string password;
        while (true) {
            const int ch = _getch();

            // Bat phim ESC de huy / thoat khoi he thong theo de bai
            if (ch == KEY_ESC) {
                std::cout << '\n';
                return "";
            }

            // Phim Enter ket thuc nhap mat khau
            if (ch == KEY_ENTER) {
                std::cout << '\n';
                return password;
            }

            // Phim Backspace xoa ky tu
            if (ch == KEY_BACKSPACE) {
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
                continue;
            }

            // Bo qua extended keys tren ban phim
            if (ch == 0 || ch == 224) {
                _getch();
                continue;
            }

            // Nhap ky tu thuong -> them vao pass va hien thi dau *
            password += static_cast<char>(ch);
            std::cout << '*';
        }
    }

    void pause() {
        std::cout << '\n' << YELLOW << "Nhan Enter (hoac phim bat ky) de tiep tuc..." << RESET;
        if (isInteractive()) {
            _getch();
            std::cout << '\n';
        } else {
            std::string dummy;
            std::getline(std::cin, dummy);
            std::cout << '\n';
        }
    }

    void clearScreen() {
        if (isInteractive()) {
            std::cout << "\033[2J\033[H";
        }
    }

}  // namespace Console
