#pragma once

#include <string>

#include "model/Ve.h"
#include "storage/DataStore.h"

class BookingService {
public:
    explicit BookingService(DataStore& store);

    // Public "Dat ve": the customer types the ticket's data.
    Ve bookDirect(const std::string& flightCode, int seat,
                  const std::string& idNumber, const std::string& fullName);

    // Staff "Xu ly dat ve": turns the FRONT waiting customer into a ticket.
    Ve bookNextWaiting(const std::string& flightCode, int seat);

    // Staff "Xu ly tra ve". Throws on any failure.
    void returnTicket(const std::string& flightCode, int seat);

private:
    ChuyenBay& requireFlight(const std::string& flightCode);
    Ve issueTicket(ChuyenBay& flight, int seat, const std::string& idNumber,
                   const std::string& fullName);
    void undoTicket(ChuyenBay& flight, const Ve& ticket);

    DataStore& store_;
};