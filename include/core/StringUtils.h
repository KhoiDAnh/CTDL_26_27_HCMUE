#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>

namespace StringUtils {

    inline bool isBlank(const std::string& text) {
        return text.find_first_not_of(" \t\r\n") == std::string::npos;
    }

    inline bool isAllDigits(const std::string& text) {
        return text.find_first_not_of("0123456789") == std::string::npos;
    }

    inline bool splitExact(const std::string& text, char separator,
                           std::string* fields, std::size_t expectedCount) {
        std::size_t start = 0;
        for (std::size_t i = 0; i + 1 < expectedCount; ++i) {
            const std::size_t end = text.find(separator, start);
            if (end == std::string::npos) {
                return false;
            }
            fields[i] = text.substr(start, end - start);
            start = end + 1;
        }
        if (text.find(separator, start) != std::string::npos) {
            return false;
        }
        fields[expectedCount - 1] = text.substr(start);
        return true;
    }

    inline int parseInt(const std::string& text) {
        std::size_t parsedLength = 0;
        int value = 0;
        try {
            value = std::stoi(text, &parsedLength);
        } catch (const std::logic_error&) {
            throw std::invalid_argument("not a valid integer: '" + text + "'");
        }
        if (parsedLength != text.size()) {
            throw std::invalid_argument("not a valid integer: '" + text + "'");
        }
        return value;
    }

}  // namespace StringUtils