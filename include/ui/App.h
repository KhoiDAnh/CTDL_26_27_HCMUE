// App.h
// Module M2: Lop dieu khien trung tam cua ung dung (Application Controller)
#ifndef CTDL_26_27_HCMUE_APP_H
#define CTDL_26_27_HCMUE_APP_H

#include <string>

#include "storage/DataStore.h"
#include "service/AuthService.h"
#include "service/StatisticsService.h"

class App {
private:
    DataStore store_;
    AuthService auth_;
    StatisticsService stats_;

    // Chuyen doi enum trang thai sang chuoi mo ta va ma mau
    static std::string statusToString(ChuyenBay::Status status);
    static const char* statusToColor(ChuyenBay::Status status);

    // Chuc nang 1: Hien thi danh sach chuyen bay dang bang
    void showFlightTable() const;

    // Chuc nang 2: Dat ve danh cho khach hang
    void bookTicket();

    // Chuc nang 3: Dang nhap danh cho nhan vien quan ly
    void manageLogin();

    // Menu quan ly sau khi dang nhap thanh cong
    void adminMenu();
    void processBooking();
    void processReturn();
    void statisticsMenu();

public:
    explicit App(const std::string& dataDir = "data");

    // Vong lap chay chuong trinh
    void run();
};

#endif // CTDL_26_27_HCMUE_APP_H
