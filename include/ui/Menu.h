#pragma once

#include <string>

#include "core/LinkedList.h"

class Menu {
private:
    struct Item {
        int number;
        std::string label;
    };

    bool hasItem(int number) const;

    std::string title_;
    LinkedList<Item> items_;
public:
    explicit Menu(const std::string& title);

    void addItem(int number, const std::string& label);

    // Draws the menu and returns the chosen number. ESC returns 0, so every
    // menu should have an item 0 (exit / back).
    int show() const;
};