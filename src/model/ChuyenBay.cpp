// ChuyenBay.cpp
#include "model/ChuyenBay.h"
#include "model/Ve.h"

#include <stdexcept>
#include <string>

#include "core/StringUtils.h"


ChuyenBay::ChuyenBay(const std::string &flightCode,
        const std::string &aircraftId,
        const Date &departureDate,
        const std::string &destination,
        int seatCount)
    : flightCode_(flightCode),
    aircraftId_(aircraftId),
    departureDate_(departureDate),
    destination_(destination),
    seatCount_(seatCount),
    cancelled_(false),
    completed_(false) {
    // 1. validate flight code
    Ve::validateFlightCode(flightCode);

    // 2. validate aircraftId
    if (StringUtils::isBlank(aircraftId)) {
        throw std::invalid_argument("ChuyenBay: Ma chuyen bay khong duoc trong.");
    }
    if (aircraftId.find('|') != std::string::npos) {
        throw std::invalid_argument("ChuyenBay: Ma chuyen bay chua ky tu khong hop le.");
    }

    // 3. validate destination
    if (StringUtils::isBlank(destination)) {
        throw std::invalid_argument("ChuyenBay: Diem den khong duoc trong.");
    }
    if (destination.find('|') != std::string::npos) {
        throw std::invalid_argument("ChuyenBay: Diem den chua ky tu khong hop le.");
    }
    // 4. validate seatCount
    if (seatCount <= 0) {
        throw std::invalid_argument("ChuyenBay: So ghe phai lon hon 0.");
    }
    // 5. fill freeSeats with 1 -> seatCount
    for (int seat = 1; seat <= seatCount; ++seat) {
        freeSeats_.pushBack(seat);
    }
}

const std::string & ChuyenBay::getFlightCode() const {
    return flightCode_;
}

const std::string& ChuyenBay::getAircraftId() const {
    return aircraftId_;
}

const Date& ChuyenBay::getDepartureDate() const {
    return departureDate_;
}

const std::string& ChuyenBay::getDestination() const {
    return destination_;
}

int ChuyenBay::getSeatCount() const {
    return seatCount_;
}

ChuyenBay::Status ChuyenBay::getStatus() const {
    // 4 checks for enum Status::VALUE
    if (cancelled_) {
        return Status::Cancelled;
    }
    if (completed_) {
        return Status::Completed;
    }
    if (freeSeats_.isEmpty()) {
        return Status::SoldOut;
    }
    return Status::Available;
}

const LinkedList<Ve>& ChuyenBay::getTickets() const {
    return tickets_;
}

const LinkedList<int>& ChuyenBay::getFreeSeats() const {
    return freeSeats_;
}

bool ChuyenBay::isBookable() const {
    return getStatus() == Status::Available;
}

void ChuyenBay::cancel() {
    // "Open": cancelled_ || completed_
    // 1. If not open -> throw error
    if (cancelled_ || completed_) {
        throw std::invalid_argument("ChuyenBay: Chuyen bay da huy hoac da hoan thanh, khong the huy.");
    }

    // 2. else set cancelled_
    cancelled_ = true;
}

void ChuyenBay::markCompleted() {
    // "Open": cancelled_ || completed_
    // 1. If not open -> throw error
    if (cancelled_ || completed_) {
        throw std::invalid_argument("ChuyenBay: Chuyen bay da huy hoac da hoan thanh, khong the huy.");
    }

    // 2. else set completed_
    completed_ = true;
}

