#include "service/BookingService.h"

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>

#include "core/Queue.h"
#include "model/KhachHang.h"

namespace {

bool hasBookableFlight(const LinkedList<ChuyenBay>& flights) {
    for (std::size_t i = 0; i < flights.size(); ++i) {
        if (flights.at(i).isBookable()) {
            return true;
        }
    }
    return false;
}

}  // namespace

BookingService::BookingService(DataStore& store) : store_(store) {}

ChuyenBay& BookingService::requireFlight(const std::string& flightCode) {
    ChuyenBay* flight = store_.findFlight(flightCode);
    if (flight == nullptr) {
        throw std::invalid_argument("Khong tim thay chuyen bay: " + flightCode);
    }
    return *flight;
}

void BookingService::undoTicket(ChuyenBay& flight, const Ve& ticket) {
    try {
        store_.deleteTicket(ticket.getCode());
    } catch (const std::exception&) {
        // the ticket file may never have been written: nothing to remove
    }
    flight.releaseSeat(ticket.getSeatNumber());
}

Ve BookingService::issueTicket(ChuyenBay& flight, int seat,
                               const std::string& idNumber,
                               const std::string& fullName) {
    // throws before anything changes if the booking is not allowed
    const Ve ticket = flight.bookSeat(seat, idNumber, fullName);
    try {
        store_.saveTicket(ticket);
        store_.saveFlights();
    } catch (...) {
        undoTicket(flight, ticket);
        throw;
    }
    return ticket;
}

KhachHang BookingService::requestTicket(const std::string& idNumber,
                                        const std::string& fullName) {
    if (!hasBookableFlight(store_.getFlights())) {
        throw std::invalid_argument("Hien khong co chuyen bay nao con ve.");
    }
    // the constructor validates the ID and name before anything changes
    const KhachHang customer(store_.nextCustomerOrder(), idNumber, fullName);
    store_.enqueueCustomer(customer);
    return customer;
}

Ve BookingService::bookNextWaiting(const std::string& flightCode, int seat) {
    Queue<KhachHang>& waiting = store_.waitingQueue();
    if (waiting.isEmpty()) {
        throw std::invalid_argument("Khong co khach hang nao dang cho mua ve.");
    }
    ChuyenBay& flight = requireFlight(flightCode);

    const KhachHang customer = waiting.front();  // copy: dequeue frees the node
    const Ve ticket =
        issueTicket(flight, seat, customer.getIdNumber(), customer.getFullName());

    waiting.dequeue();
    try {
        store_.saveCustomers();
    } catch (...) {
        // put the customer back at the FRONT, then undo the ticket
        Queue<KhachHang> restored;
        restored.enqueue(customer);
        for (std::size_t i = 0; i < waiting.size(); ++i) {
            restored.enqueue(waiting.at(i));
        }
        waiting = restored;
        undoTicket(flight, ticket);
        throw;
    }
    return ticket;
}

void BookingService::returnTicket(const std::string& flightCode, int seat) {
    ChuyenBay& flight = requireFlight(flightCode);

    const LinkedList<Ve>& tickets = flight.getTickets();
    std::size_t position = 0;
    bool found = false;
    for (std::size_t i = 0; i < tickets.size(); ++i) {
        if (tickets.at(i).getSeatNumber() == seat) {
            position = i;
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::invalid_argument("Khong co ve nao o ghe " + std::to_string(seat) + ".");
    }

    const Ve ticket = tickets.at(position);  // copy: releaseSeat removes the node
    flight.releaseSeat(seat);                // throws for a completed flight

    try {
        store_.deleteTicket(ticket.getCode());
        store_.saveFlights();
    } catch (...) {
        flight.restoreTicket(ticket);
        try {
            store_.saveTicket(ticket);
        } catch (const std::exception&) {
            // best effort: the original error is the one to report
        }
        throw;
    }
}