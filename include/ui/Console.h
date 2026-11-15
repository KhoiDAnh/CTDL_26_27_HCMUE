#pragma once

#include <string>

// The only class that touches the operating system's console.
class Console {
public:
    enum class Color { White, Yellow, Green, Red, Cyan, Gray };
    enum Key { kEscape = 27, kEnter = 13, kBackspace = 8 };

    static void clear();
    static void setColor(Color color);
    static void resetColor();

    static void print(const std::string& text, Color color = Color::White);
    static void println(const std::string& text = "", Color color = Color::White);

    // Reads one key without echo. Returns 0 for keys we ignore (arrows, F-keys).
    static int readKey();

    // Prompt in green, input in white. Return false when the user pressed ESC.
    static bool readLine(const std::string& prompt, std::string& out);
    static bool readHidden(const std::string& prompt, std::string& out);  // shows '*'

    static void waitForKey(const std::string& message = " Nhan phim bat ky de tiep tuc...");
};