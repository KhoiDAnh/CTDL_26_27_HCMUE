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