#pragma once

#include <string>

#include "service/BookingService.h"
#include "service/StatisticsService.h"
#include "storage/DataStore.h"

class App {
public:
    explicit App(const std::string& dataDir);

    // Returns the process exit code.
    int run();

private:
    // main menu
    void showFlightTable();
    void bookTicketScreen();
    bool login();  // false: the program must end (3 wrong tries, or ESC)

    // management menu
    void adminMenu();
    void processBookingScreen();
    void returnTicketScreen();
    void statisticsMenu();
    void passengersScreen();
    void freeSeatsScreen();
    void aircraftScreen();

    // plumbing
    void runSafely(void (App::*screen)());
    void showError(const std::string& message) const;
    void showCancelled() const;

    // drawing
    void drawBanner(const std::string& title) const;
    void drawFlightTable(bool bookableOnly) const;
    void drawFreeSeats(const ChuyenBay& flight) const;
    void drawWaitingCustomers() const;

    // input: every ask* returns false when the user pressed ESC
    bool askText(const std::string& prompt, std::string& out) const;
    bool askNumber(const std::string& prompt, int& out) const;
    bool askSeat(const ChuyenBay& flight, int& seat) const;
    bool askIdNumber(std::string& out) const;
    bool askFullName(std::string& out) const;
    bool askConfirm(const std::string& prompt) const;
    ChuyenBay* askBookableFlight();  // nullptr on ESC, throws if unusable

    DataStore store_;
    BookingService booking_;
    StatisticsService statistics_;
};