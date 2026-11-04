#include "storage/DataStore.h"

#include <cstddef>
#include <stdexcept>
#include <string>

#include "core/StringUtils.h"
#include "storage/FileRepository.h"

namespace {
    const char* const kAircraftFile = "MayBay.txt";
    const char* const kFlightFile = "ChuyenBay.txt";
    const char* const KAdminFile = "Admin.txt";
    const char* const kCustomerFile = "KhachHang.txt";
    const char* const kTicketFolder = "tickets/";

    const MayBay* findAircraftIn(const LinkedList<MayBay>& list, const std::string& id) {
        // loop then return ADDRESS of matching element id
        for (std::size_t i = 0; i < list.size(); ++i) {
            if (list.at(i).getId() == id) {
                return &list.at(i);
            }
        }
        return nullptr;
    }
} // namespace

DataStore::DataStore(const std::string& dataDir) : dataDir_(dataDir){
    // a folder path needs trailing seperator so file names can be appended
    if (!dataDir_.empty() && dataDir_.back() != '/' && dataDir_.back() != '\\') {
        dataDir_ += '/';
    }
}

std::string DataStore::ticketPath(const std::string& ticketCode) const {
    // PATH for each ticket
    return dataDir_ + kTicketFolder + ticketCode + ".txt";
}

void DataStore::load() {
    /// everything is built into LOCAL lists, the members are replaced only at the very end,
    /// if failure at any step -> previous data untouched

    // 1. MayBay
    const LinkedList<MayBay> aircraft =
        FileRepository<MayBay>::loadAll(
            dataDir_ + kAircraftFile,
            &MayBay::fromLine, [] (const MayBay& plane) {
                return plane.getId();
            });

    // 2. ChuyenBay -> need its MayBay's seat count
    LinkedList<ChuyenBay> flights =
        FileRepository<ChuyenBay>::loadAll(
            dataDir_ + kFlightFile,
            [&aircraft] (const std::string& line) {
                std::string fields[5];
                if (!StringUtils::splitExact(line, '|', fields, 5)) {
                    throw std::invalid_argument("Chuoi phai co dung 5 vung du lieu.");
                }
                const MayBay* plane = findAircraftIn(aircraft, fields[1]);
                if (plane == nullptr) {
                    throw std::invalid_argument("Khong tim thay so hieu may bay: " + fields[1]);
                }
                return ChuyenBay::fromLine(line, plane->getSeatCount());
            },
            [] (const ChuyenBay& flight) {
                return flight.getFlightCode();
            });

    // 3. Ve -> restore into LOCAL flights
    loadTickets(flights);

    // 4. admins and waiting KhachHang
    const LinkedList<Admin> admins =
        FileRepository<Admin>::loadAll(
            dataDir_ + KAdminFile,
            &Admin::fromLine,
            [] (const Admin& admin)
            { return admin.getUsername(); });

    const LinkedList<KhachHang> customers =
        FileRepository<KhachHang>::loadAll(
            dataDir_ + kCustomerFile,
            &KhachHang::fromLine,
            [] (const KhachHang& customer)
            { return std::to_string(customer.getOrder()); });

    // 5. if nothing above throws, replace the members
    aircraft_ = aircraft;
    flights_ = flights;
    customers_ = customers;
    admins_ = admins;
}

void DataStore::loadTickets(LinkedList<ChuyenBay>& flights) const {
    // Note: a ticket file for a seat > seatCount || flight no longer exists
    // -> never probed, ignored silently

    for (std::size_t i = 0; i < flights.size(); ++i) {
        ChuyenBay& flight = flights.at(i);

        for (int seat = 1; i <= flight.getSeatCount(); ++seat) {
            const std::string path =
                ticketPath(Ve::makeCode(flight.getFlightCode(), seat));
            if (!FileRepository<Ve>::exists(path)) {
                 continue; // ~no ticket for this seat
            }

            const Ve ticket = FileRepository<Ve>::loadOne(path, &Ve::fromLine);

            try {
                if (ticket.getSeatNumber() != seat) {
                    throw std::invalid_argument(
                        "DataStore: Ten file ghi ghe + " + std::to_string(seat) +
                        " nhung noi dung ghi ghe " +
                        std::to_string(ticket.getSeatNumber()));
                }
                flight.restoreTicket(ticket);
            } catch (const std::invalid_argument& e) {
                throw std::invalid_argument("DataStore: " + path + ": " + e.what());
            }
        }
    }
}

const LinkedList<MayBay>& DataStore::getAircraft() const {
    return aircraft_;
}

const LinkedList<ChuyenBay>& DataStore::getFlights() const {
    return flights_;
}

const LinkedList<Admin>& DataStore::getAdmins() const {
    return admins_;
}

const LinkedList<KhachHang>& DataStore::getCustomers() const {
    return customers_;
}

ChuyenBay* DataStore::findFlight(const std::string& flightCode) {
    // loop -> index of getFlightCode() match flightCode -> return ADDRESS &flights_.at(i)
    for (std::size_t i = 0; i < flights_.size(); ++i) {
        if (flights_.at(i).getFlightCode() == flightCode) {
            return &flights_.at(i);
        }
    }
    return nullptr;
}

const ChuyenBay* DataStore::findFlight(const std::string& flightCode) const {
    for (std::size_t i = 0; i < flights_.size(); ++i) {
        if (flights_.at(i).getFlightCode() == flightCode) {
            return &flights_.at(i);
        }
    }
    return nullptr;
}

const MayBay* DataStore::findAircraft(const std::string& aircraftId) const {
    return findAircraftIn(aircraft_, aircraftId);
}

void DataStore::saveTicket(const Ve& ticket) const {
    FileRepository<Ve>::saveOne(ticketPath(ticket.getCode()), ticket);
}

void DataStore::deleteTicket(const std::string& ticketCode) const {
    FileRepository<Ve>::removeFile(ticketPath(ticketCode));
}

void DataStore::saveFlights() const {
    FileRepository<ChuyenBay>::saveAll(dataDir_ + kFlightFile, flights_);
}