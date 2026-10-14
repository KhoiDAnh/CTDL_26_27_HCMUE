#pragma once

#include <string>

class MayBay {
private:
    std::string id_;
    int seatCount_;
public:
    // constructor needs id + seatCnt
    MayBay(const std::string& id, int seatCount);

    // read only, const makes it so it doesnt change MayBay
    const std::string& getId() const;
    int getSeatCount() const;

    // MayBay IS FIXED ONCE BUILD!

    std::string toLine() const;
    static MayBay fromLine(const std::string& line);
};