#pragma once

#include <string>

#include "core/LinkedList.h"
#include "model/Admin.h"
#include "model/ChuyenBay.h"
#include "model/KhachHang.h"
#include "model/MayBay.h"
#include "model/Ve.h"

class DataStore {
private:
    std::string ticketPath(const std::string& ticketCode) const;
    void loadTickets(LinkedList<ChuyenBay>& flights) const;

    std::string dataDir_;
    LinkedList<MayBay> aircraft_;
    LinkedList<ChuyenBay> flights_;
    LinkedList<Admin> admins_;
    LinkedList<KhachHang> customers_;
public:
    explicit DataStore(const std::string& dataDir);

    void load();

    const LinkedList<MayBay>& getAircraft() const;
    const LinkedList<ChuyenBay>& getFlights() const;
    const LinkedList<Admin>& getAdmins() const;
    const LinkedList<KhachHang>& getCustomers() const;

    ChuyenBay* findFlight(const std::string& flightCode);
    const ChuyenBay* findFlight(const std::string& flightCode) const;
    const MayBay* findAircraft(const std::string& aircraftId) const;

    void saveTicket(const Ve& ticket) const;
    void deleteTicket(const std::string& ticketCode) const;
    void saveFlights() const;
};