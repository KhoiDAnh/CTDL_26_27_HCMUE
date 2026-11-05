// App.cpp
// Module M2: Hien thuc lop dieu khien trung tam cua ung dung
#include "ui/App.h"
#include "ui/Console.h"
#include "ui/Menu.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

// ==================== Constructor ====================

App::App(const std::string& dataDir)
    : store_(dataDir),
      auth_(store_.getAdmins()),
      stats_() {
}

// ==================== Tien ich ho tro ====================

std::string App::statusToString(ChuyenBay::Status status) {
    switch (status) {
        case ChuyenBay::Status::Cancelled: return "Huy chuyen";
        case ChuyenBay::Status::Available: return "Con ve";
        case ChuyenBay::Status::SoldOut:   return "Het ve";
        case ChuyenBay::Status::Completed: return "Hoan tat";
        default: return "Khong xac dinh";
    }
}

const char* App::statusToColor(ChuyenBay::Status status) {
    switch (status) {
        case ChuyenBay::Status::Cancelled: return Console::RED;
        case ChuyenBay::Status::Available: return Console::GREEN;
        case ChuyenBay::Status::SoldOut:   return Console::YELLOW;
        case ChuyenBay::Status::Completed: return Console::BLUE;
        default: return Console::WHITE;
    }
}

// ==================== 1. Hien thi danh sach chuyen bay ====================

void App::showFlightTable() const {
    Console::printHeader("DANH SACH CAC CHUYEN BAY");

    const LinkedList<ChuyenBay>& flights = store_.getFlights();

    if (flights.isEmpty()) {
        Console::printColor(Console::YELLOW, "Hien chua co chuyen bay nao trong he thong.\n");
        return;
    }

    // In tieu de cac cot
    std::cout << Console::CYAN
              << std::left
              << std::setw(12) << "Ma CB"
              << std::setw(14) << "So hieu MB"
              << std::setw(14) << "Ngay KH"
              << std::setw(18) << "San bay den"
              << std::setw(14) << "Trang thai"
              << std::setw(10) << "Da ban"
              << Console::RESET << '\n';

    Console::printLine(82, '-');

    // In chi tiet tung chuyen bay
    for (std::size_t i = 0; i < flights.size(); ++i) {
        const ChuyenBay& flight = flights.at(i);
        const char* color = statusToColor(flight.getStatus());

        std::cout << color
                  << std::left
                  << std::setw(12) << flight.getFlightCode()
                  << std::setw(14) << flight.getAircraftId()
                  << std::setw(14) << flight.getDepartureDate().toString()
                  << std::setw(18) << flight.getDestination()
                  << std::setw(14) << statusToString(flight.getStatus())
                  << std::setw(10) << flight.getTickets().size()
                  << Console::RESET << '\n';
    }

    Console::printLine(82, '-');
}

// ==================== 2. Dat ve chuyen bay ====================

void App::bookTicket() {
    Console::printHeader("DAT VE CHUYEN BAY");

    const std::string flightCode = Console::readLine("Nhap ma chuyen bay: ");

    ChuyenBay* flight = store_.findFlight(flightCode);
    if (flight == nullptr) {
        Console::printError("Khong tim thay chuyen bay: " + flightCode);
        return;
    }

    if (!flight->isBookable()) {
        Console::printError("Chuyen bay " + flightCode + " hien khong the dat ve ("
                            + statusToString(flight->getStatus()) + ").");
        return;
    }

    // Hien thi danh sach ghe con trong
    const LinkedList<int>& freeSeats = flight->getFreeSeats();
    std::cout << Console::CYAN << "Cac ghe con trong: " << Console::WHITE;
    for (std::size_t i = 0; i < freeSeats.size(); ++i) {
        std::cout << freeSeats.at(i) << " ";
    }
    std::cout << Console::RESET << '\n';

    // Nhap thong tin ve (khong de trong)
    const int seat = Console::readInt("Nhap so ghe muon dat: ");
    const std::string idNumber = Console::readLine("Nhap CMND khach hang (12 chu so): ");
    const std::string fullName = Console::readLine("Nhap ho ten khach hang: ");

    // Dat ve va luu tru
    const Ve ticket = flight->bookSeat(seat, idNumber, fullName);
    store_.saveTicket(ticket);
    store_.saveFlights();

    Console::printSuccess("Dat ve thanh cong! Ma ve: " + ticket.getCode());
}

