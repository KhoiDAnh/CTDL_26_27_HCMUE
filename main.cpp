#include <iostream>
#include <stdexcept>
#include <string>

#include "model/KhachHang.h"
#include "model/Ve.h"
#include "model/Date.h"

// ThêmIncludeM5
#include "core/LinkedList.h"
#include "model/ChuyenBay.h"
#include "service/StatisticsService.h"

void checkKhachHang(const std::string& line) {
    try {
        KhachHang::fromLine(line);
        std::cout << "OK: " << line << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "THROWS: " << line << " -> " << e.what() << '\n';
    }
}

void checkVe(const std::string& line) {
    try {
        const Ve ticket = Ve::fromLine(line);
        std::cout << "OK : " << line << "   code=" << ticket.getCode()
                  << "   roundtrip=" << (ticket.toLine() == line ? "yes" : "NO") << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "THROWS: " << line << " -> " << e.what() << '\n';
    }
}

void checkDate(const std::string& line) {
    try {
        Date::fromString(line);
        std::cout << "OK: " << line << '\n';
    }
    catch (const std::invalid_argument& e) {
        std::cout << "THROWS: " << line << " -> " << e.what() << '\n';
    }
}

int main() {
    std::cout << "=== TEST CODE CUA M3 ===\n";

    std::cout << "\n=== BAT DAU TEST MODULE M5 (STATISTICS) ===\n";

    LinkedList<ChuyenBay> dsChuyenBay;

    Date d1(20, 5, 2024);

    ChuyenBay cb1("VN12", "VN-A321", d1, "Ha Noi", 130);

    cb1.bookSeat(10, "079201001284", "Nguyen Van A");

    ChuyenBay cb2("VN34", "VN-A320", d1, "Da Nang", 300);

    dsChuyenBay.pushBack(cb1);
    dsChuyenBay.pushBack(cb2);

    StatisticsService stats;

    stats.showPassengersOfFlight("VN12", dsChuyenBay);

    stats.showAvailableSeats("VN12", dsChuyenBay);

    stats.countFlightsOfAircraft("VN-A321", dsChuyenBay);

    return 0;
}