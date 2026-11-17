#pragma once

#include <string>

#include "model/KhachHang.h"
#include "model/Ve.h"
#include "storage/DataStore.h"

class BookingService {
private:
    ChuyenBay& requireFlight(const std::string& flightCode);
    Ve issueTicket(ChuyenBay& flight, int seat, const std::string& idNumber,
                   const std::string& fullName);
    void undoTicket(ChuyenBay& flight, const Ve& ticket);

    DataStore& store_;
public:
    explicit BookingService(DataStore& store);

    // Public "Dat ve": the customer joins the waiting queue. No ticket exists yet.
    KhachHang requestTicket(const std::string& idNumber, const std::string& fullName);

    // Staff "Xu ly dat ve": turns the FRONT waiting customer into a ticket.
    Ve bookNextWaiting(const std::string& flightCode, int seat);

    // Staff "Xu ly tra ve". Throws on any failure.
    void returnTicket(const std::string& flightCode, int seat);

};