void ChuyenBay::place(const Ve& ticket) {
    const int seat = ticket.getSeatNumber();

    // 1. ticket must belong to this flight
    if (ticket.getFlightCode() != flightCode_) {
        throw std::invalid_argument("ChuyenBay: Ve khong thuoc chuyen bay nay.");
    }

    // 2. seat must exist on aircraft
    if (seat < 1 || seat > seatCount_) {
        throw std::invalid_argument("ChuyenBay: So ghe nam ngoai pham vi chuyen bay.");
    }

    // 3. READ-ONLY: the seat must be free -> find its position
    std::size_t position = 0;
    bool found = false;
    for (std::size_t i = 0; i < freeSeats_.size(); ++i) {
        if (freeSeats_.at(i) == seat) {
            position = i;
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::invalid_argument("ChuyenBay: Ghe nay da duoc ban.");
    }

    // 4. push ticket into tickets_ (malloc -> first)
    tickets_.pushBack(ticket);

    // 5. removing sold ticket from freeSeats (position found -> last)
    freeSeats_.removeAt(position);
}

Ve ChuyenBay::bookSeat(int seat, const std::string& idNumber,
                       const std::string& fullName) {
    const Status status = getStatus();

    // 1. status checks
    if (status == Status::Cancelled) {
        throw std::invalid_argument("ChuyenBay: Khong the dat ve chuyen bay da huy.");
    }
    if (status == Status::Completed) {
        throw std::invalid_argument("ChuyenBay: Khong the dat ve chuyen bay da hoan thanh.");
    }
    if (status == Status::SoldOut) {
        throw std::invalid_argument("ChuyenBay: Khong the dat ve chuyen bay da het ve.");
    }

    // 2. Build new ticket with *this flightCode_
    const Ve ticket(flightCode_, seat, idNumber, fullName);

    // 3. place ticket in tickets_
    place(ticket);

    // 4. Return ticket -> BookingService can save files
    return ticket;
}

void ChuyenBay::restoreTicket(const Ve& ticket) {
    // no status checks on purpose: completed && cancelled flights must reload their tickets
    place(ticket);
}

void ChuyenBay::releaseSeat(int seat) {
    // 1. return failed only for completed flights
    if (getStatus() == Status::Completed) {
        throw std::invalid_argument("ChuyenBay: Khong the huy ve cua chuyen bay da hoan thanh.");
    }

    // 2. Find ticket with this seat, remember position
    std::size_t ticketPos = 0;
    bool found = false;
    for (std::size_t i = 0; i < tickets_.size(); ++i) {
        if (tickets_.at(i).getSeatNumber() == seat) {
            ticketPos = i;
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::invalid_argument("ChuyenBay: Khong co ve duoc dat o ghe nay.");
    }
    // 3. Find seat in sorted freeSeats_
    std::size_t insertPos = freeSeats_.size();
    for (std::size_t i = 0; i < freeSeats_.size(); ++i) {
        if (freeSeats_.at(i) > seat) {
            insertPos = i;
            break;
        }
    }

    // 4. Insert seat + remove ticket
    freeSeats_.insertAt(insertPos, seat); // (malloc -> first)
    tickets_.removeAt(ticketPos); // (position found -> last)
}

std::string ChuyenBay::toLine() const {
    // Format: flightCode|aircraftId|dd/mm/yyyy|destination|status
    return flightCode_ + "|" + aircraftId_ + "|" + departureDate_.toString() + "|"
        + destination_ + "|" + std::to_string(static_cast<int>(getStatus()));
}

ChuyenBay ChuyenBay::fromLine(const std::string& line, int seatCount) {
    // Format: flightCode|aircraftId|dd/mm/yyyy|destination|status
    // 1. initialize fields
    std::string fields[5];

    if (!StringUtils::splitExact(line, '|', fields, 5)) {
        throw std::invalid_argument("ChuyenBay: Chuoi phai co du 5 vung du lieu.");
    }

    // 2. date && status check
    const Date date = Date::fromString(fields[2]); // throw a bad date itself
    const int status = StringUtils::parseInt(fields[4]);
    if (status < 0 || status > 3) {
        throw std::invalid_argument("ChuyenBay: Tinh trang khong hop le.");
    }

    // 3. Building ChuyenBay
    ChuyenBay flight(fields[0],
        fields[1], date, fields[3], seatCount);

    // 4. Correspond status codes
    // no 1 and 2 cause value is recomputed from free seats
    if (status == 0)
        flight.cancel();
    if (status == 3)
        flight.markCompleted();

    // 5. Return
    return flight;
}