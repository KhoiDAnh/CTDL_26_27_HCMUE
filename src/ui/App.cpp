#include "ui/App.h"

#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "core/StringUtils.h"
#include "model/KhachHang.h"
#include "service/AuthService.h"
#include "ui/Console.h"
#include "ui/Format.h"
#include "ui/Menu.h"

namespace {

typedef Console::Color Color;

const std::size_t kRuleWidth = 88;
const std::size_t kMaxWaitingShown = 10;

Color colorFor(ChuyenBay::Status status) {
    switch (status) {
        case ChuyenBay::Status::Available:
            return Color::Green;
        case ChuyenBay::Status::SoldOut:
            return Color::Yellow;
        case ChuyenBay::Status::Cancelled:
            return Color::Red;
        case ChuyenBay::Status::Completed:
            return Color::Gray;
    }
    return Color::White;
}

bool hasBookableFlight(const LinkedList<ChuyenBay>& flights) {
    for (std::size_t i = 0; i < flights.size(); ++i) {
        if (flights.at(i).isBookable()) {
            return true;
        }
    }
    return false;
}

bool isSeatFree(const ChuyenBay& flight, int seat) {
    const LinkedList<int>& seats = flight.getFreeSeats();
    for (std::size_t i = 0; i < seats.size(); ++i) {
        if (seats.at(i) == seat) {
            return true;
        }
    }
    return false;
}

void goodbye() {
    Console::println();
    Console::println(" Cam on ban da su dung chuong trinh!", Color::Cyan);
}

}  // namespace

App::App(const std::string& dataDir) : store_(dataDir), booking_(store_) {}

int App::run() {
    try {
        store_.load();
        std::cerr << "[debug] data loaded\n";
    } catch (const std::exception& e) {
        Console::println(" Khong the doc du lieu:", Color::Red);
        Console::println(std::string(" ") + e.what(), Color::Red);
        Console::waitForKey(" Nhan phim bat ky de thoat...");
        return 1;
    }

    Menu menu("CHUONG TRINH QUAN LY CHUYEN BAY");
    menu.addItem(1, "Hien thi danh sach cac chuyen bay");
    menu.addItem(2, "Dat ve");
    menu.addItem(3, "Quan ly (can dang nhap)");
    menu.addItem(0, "Thoat chuong trinh");

    while (true) {
        switch (menu.show()) {
            case 1:
                runSafely(&App::showFlightTable);
                break;
            case 2:
                runSafely(&App::bookTicketScreen);
                break;
            case 3:
                if (!login()) {
                    goodbye();
                    return 0;
                }
                adminMenu();
                break;
            default:
                goodbye();
                return 0;
        }
    }
}

// ---------------------------------------------------------------- plumbing

void App::runSafely(void (App::*screen)()) {
    try {
        (this->*screen)();
    } catch (const std::exception& e) {
        showError(e.what());
    }
}

void App::showError(const std::string& message) const {
    Console::println();
    Console::println(" LOI: " + message, Color::Red);
    Console::waitForKey();
}

void App::showCancelled() const {
    Console::println();
    Console::println(" Da huy thao tac.", Color::Yellow);
    Console::waitForKey();
}

// ----------------------------------------------------------------- drawing

void App::drawBanner(const std::string& title) const {
    Console::clear();
    Console::println(std::string(kRuleWidth, '='), Color::Cyan);
    Console::println(" " + title, Color::Cyan);
    Console::println(std::string(kRuleWidth, '='), Color::Cyan);
}

void App::drawFlightTable(bool bookableOnly) const {
    const LinkedList<ChuyenBay>& flights = store_.getFlights();

    Console::println(Format::fit(" STT", 6) + Format::fit("Ma CB", 9) +
                         Format::fit("Ngay khoi hanh", 16) + Format::fit("San bay den", 15) +
                         Format::fit("Trang thai", 12) + Format::fit("Ve da ban", 10) +
                         "Danh sach ve",
                     Color::Yellow);
    Console::println(std::string(kRuleWidth, '-'), Color::Gray);

    std::size_t shown = 0;
    for (std::size_t i = 0; i < flights.size(); ++i) {
        const ChuyenBay& flight = flights.at(i);
        if (bookableOnly && !flight.isBookable()) {
            continue;
        }
        ++shown;

        const LinkedList<Ve>& tickets = flight.getTickets();
        std::string codes;
        for (std::size_t j = 0; j < tickets.size(); ++j) {
            if (j > 0) {
                codes += ", ";
            }
            codes += tickets.at(j).getCode();
        }
        const std::string sold =
            std::to_string(tickets.size()) + "/" + std::to_string(flight.getSeatCount());

        Console::println(
            Format::fit(" " + std::to_string(shown), 6) +
                Format::fit(flight.getFlightCode(), 9) +
                Format::fit(flight.getDepartureDate().toString(), 16) +
                Format::fit(flight.getDestination(), 15) +
                Format::fit(Format::statusText(flight.getStatus()), 12) +
                Format::fit(sold, 10) + Format::fit(codes.empty() ? "-" : codes, 22),
            colorFor(flight.getStatus()));
    }
    if (shown == 0) {
        Console::println(" (khong co chuyen bay nao)", Color::Red);
    }
}

