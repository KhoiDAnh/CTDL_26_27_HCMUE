// Console.h
// Module M2: Tien ich giao dien console (mau sac, nhap xuat an toan, doc mat khau an *)
#ifndef CTDL_26_27_HCMUE_CONSOLE_H
#define CTDL_26_27_HCMUE_CONSOLE_H

#include <string>

namespace Console {

    // --- Ma mau ANSI ho tro giao dien console de nhin ---
    const char* const RESET   = "\033[0m";
    const char* const RED     = "\033[1;31m";
    const char* const GREEN   = "\033[1;32m";
    const char* const YELLOW  = "\033[1;33m";
    const char* const BLUE    = "\033[1;34m";
    const char* const CYAN    = "\033[1;36m";
    const char* const WHITE   = "\033[1;37m";

    // --- Cac ham in an dinh dang ---
    // In chuoi voi ma mau chi dinh
    void printColor(const char* color, const std::string& text);

    // In duong ke ngang phan cach
    void printLine(int width = 70, char ch = '-');

    // In khung tieu de noi bat voi mau CYAN
    void printHeader(const std::string& title, int width = 60);

    // In thong bao loi voi mau do
    void printError(const std::string& message);

    // In thong bao thanh cong voi mau xanh la
    void printSuccess(const std::string& message);

    // --- Cac ham nhap lieu an toan ---
    // Doc chuoi tu ban phim, khong duoc phep de trong hoac chi chua khoang trang
    std::string readLine(const std::string& prompt);

    // Doc so nguyen tu ban phim, kiem tra validation so hop le
    int readInt(const std::string& prompt);

    // Doc mat khau an thanh ky tu '*', ho tro Backspace va bat phim ESC de thoat/huy
    // Tra ve chuoi rong "" neu nguoi dung nhan phím ESC
    std::string readPassword(const std::string& prompt);

    // Dung man hinh cho nguoi dung nhan phim tiep tuc
    void pause();

    // Xoa man hinh console
    void clearScreen();

}  // namespace Console

#endif // CTDL_26_27_HCMUE_CONSOLE_H
