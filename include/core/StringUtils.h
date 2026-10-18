//
// Created by KHOI DO on 14/10/2026.
//

#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>

namespace StringUtils {
    inline bool isBlank(const std::string& text) {
        return text.find_first_not_of(" \t\r\n") == std::string::npos;
    }

    // cuts text at seperator into EXACTLY expectedCnt fields
    inline bool splitExact(const std::string& text, char seperator,
                            std::string* fields, std::size_t expectedCnt) {
        std::size_t start = 0;
        for (std::size_t i = 0; i + 1 < expectedCnt; ++i) {

            const std::size_t end = text.find(seperator, start);
            // if not found
            if (end == std::string::npos) {
                return false;
            }
            // else split into an array
            fields[i] = text.substr(start, end - start);
            start = end + 1;
        }
        if (text.find(seperator, start) != std::string::npos) {
            return false;
        }
        fields[expectedCnt - 1] = text.substr(start);
        return true;
    }

    inline int parseInt(const std::string& text) {
        std::size_t parsedLength = 0;
        int value = 0;

        try {
            value = std::stoi(text, &parsedLength);
        }
        catch (const std::logic_error&) {
            throw std::invalid_argument("Khong phai la so nguyen hop le: '" + text + "'");
        }
        if (parsedLength != text.size()) {
            throw std::invalid_argument("Khong phai la so nguyen hop le: '" + text + "'");
        }
        return value;
    }
    inline bool isAllDigits(const std::string& text) {
        return text.find_first_not_of("0123456789") == std::string::npos;
    }
}