void App::drawFreeSeats(const ChuyenBay& flight) const {
    const LinkedList<int>& seats = flight.getFreeSeats();
    Console::println(" Ghe con trong (" + std::to_string(seats.size()) + "/" +
                         std::to_string(flight.getSeatCount()) + "):",
                     Color::Green);
    std::string row;
    for (std::size_t i = 0; i < seats.size(); ++i) {
        const std::string number = std::to_string(seats.at(i));
        row += std::string(number.size() < 5 ? 5 - number.size() : 0, ' ') + number;
        if ((i + 1) % 10 == 0) {
            Console::println(row);
            row.clear();
        }
    }
    if (!row.empty()) {
        Console::println(row);
    }
}

void App::drawWaitingCustomers() const {
    const Queue<KhachHang>& waiting = store_.getCustomers();
    Console::println(" Khach hang dang cho mua ve: " + std::to_string(waiting.size()) +
                         " (xu ly theo thu tu, nguoi dau tien truoc)",
                     Color::Yellow);
    Console::println(Format::fit(" STT", 7) + Format::fit("CMND", 16) + "Ho ten", Color::Yellow);
    Console::println(std::string(kRuleWidth, '-'), Color::Gray);

    for (std::size_t i = 0; i < waiting.size() && i < kMaxWaitingShown; ++i) {
        const KhachHang& customer = waiting.at(i);
        Console::println(Format::fit(" " + std::to_string(customer.getOrder()), 7) +
                         Format::fit(customer.getIdNumber(), 16) + customer.getFullName());
    }
    if (waiting.size() > kMaxWaitingShown) {
        Console::println(" ... va " + std::to_string(waiting.size() - kMaxWaitingShown) +
                             " khach hang khac",
                         Color::Gray);
    }
}

// ------------------------------------------------------------------- input

bool App::askText(const std::string& prompt, std::string& out) const {
    while (true) {
        std::string raw;
        if (!Console::readLine(prompt, raw)) {
            return false;
        }
        const std::string text = Format::trim(raw);
        if (!text.empty()) {
            out = text;
            return true;
        }
        Console::println(" Khong duoc de trong hoac chi nhap khoang trang.", Color::Red);
    }
}

bool App::askNumber(const std::string& prompt, int& out) const {
    while (true) {
        std::string text;
        if (!askText(prompt, text)) {
            return false;
        }
        try {
            out = StringUtils::parseInt(text);
            return true;
        } catch (const std::invalid_argument&) {
            Console::println(" Vui long nhap mot so nguyen.", Color::Red);
        }
    }
}

bool App::askSeat(const ChuyenBay& flight, int& seat) const {
    while (true) {
        int value = 0;
        if (!askNumber(" So ghe: ", value)) {
            return false;
        }
        if (value < 1 || value > flight.getSeatCount()) {
            Console::println(" So ghe phai tu 1 den " + std::to_string(flight.getSeatCount()) + ".",
                             Color::Red);
        } else if (!isSeatFree(flight, value)) {
            Console::println(" Ghe nay da co nguoi dat. Hay chon ghe khac.", Color::Red);
        } else {
            seat = value;
            return true;
        }
    }
}

bool App::askIdNumber(std::string& out) const {
    while (true) {
        std::string text;
        if (!askText(" CMND (12 chu so): ", text)) {
            return false;
        }
        try {
            KhachHang::validateIDNumber(text);
            out = text;
            return true;
        } catch (const std::invalid_argument& e) {
            Console::println(std::string(" ") + e.what(), Color::Red);
        }
    }
}

bool App::askFullName(std::string& out) const {
    while (true) {
        std::string text;
        if (!askText(" Ho ten: ", text)) {
            return false;
        }
        try {
            KhachHang::validateFullName(text);
            out = text;
            return true;
        } catch (const std::invalid_argument& e) {
            Console::println(std::string(" ") + e.what(), Color::Red);
        }
    }
}

bool App::askConfirm(const std::string& prompt) const {
    std::string answer;
    if (!Console::readLine(prompt + " (y/n): ", answer)) {
        return false;
    }
    const std::string text = Format::trim(answer);
    return text == "y" || text == "Y";
}

