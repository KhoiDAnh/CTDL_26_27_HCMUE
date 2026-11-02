#include "model/ChuyenBay.h"
#include "core/StringUtils.h"
#include <sstream>
#include <vector>

void ChuyenBay::place(const Ve& ticket) {
    this->tickets_.pushBack(ticket);

    for (std::size_t i = 0; i < this->freeSeats_.size(); ++i) {
        if (this->freeSeats_.at(i) == ticket.getSeatNumber()) {
            this->freeSeats_.removeAt(i);
            break;
        }
    }
}
ChuyenBay::ChuyenBay(const std::string& flightCode, const std::string& aircraftId,
    const Date& departureDate, const std::string& destination,
    int seatCount)
    : flightCode_(flightCode), aircraftId_(aircraftId), destination_(destination),
    departureDate_(departureDate), seatCount_(seatCount), completed_(false), cancelled_(false) {

    for (int i = 1; i <= seatCount; ++i) {
        this->freeSeats_.pushBack(i);
    }
}

const std::string& ChuyenBay::getFlightCode() const { return this->flightCode_; }
const std::string& ChuyenBay::getAircraftId() const { return this->aircraftId_; }
const Date& ChuyenBay::getDepartureDate() const { return this->departureDate_; }
const std::string& ChuyenBay::getDestination() const { return this->destination_; }
int ChuyenBay::getSeatCount() const { return this->seatCount_; }
const LinkedList<Ve>& ChuyenBay::getTickets() const { return this->tickets_; }
const LinkedList<int>& ChuyenBay::getFreeSeats() const { return this->freeSeats_; }

ChuyenBay::Status ChuyenBay::getStatus() const {
    if (this->cancelled_) return Status::Cancelled;
    if (this->completed_) return Status::Completed;
    if (this->freeSeats_.isEmpty()) return Status::SoldOut;
    return Status::Available;
}

bool ChuyenBay::isBookable() const {
    return this->getStatus() == Status::Available;
}

Ve ChuyenBay::bookSeat(int seat, const std::string& idNumber, const std::string& fullName) {
    if (!this->isBookable()) {
        throw std::logic_error("Chuyen bay khong the dat ve.");
    }

    Ve newTicket(this->flightCode_, seat, idNumber, fullName);

    this->place(newTicket);

    return newTicket;
}

void ChuyenBay::restoreTicket(const Ve& ticket) {
    this->place(ticket);
}

void ChuyenBay::releaseSeat(int seat) {
    for (std::size_t i = 0; i < this->tickets_.size(); ++i) {
        if (this->tickets_.at(i).getSeatNumber() == seat) {
            this->tickets_.removeAt(i);
            break;
        }
    }

    bool bInserted = false;
    for (std::size_t i = 0; i < this->freeSeats_.size(); ++i) {
        if (this->freeSeats_.at(i) > seat) {
            LinkedList<int> tempList;
            for (std::size_t j = 0; j < this->freeSeats_.size(); ++j) {
                if (j == i) tempList.pushBack(seat);
                tempList.pushBack(this->freeSeats_.at(j));
            }
            this->freeSeats_ = tempList;
            bInserted = true;
            break;
        }
    }
    if (!bInserted) {
        this->freeSeats_.pushBack(seat); 
    }
}

void ChuyenBay::cancel() {
    this->cancelled_ = true;
}

void ChuyenBay::markCompleted() {
    this->completed_ = true;
}

static int statusToInt(ChuyenBay::Status status) {
    return static_cast<int>(status);
}

std::string ChuyenBay::toLine() const {
    std::stringstream ss;
    ss << this->flightCode_ << "|"
        << this->aircraftId_ << "|"
        << this->departureDate_.toString() << "|"
        << this->destination_ << "|"
        << statusToInt(this->getStatus());
    return ss.str();
}

ChuyenBay ChuyenBay::fromLine(const std::string& line, int seatCount) {
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;

    while (std::getline(ss, item, '|')) {
        tokens.push_back(item);
    }

    if (tokens.size() < 5) {
        throw std::invalid_argument("ChuyenBay: Du lieu khong hop le.");
    }

    std::string flightCode = tokens[0];
    std::string aircraftId = tokens[1];
    std::string dateStr = tokens[2];
    std::string destination = tokens[3];
    int statusInt = std::stoi(tokens[4]);

    Date date = Date::fromString(dateStr);

    ChuyenBay cb(flightCode, aircraftId, date, destination, seatCount);

    if (statusInt == 0) cb.cancel();
    if (statusInt == 3) cb.markCompleted();

    return cb;
}