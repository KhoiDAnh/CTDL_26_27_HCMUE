#include "service/StatisticsService.h"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

const ChuyenBay& requireFlight(const LinkedList<ChuyenBay>& flights,
                               const std::string& flightCode) {
    for (std::size_t i = 0; i < flights.size(); ++i) {
        if (flights.at(i).getFlightCode() == flightCode) {
            return flights.at(i);
        }
    }
    throw std::invalid_argument("Khong tim thay chuyen bay: " + flightCode);
}

}  // namespace

void StatisticsService::showPassengersOfFlight(
    const std::string& flightCode, const LinkedList<ChuyenBay>& flights) const {
    const ChuyenBay& flight = requireFlight(flights, flightCode);
    const LinkedList<Ve>& tickets = flight.getTickets();

    std::cout << "\n Danh sach khach cua chuyen bay " << flightCode << " ("
              << tickets.size() << " ve)\n";
    if (tickets.isEmpty()) {
        std::cout << " Chuyen bay chua co khach hang nao.\n";
        return;
    }

    std::cout << std::left << ' ' << std::setw(6) << "STT" << std::setw(14) << "Ma ve"
              << std::setw(7) << "Ghe" << std::setw(15) << "CMND" << "Ho ten\n";
    std::cout << ' ' << std::string(62, '-') << '\n';
    for (std::size_t i = 0; i < tickets.size(); ++i) {
        const Ve& ticket = tickets.at(i);
        std::cout << ' ' << std::setw(6) << (i + 1) << std::setw(14) << ticket.getCode()
                  << std::setw(7) << ticket.getSeatNumber() << std::setw(15)
                  << ticket.getIdNumber() << ticket.getFullName() << '\n';
    }
}

void StatisticsService::showAvailableSeats(
    const std::string& flightCode, const LinkedList<ChuyenBay>& flights) const {
    const ChuyenBay& flight = requireFlight(flights, flightCode);
    const LinkedList<int>& seats = flight.getFreeSeats();

    std::cout << "\n Ghe con trong cua chuyen bay " << flightCode << ": " << seats.size()
              << "/" << flight.getSeatCount() << "\n";
    if (seats.isEmpty()) {
        std::cout << " Da het ghe!\n";
        return;
    }

    std::cout << std::right;
    for (std::size_t i = 0; i < seats.size(); ++i) {
        std::cout << std::setw(5) << seats.at(i);
        if ((i + 1) % 10 == 0) {
            std::cout << '\n';
        }
    }
    if (seats.size() % 10 != 0) {
        std::cout << '\n';
    }
}

void StatisticsService::countFlightsOfAircraft(
    const std::string& aircraftId, const LinkedList<ChuyenBay>& flights) const {
    std::size_t total = 0;
    std::size_t completed = 0;
    std::size_t cancelled = 0;
    std::size_t scheduled = 0;

    for (std::size_t i = 0; i < flights.size(); ++i) {
        const ChuyenBay& flight = flights.at(i);
        if (flight.getAircraftId() != aircraftId) {
            continue;
        }
        ++total;
        switch (flight.getStatus()) {
            case ChuyenBay::Status::Completed:
                ++completed;
                break;
            case ChuyenBay::Status::Cancelled:
                ++cancelled;
                break;
            default:
                ++scheduled;
                break;
        }
    }

    std::cout << "\n Thong ke may bay " << aircraftId << ":\n";
    std::cout << "   Tong so chuyen bay        : " << total << '\n';
    std::cout << "   Da thuc hien (hoan tat)   : " << completed << '\n';
    std::cout << "   Da huy                    : " << cancelled << '\n';
    std::cout << "   Chua bay (con ve / het ve): " << scheduled << '\n';
}