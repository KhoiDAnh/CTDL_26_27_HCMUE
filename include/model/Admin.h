// Admin.h
#pragma once

#include <string>

class Admin {
private:
    std::string username_;
    std::string password_;
public:
    // constructor
    Admin(const std::string& username, const std::string& password);

    // username getter, no password getter cuz no one needs it
    const std::string& getUsername() const;

    bool matches(const std::string& username, const std::string& password) const;

    // chuan hoa
    std::string toLine() const;
    static Admin fromLine(const std::string& line);
};

// ChuyenBay.h
#pragma once

#include <string>

#include "model/Date.h"
#include "model/Ve.h"
#include "core/StringUtils.h"
#include "core/LinkedList.h"

class ChuyenBay {
private:
    std::string flightCode_;
    std::string aircraftId_;
    std::string destination_;
    Date departureDate_;
    int seatCount_;
    bool completed_;
    bool cancelled_;
    LinkedList<Ve> tickets_;
    LinkedList<int> freeSeats_;

    void place(const Ve& ticket);
public:
    // named constants
    enum class Status { Cancelled = 0, Available = 1, SoldOut = 2, Completed = 3 };
    ChuyenBay(const std::string& flightCode, const std::string& aircraftId,
                const Date& departureDate, const std::string& destination,
                int seatCount);

    // getters
    const std::string& getFlightCode() const;
    const std::string& getAircraftId() const;
    const Date& getDepartureDate() const;
    const std::string& getDestination() const;
    int getSeatCount() const;
    Status getStatus() const;
    const LinkedList<Ve>& getTickets() const;
    const LinkedList<int>& getFreeSeats() const;

    // chuan hoa
    static ChuyenBay fromLine(const std::string& line, int seatCount);
    std::string toLine() const;

    // core methods
    bool isBookable() const;
    Ve bookSeat(int seat, const std::string& idNumber, const std::string& fullName);
    void restoreTicket(const Ve& ticket);
    void releaseSeat(int seat);
    void cancel();
    void markCompleted();
};

// Date.h
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

// KhachHang.h
// constraints: order_ >= 1, idNumber_.size() = 12,
// fullName_ not blank && no |

#pragma once

#include <string>

class KhachHang {
private:
    int order_;
    std::string idNumber_;
    std::string fullName_;
public:
    KhachHang(int order, const std::string& idNumber, const std::string& fullName);

    int getOrder() const;
    const std::string& getIdNumber() const;
    const std::string& getFullName() const;

    // no setters, getters marked const
    std::string toLine() const;
    static KhachHang fromLine(const std::string& line);

    static void validateIDNumber(const std::string& idNumber);
    static void validateFullName(const std::string& fullName);
};