// ==================== 3. Dang nhap quan ly ====================

void App::manageLogin() {
    Console::printHeader("DANG NHAP HE THONG");

    while (!auth_.isLocked()) {
        const std::string username = Console::readLine("User: ");
        const std::string password = Console::readPassword("Password: ");

        // Nguoi dung nhan phim ESC de thoat ra ngoai
        if (password.empty()) {
            Console::printColor(Console::YELLOW, "Nguoi dung da huy dang nhap.\n");
            return;
        }

        if (auth_.login(username, password)) {
            Console::printSuccess("Dang nhap thanh cong! Xin chao quan tri vien: " + username);
            adminMenu();
            return;
        }

        Console::printError("Sai ten dang nhap hoac mat khau. Con lai: "
                            + std::to_string(auth_.remainingAttempts()) + " lan thu.");
    }

    Console::printError("Da nhap sai qua 3 lan! He thong da bi khoa.");
}

// ==================== Menu quan ly ====================

void App::adminMenu() {
    while (true) {
        const int choice = Menu::showAdmin();

        switch (choice) {
            case 1:
                try {
                    processBooking();
                } catch (const std::invalid_argument& e) {
                    Console::printError(e.what());
                }
                break;
            case 2:
                try {
                    processReturn();
                } catch (const std::invalid_argument& e) {
                    Console::printError(e.what());
                }
                break;
            case 3:
                statisticsMenu();
                break;
            case 0:
                Console::printColor(Console::YELLOW, "Da dang xuat khoi quyen quan ly.\n");
                return;
            default:
                Console::printError("Lua chon khong hop le. Vui long chon lai.");
                break;
        }
        Console::pause();
    }
}

// ==================== Xu ly dat ve (Quan ly) ====================

void App::processBooking() {
    Console::printHeader("XU LY DAT VE CHO KHACH HANG CHO");

    const LinkedList<KhachHang>& customers = store_.getCustomers();
    if (customers.isEmpty()) {
        Console::printColor(Console::YELLOW, "Hien khong co khach hang nao trong danh sach cho.\n");
        return;
    }

    const KhachHang& customer = customers.front();
    std::cout << Console::CYAN << "Thong tin khach hang cho tiep theo:\n" << Console::RESET
              << "  STT:  " << customer.getOrder() << '\n'
              << "  CMND: " << customer.getIdNumber() << '\n'
              << "  Ten:  " << customer.getFullName() << '\n';

    const std::string flightCode = Console::readLine("Nhap ma chuyen bay: ");
    ChuyenBay* flight = store_.findFlight(flightCode);
    if (flight == nullptr) {
        Console::printError("Khong tim thay chuyen bay: " + flightCode);
        return;
    }

    if (!flight->isBookable()) {
        Console::printError("Chuyen bay khong the dat ve (" + statusToString(flight->getStatus()) + ").");
        return;
    }

    const LinkedList<int>& freeSeats = flight->getFreeSeats();
    std::cout << Console::CYAN << "Cac ghe con trong: " << Console::WHITE;
    for (std::size_t i = 0; i < freeSeats.size(); ++i) {
        std::cout << freeSeats.at(i) << " ";
    }
    std::cout << Console::RESET << '\n';

    const int seat = Console::readInt("Nhap so ghe can dat: ");
    const Ve ticket = flight->bookSeat(seat, customer.getIdNumber(), customer.getFullName());

    store_.saveTicket(ticket);
    store_.saveFlights();

    Console::printSuccess("Xu ly dat ve thanh cong! Ma ve moi: " + ticket.getCode());
}

