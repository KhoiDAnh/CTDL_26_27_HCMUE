#ifndef CTDL_26_27_HCMUE_BOOKINGSERVICE_H
#define CTDL_26_27_HCMUE_BOOKINGSERVICE_H

#include <cstddef>
#include <stdexcept>
#include <string>

#include "storage/DataStore.h"
#include "storage/DataStore.h"
#include "service/BookingService.h"
class BookingService {
private:
    DataStore& store_;

public:
    explicit BookingService(DataStore& store)
        : store_(store) {}

    Ve bookTicket(
        const std::string& flightCode,
        const std::string& idNumber,
        int seatNumber
    ) {

        const LinkedList<KhachHang>& customers =
            store_.getCustomers();

        const KhachHang* customer = nullptr;

        for (std::size_t i = 0; i < customers.size(); ++i) {
            if (customers.at(i).getIdNumber() == idNumber) {
                customer = &customers.at(i);
                break;
            }
        }

        if (customer == nullptr) {
            throw std::invalid_argument(
            );
        }

        const std::string customerId = customer->getIdNumber();
        const std::string customerName = customer->getFullName();

        ChuyenBay* flight = store_.findFlight(flightCode);

        if (flight == nullptr) {
            throw std::invalid_argument(
                "Khong tim thay chuyen bay."
            );
        }

        if (!flight->isBookable()) {
            throw std::invalid_argument(
                "Chuyen bay khong the dat ve."
            );
        }

        Ve ticket = flight->bookSeat(
            seatNumber,
            customerId,
            customerName
        );

        try {
            store_.saveTicket(ticket);
        }
        catch (...) {
            flight->releaseSeat(seatNumber);
            throw;
        }

        try {
            if (!store_.removeWaitingCustomer(customerId)) {
                throw std::runtime_error(
                    "Khong the xoa khach hang khoi danh sach cho."
                );
            }
        }
        catch (...) {

            try {
                store_.deleteTicket(ticket.getCode());
            }
            catch (...) {

            }

            flight->releaseSeat(seatNumber);
            throw;
        }

        return ticket;
    }

    bool returnTicket(
        const std::string& flightCode,
        int seatNumber
    ) {
        ChuyenBay* flight = store_.findFlight(flightCode);

        if (flight == nullptr) {
            throw std::invalid_argument(
                "Khong tim thay chuyen bay."
            );
        }

        if (flight->getStatus() == ChuyenBay::Status::Completed) {
            throw std::invalid_argument(
                "Chuyen bay da hoan tat, khong the tra ve."
            );
        }

        const LinkedList<Ve>& tickets = flight->getTickets();

        const Ve* foundTicket = nullptr;

        for (std::size_t i = 0; i < tickets.size(); ++i) {
            if (tickets.at(i).getSeatNumber() == seatNumber) {
                foundTicket = &tickets.at(i);
                break;
            }
        }

        if (foundTicket == nullptr) {
            throw std::invalid_argument(
                "Khong tim thay ve o so ghe nay."
            );
        }

        const Ve ticket = *foundTicket;

        flight->releaseSeat(seatNumber);

        try {
            store_.deleteTicket(ticket.getCode());
        }
        catch (...) {
            flight->restoreTicket(ticket);
            throw;
        }

        return true;
    }
};

#endif
