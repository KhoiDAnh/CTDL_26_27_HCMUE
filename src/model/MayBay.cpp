// MayBay.cpp
#include "model/MayBay.h"

#include <cstddef>
#include <stdexcept>
#include <string>

#include "core/StringUtils.h"

// problems need to solve:
// blankID, blankSeatCount
// no | (seatCount)
// VN-A123 | abc (stoi - string to integer))
// VN-A123 | 180abc (size check)
// VN-A123 | 1 | 2 (size check)
MayBay::MayBay(const std::string& id, int seatCount)
    : id_(id), seatCount_(seatCount) {
    if (StringUtils::isBlank(id)) {
        throw std::invalid_argument("MayBay: ID MayBay khong duoc trong.");
    }
    if (id.find('|') != std::string::npos) {
        throw std::invalid_argument("MayBay: ID MayBay khong duoc co dau '|'.");
    }
    if (seatCount <= 0) {
        throw std::invalid_argument("MayBay: So luong ghe phai > 0.");
    }
}

// oop stuff
const std::string& MayBay::getId() const {
    return id_;
}

int MayBay::getSeatCount() const {
    return seatCount_;
}

std::string MayBay::toLine() const {
    return id_ + "|" + std::to_string(seatCount_);
}

MayBay MayBay::fromLine(const std::string& line) {
    // MayBay (string id_, int seatCnt)
    std::string fields[2];
    if (!StringUtils::splitExact(line, '|', fields, 2)) {
        throw std::invalid_argument("MayBay: line phai co it nhat 2 vung du lieu.");
    }
    return MayBay{fields[0], StringUtils::parseInt(fields[1])};
}