// ==================== Xu ly tra ve ====================

void App::processReturn() {
    Console::printHeader("XU LY TRA VE");

    const std::string flightCode = Console::readLine("Nhap ma chuyen bay can tra ve: ");
    ChuyenBay* flight = store_.findFlight(flightCode);
    if (flight == nullptr) {
        Console::printError("Khong tim thay chuyen bay: " + flightCode);
        return;
    }

    if (flight->getStatus() == ChuyenBay::Status::Completed) {
        Console::printError("Chuyen bay da hoan tat, khong the tra ve.");
        return;
    }

    const LinkedList<Ve>& tickets = flight->getTickets();
    if (tickets.isEmpty()) {
        Console::printColor(Console::YELLOW, "Chuyen bay nay chua co ve nao duoc dat.\n");
        return;
    }

    std::cout << Console::CYAN
              << std::left
              << std::setw(15) << "Ma ve"
              << std::setw(8)  << "Ghe"
              << std::setw(16) << "CMND"
              << std::setw(25) << "Ho ten"
              << Console::RESET << '\n';
    Console::printLine(64, '-');

    for (std::size_t i = 0; i < tickets.size(); ++i) {
        const Ve& ticket = tickets.at(i);
        std::cout << Console::WHITE
                  << std::left
                  << std::setw(15) << ticket.getCode()
                  << std::setw(8)  << ticket.getSeatNumber()
                  << std::setw(16) << ticket.getIdNumber()
                  << std::setw(25) << ticket.getFullName()
                  << Console::RESET << '\n';
    }

    const int seat = Console::readInt("Nhap so ghe can tra ve: ");
    const std::string ticketCode = Ve::makeCode(flightCode, seat);

    flight->releaseSeat(seat);
    store_.deleteTicket(ticketCode);
    store_.saveFlights();

    Console::printSuccess("Tra ve thanh cong! Da tra lai ghe so: " + std::to_string(seat));
}

// ==================== Menu Thong ke ====================

void App::statisticsMenu() {
    while (true) {
        const int choice = Menu::showStatistics();

        switch (choice) {
            case 1: {
                const std::string code = Console::readLine("Nhap ma chuyen bay: ");
                stats_.showPassengersOfFlight(code, store_.getFlights());
                break;
            }
            case 2: {
                const std::string code = Console::readLine("Nhap ma chuyen bay: ");
                stats_.showAvailableSeats(code, store_.getFlights());
                break;
            }
            case 3: {
                const std::string id = Console::readLine("Nhap so hieu may bay: ");
                stats_.countFlightsOfAircraft(id, store_.getFlights());
                break;
            }
            case 0:
                return;
            default:
                Console::printError("Lua chon khong hop le.");
                break;
        }
        Console::pause();
    }
}

// ==================== Vong lap chay chinh ====================

void App::run() {
    try {
        store_.load();
        auth_ = AuthService(store_.getAdmins());
    } catch (const std::invalid_argument& e) {
        Console::printError("Khong the nap du lieu: " + std::string(e.what()));
        Console::printError("Vui long kiem tra lai cac file trong thu muc data/.");
        return;
    }

    Console::clearScreen();

    while (true) {
        const int choice = Menu::showMain();

        switch (choice) {
            case 1:
                try {
                    showFlightTable();
                } catch (const std::invalid_argument& e) {
                    Console::printError(e.what());
                }
                break;
            case 2:
                try {
                    bookTicket();
                } catch (const std::invalid_argument& e) {
                    Console::printError(e.what());
                }
                break;
            case 3:
                manageLogin();
                break;
            case 0:
                Console::printColor(Console::GREEN, "\nCam on ban da su dung he thong quan ly chuyen bay. Tam biet!\n\n");
                return;
            default:
                Console::printError("Lua chon khong hop le. Vui long chon lai.");
                break;
        }
        Console::pause();
    }
}
