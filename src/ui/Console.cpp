#include "ui/Console.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdio>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <conio.h>
#include <windows.h>
#else
#include <sys/select.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace {

const std::size_t kMaxInputLength = 100;

bool readInput(const std::string& prompt, std::string& out, bool hidden) {
    Console::print(prompt, Console::Color::Green);
    std::string text;
    while (true) {
        const int key = Console::readKey();
        if (key == Console::kEscape) {
            std::cout << '\n';
            return false;
        }
        if (key == Console::kEnter) {
            std::cout << '\n';
            out = text;
            return true;
        }
        if (key == Console::kBackspace) {
            if (!text.empty()) {
                text.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else if (key >= 32 && key < 127 && text.size() < kMaxInputLength) {
            text += static_cast<char>(key);
            std::cout << (hidden ? '*' : static_cast<char>(key)) << std::flush;
        }
    }
}

}  // namespace

#ifdef _WIN32

void Console::clear() {
    std::system("cls");
}

void Console::setColor(Color color) {
    WORD attribute =
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    switch (color) {
        case Color::Yellow:
            attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case Color::Green:
            attribute = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case Color::Red:
            attribute = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case Color::Cyan:
            attribute = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case Color::Gray:
            attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case Color::White:
            break;
    }
    std::cout.flush();  // text already written must keep its old color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
}

int Console::readKey() {
    const int key = _getch();
    if (key == EOF) {
        return kEscape;  // no console attached: end the loop instead of spinning
    }
    if (key == 0 || key == 224) {  // arrow / function key: a second code follows
        _getch();
        return 0;
    }
    return key;
}

#else

namespace {

bool inputPending() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 20000;
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &timeout) > 0;
}

}  // namespace

void Console::clear() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void Console::setColor(Color color) {
    const char* code = "\033[97m";
    switch (color) {
        case Color::Yellow:
            code = "\033[93m";
            break;
        case Color::Green:
            code = "\033[92m";
            break;
        case Color::Red:
            code = "\033[91m";
            break;
        case Color::Cyan:
            code = "\033[96m";
            break;
        case Color::Gray:
            code = "\033[90m";
            break;
        case Color::White:
            break;
    }
    std::cout << code;
}

int Console::readKey() {
    termios original;
    tcgetattr(STDIN_FILENO, &original);
    termios raw = original;
    raw.c_lflag &= ~static_cast<tcflag_t>(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int key = kEscape;  // end of input counts as ESC so loops always finish
    unsigned char byte = 0;
    if (read(STDIN_FILENO, &byte, 1) == 1) {
        key = byte;
    }
    if (key == 27) {
        bool swallowed = false;
        while (inputPending()) {
            unsigned char ignored = 0;
            if (read(STDIN_FILENO, &ignored, 1) != 1) {
                break;
            }
            swallowed = true;
        }
        if (swallowed) {
            key = 0;  // an arrow key, not a real ESC
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &original);

    if (key == '\n') {
        key = kEnter;
    }
    if (key == 127) {
        key = kBackspace;
    }
    return key;
}

#endif

void Console::resetColor() {
    setColor(Color::White);
}

void Console::print(const std::string& text, Color color) {
    setColor(color);
    std::cout << text << std::flush;
    resetColor();
}

void Console::println(const std::string& text, Color color) {
    print(text + "\n", color);
}

bool Console::readLine(const std::string& prompt, std::string& out) {
    return readInput(prompt, out, false);
}

bool Console::readHidden(const std::string& prompt, std::string& out) {
    return readInput(prompt, out, true);
}

void Console::waitForKey(const std::string& message) {
    print(message, Color::Gray);
    readKey();
    std::cout << '\n';
}