ChuyenBay* App::askBookableFlight() {
    std::string code;
    if (!askText(" Ma chuyen bay: ", code)) {
        return nullptr;
    }
    ChuyenBay* flight = store_.findFlight(code);
    if (flight == nullptr) {
        throw std::invalid_argument("Khong tim thay chuyen bay: " + code);
    }
    if (!flight->isBookable()) {
        throw std::invalid_argument("Chuyen bay " + code + " khong the dat ve (" +
                                    Format::statusText(flight->getStatus()) + ").");
    }
    return flight;
}

// ------------------------------------------------------------ main screens

void App::showFlightTable() {
    drawBanner("DANH SACH CAC CHUYEN BAY");
    drawFlightTable(false);
    Console::println();
    Console::waitForKey();
}

void App::bookTicketScreen() {
    drawBanner("DAT VE");
    if (!hasBookableFlight(store_.getFlights())) {
        throw std::invalid_argument("Hien khong co chuyen bay nao con ve.");
    }
    drawFlightTable(true);
    Console::println();

    ChuyenBay* flight = askBookableFlight();
    if (flight == nullptr) {
        showCancelled();
        return;
    }
    Console::println();
    drawFreeSeats(*flight);
    Console::println();

    int seat = 0;
    std::string idNumber;
    std::string fullName;
    if (!askSeat(*flight, seat) || !askIdNumber(idNumber) || !askFullName(fullName)) {
        showCancelled();
        return;
    }

    Console::println();
    Console::println(" Chuyen bay: " + flight->getFlightCode() + "   Ghe: " + std::to_string(seat));
    Console::println(" CMND: " + idNumber + "   Ho ten: " + fullName);
    if (!askConfirm(" Xac nhan dat ve")) {
        showCancelled();
        return;
    }

    const Ve ticket = booking_.bookDirect(flight->getFlightCode(), seat, idNumber, fullName);
    Console::println();
    Console::println(" Dat ve thanh cong! Ma ve: " + ticket.getCode(), Color::Green);
    Console::waitForKey();
}

bool App::login() {
    AuthService auth(store_.getAdmins());

    const std::string frame = "  * * * * * * * * * * * * * * * * * * * * *";
    Console::clear();
    Console::println(frame, Color::Yellow);
    Console::print("  *", Color::Yellow);
    Console::print(std::string(10, ' ') + "DANG NHAP HE THONG" + std::string(11, ' '),
                   Color::Green);
    Console::println("*", Color::Yellow);
    Console::println(frame, Color::Yellow);
    Console::println("  (Nhan ESC de thoat)", Color::Gray);
    Console::println();

    while (!auth.isLocked()) {
        std::string user;
        std::string password;
        if (!Console::readLine("    User: ", user) ||
            !Console::readHidden("    Password: ", password)) {
            Console::println();
            Console::println("  Thoat khoi he thong.", Color::Yellow);
            Console::waitForKey();
            return false;
        }
        if (auth.login(Format::trim(user), password)) {
            Console::println();
            Console::println("  Dang nhap thanh cong!", Color::Green);
            Console::waitForKey();
            return true;
        }
        if (!auth.isLocked()) {
            Console::println("  Sai user hoac password. Con " +
                                 std::to_string(auth.remainingAttempts()) + " lan thu.",
                             Color::Red);
            Console::println();
        }
    }

    Console::println("  Nhap sai 3 lan. Thoat khoi he thong.", Color::Red);
    Console::waitForKey();
    return false;
}

// --------------------------------------------------------- management menu

void App::adminMenu() {
    Menu menu("MENU QUAN LY");
    menu.addItem(1, "Xu ly dat ve");
    menu.addItem(2, "Xu ly tra ve");
    menu.addItem(3, "Thong ke");
    menu.addItem(0, "Dang xuat (quay lai)");

    while (true) {
        switch (menu.show()) {
            case 1:
                runSafely(&App::processBookingScreen);
                break;
            case 2:
                runSafely(&App::returnTicketScreen);
                break;
            case 3:
                statisticsMenu();
                break;
            default:
                return;
        }
    }
}

