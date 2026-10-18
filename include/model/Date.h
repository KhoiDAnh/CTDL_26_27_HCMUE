//
// Created by KHOI DO on 18/10/2026.
//

#ifndef CTDL_26_27_HCMUE_NGAY_H
#define CTDL_26_27_HCMUE_NGAY_H

#pragma once

#include <string>

class Date {
private:
    int day_, month_, year_;
public:
    Date(int day, int month, int year);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    std::string toString() const;
    static Date fromString(const std::string& text);
};

#endif //CTDL_26_27_HCMUE_NGAY_H
