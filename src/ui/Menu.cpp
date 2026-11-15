#include "ui/Menu.h"

#include <cstddef>
#include <stdexcept>
#include <string>

#include "core/StringUtils.h"
#include "ui/Console.h"

namespace {

    const std::size_t kMenuWidth = 40;

}  // namespace

Menu::Menu(const std::string& title) : title_(title) {}

void Menu::addItem(int number, const std::string& label) {
    Item item;
    item.number = number;
    item.label = label;
    items_.pushBack(item);
}

bool Menu::hasItem(int number) const {
    for (std::size_t i = 0; i < items_.size(); ++i) {
        if (items_.at(i).number == number) {
            return true;
        }
    }
    return false;
}

int Menu::show() const {
    const std::string heavyRule(kMenuWidth, '=');
    const std::size_t padding =
        title_.size() < kMenuWidth ? (kMenuWidth - title_.size()) / 2 : 0;

    while (true) {
        Console::clear();
        Console::println(heavyRule, Console::Color::Cyan);
        Console::println(std::string(padding, ' ') + title_, Console::Color::Cyan);
        Console::println(heavyRule, Console::Color::Cyan);
        for (std::size_t i = 0; i < items_.size(); ++i) {
            Console::print(" " + std::to_string(items_.at(i).number) + ". ",
                           Console::Color::Yellow);
            Console::println(items_.at(i).label);
        }
        Console::println(std::string(kMenuWidth, '-'), Console::Color::Gray);

        std::string input;
        if (!Console::readLine(" Nhap lua chon: ", input)) {
            return 0;  // ESC
        }
        try {
            const int choice = StringUtils::parseInt(input);
            if (hasItem(choice)) {
                return choice;
            }
        } catch (const std::invalid_argument&) {
            // falls through to the message below
        }
        Console::println(" Lua chon khong hop le. Vui long thu lai.", Console::Color::Red);
        Console::waitForKey();
    }
}