#include "service/StatisticsService.h"
#include <iostream>
#include <iomanip>

void StatisticsService::showPassengersOfFlight(const std::string& strFlightCode, const LinkedList<ChuyenBay>& flightList) {
    std::cout << "\n--- DANH SACH KHACH HANG CHUYEN BAY: " << strFlightCode << " ---\n";

    bool bFound = false;
   
    for (std::size_t i = 0; i < flightList.size(); ++i) {
        
        ChuyenBay flight = flightList.at(i);

        if (flight.getFlightCode() == strFlightCode) {
            bFound = true;
            const LinkedList<Ve>& dsVe = flight.getTickets();

            if (dsVe.size() == 0) { 
                std::cout << "Chuyen bay chua co khach hang nao.\n";
                return;
            }

            std::cout << std::left << std::setw(10) << "STT"
                << std::setw(20) << "CMND"
                << std::setw(30) << "Ho Ten" << "\n";
            std::cout << "------------------------------------------------------------\n";

            for (std::size_t j = 0; j < dsVe.size(); ++j) {
                Ve ticket = dsVe.at(j); 
                std::cout << std::left << std::setw(10) << (j + 1)
                    << std::setw(20) << ticket.getIdNumber()
                    << std::setw(30) << ticket.getFullName() << "\n";
            }
            break;
        }
    }
    if (!bFound) std::cout << "Khong tim thay chuyen bay!\n";
}

void StatisticsService::showAvailableSeats(const std::string& strFlightCode, const LinkedList<ChuyenBay>& flightList) {
    std::cout << "\n--- GHE CON TRONG CHUYEN BAY: " << strFlightCode << " ---\n";

    bool bFound = false;
    for (std::size_t i = 0; i < flightList.size(); ++i) { 
        ChuyenBay flight = flightList.at(i);

        if (flight.getFlightCode() == strFlightCode) {
            bFound = true;
            const LinkedList<int>& dsGheTrong = flight.getFreeSeats();

            if (dsGheTrong.size() == 0) {
                std::cout << "Da het ghe!\n";
                return;
            }

            std::cout << "Cac ghe con trong: ";
            for (std::size_t j = 0; j < dsGheTrong.size(); ++j) { 
                std::cout << dsGheTrong.at(j) << " "; 
            }
            std::cout << "\n";
            break;
        }
    }
    if (!bFound) std::cout << "Khong tim thay chuyen bay!\n";
}

void StatisticsService::countFlightsOfAircraft(const std::string& strAircraftId, const LinkedList<ChuyenBay>& flightList) {
    int iCount = 0;
    for (std::size_t i = 0; i < flightList.size(); ++i) { 
        ChuyenBay flight = flightList.at(i);
        if (flight.getAircraftId() == strAircraftId) {
            iCount++;
        }
    }
    std::cout << "\nThong ke: May bay " << strAircraftId << " da thuc hien " << iCount << " chuyen bay.\n";
}