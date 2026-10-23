// Ve.h
#pragma once

#include <string>

class Ve {
private:
    std::string flightCode_;
    int seatNumber_;
    std::string idNumber_;
    std::string fullName_;
public:
    Ve(const std::string& flightCode, int seatNumber,
        const std::string& idNumber, const std::string& fullName);

    const std::string& getFlightCode() const;
    int getSeatNumber() const;
    const std::string& getIdNumber() const;
    const std::string& getFullName() const;
    std::string getCode() const;

    std::string toLine() const;
    static Ve fromLine(const std::string& line);
    static void validateFlightCode(const std::string& flightCode);
};