void App::processBookingScreen() {
    drawBanner("XU LY DAT VE");
    if (store_.getCustomers().isEmpty()) {
        throw std::invalid_argument("Khong co khach hang nao dang cho mua ve.");
    }
    if (!hasBookableFlight(store_.getFlights())) {
        throw std::invalid_argument("Hien khong co chuyen bay nao con ve.");
    }
    drawWaitingCustomers();
    Console::println();
    drawFlightTable(true);
    Console::println();

    ChuyenBay* flight = askBookableFlight();
    if (flight == nullptr) {
        showCancelled();
        return;
    }
    Console::println();
    drawFreeSeats(*flight);
    Console::println();

    int seat = 0;
    if (!askSeat(*flight, seat)) {
        showCancelled();
        return;
    }

    const KhachHang& next = store_.getCustomers().front();
    Console::println();
    if (!askConfirm(" Xu ly dat ve cho khach hang STT " + std::to_string(next.getOrder()) +
                    " (" + next.getFullName() + "), chuyen bay " + flight->getFlightCode() +
                    ", ghe " + std::to_string(seat))) {
        showCancelled();
        return;
    }

    const Ve ticket = booking_.bookNextWaiting(flight->getFlightCode(), seat);
    Console::println();
    Console::println(" Xu ly thanh cong! Ma ve: " + ticket.getCode(), Color::Green);
    Console::println(" Con " + std::to_string(store_.getCustomers().size()) +
                     " khach hang dang cho.");
    Console::waitForKey();
}

void App::returnTicketScreen() {
    drawBanner("XU LY TRA VE");
    drawFlightTable(false);
    Console::println();

    std::string code;
    if (!askText(" Ma chuyen bay: ", code)) {
        showCancelled();
        return;
    }
    const ChuyenBay* flight = store_.findFlight(code);
    if (flight == nullptr) {
        throw std::invalid_argument("Khong tim thay chuyen bay: " + code);
    }
    if (flight->getStatus() == ChuyenBay::Status::Completed) {
        throw std::invalid_argument("Chuyen bay da hoan tat, khong the tra ve.");
    }
    if (flight->getTickets().isEmpty()) {
        throw std::invalid_argument("Chuyen bay chua co ve nao.");
    }

    statistics_.showPassengersOfFlight(code, store_.getFlights());
    Console::println();

    int seat = 0;
    if (!askNumber(" So ghe cua ve can tra: ", seat)) {
        showCancelled();
        return;
    }
    if (!askConfirm(" Xac nhan tra ve ghe " + std::to_string(seat) + " cua chuyen bay " + code)) {
        showCancelled();
        return;
    }

    booking_.returnTicket(code, seat);
    Console::println();
    Console::println(" Tra ve thanh cong. Ghe " + std::to_string(seat) +
                         " da tro lai danh sach ghe trong.",
                     Color::Green);
    Console::waitForKey();
}

// -------------------------------------------------------------- statistics

void App::statisticsMenu() {
    Menu menu("THONG KE");
    menu.addItem(1, "Danh sach khach cua mot chuyen bay");
    menu.addItem(2, "Danh sach ghe con trong cua mot chuyen bay");
    menu.addItem(3, "So chuyen bay cua mot may bay");
    menu.addItem(0, "Quay lai");

    while (true) {
        switch (menu.show()) {
            case 1:
                runSafely(&App::passengersScreen);
                break;
            case 2:
                runSafely(&App::freeSeatsScreen);
                break;
            case 3:
                runSafely(&App::aircraftScreen);
                break;
            default:
                return;
        }
    }
}

void App::passengersScreen() {
    drawBanner("DANH SACH KHACH CUA MOT CHUYEN BAY");
    drawFlightTable(false);
    Console::println();
    std::string code;
    if (!askText(" Ma chuyen bay: ", code)) {
        showCancelled();
        return;
    }
    statistics_.showPassengersOfFlight(code, store_.getFlights());
    Console::println();
    Console::waitForKey();
}

void App::freeSeatsScreen() {
    drawBanner("DANH SACH GHE CON TRONG");
    drawFlightTable(false);
    Console::println();
    std::string code;
    if (!askText(" Ma chuyen bay: ", code)) {
        showCancelled();
        return;
    }
    statistics_.showAvailableSeats(code, store_.getFlights());
    Console::println();
    Console::waitForKey();
}

void App::aircraftScreen() {
    drawBanner("SO CHUYEN BAY CUA MOT MAY BAY");
    const LinkedList<MayBay>& aircraft = store_.getAircraft();
    Console::println(Format::fit(" So hieu may bay", 20) + "So cho", Color::Yellow);
    for (std::size_t i = 0; i < aircraft.size(); ++i) {
        Console::println(Format::fit(" " + aircraft.at(i).getId(), 20) +
                         std::to_string(aircraft.at(i).getSeatCount()));
    }
    Console::println();

    std::string id;
    if (!askText(" So hieu may bay: ", id)) {
        showCancelled();
        return;
    }
    if (store_.findAircraft(id) == nullptr) {
        throw std::invalid_argument("Khong tim thay may bay: " + id);
    }
    statistics_.countFlightsOfAircraft(id, store_.getFlights());
    Console::println();
    Console::waitForKey();
}