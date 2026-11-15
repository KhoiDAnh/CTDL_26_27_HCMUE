#pragma once

#include <cstddef>
#include <string>

#include "model/ChuyenBay.h"

namespace Format {

    inline std::string statusText(ChuyenBay::Status status) {
        switch (status) {
            case ChuyenBay::Status::Cancelled:
                return "Huy chuyen";
            case ChuyenBay::Status::Available:
                return "Con ve";
            case ChuyenBay::Status::SoldOut:
                return "Het ve";
            case ChuyenBay::Status::Completed:
                return "Hoan tat";
        }
        return "Khong ro";
    }

    // Pads or cuts text to exactly `width` characters, always leaving at least one
    // trailing space. Cut text ends in "...". Assumes width >= 2.
    inline std::string fit(const std::string& text, std::size_t width) {
        std::string cell = text;
        if (cell.size() > width - 1) {
            cell = (width > 4) ? cell.substr(0, width - 4) + "..." : cell.substr(0, width - 1);
        }
        return cell + std::string(width - cell.size(), ' ');
    }

    inline std::string trim(const std::string& text) {
        const std::size_t first = text.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            return "";
        }
        const std::size_t last = text.find_last_not_of(" \t\r\n");
        return text.substr(first, last - first + 1);
    }

}  // namespace Format