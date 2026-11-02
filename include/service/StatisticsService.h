#ifndef STATISTICS_SERVICE_H
#define STATISTICS_SERVICE_H

#include <string>
#include "core/LinkedList.h"
#include "model/ChuyenBay.h"

class StatisticsService {
public:
    /**************************************************************
     * @Description Hiển thị danh sách khách hàng của một chuyến bay
     * @param strFlightCode Mã chuyến bay cần tìm
     **************************************************************/
    void showPassengersOfFlight(const std::string& strFlightCode, const LinkedList<ChuyenBay>& flightList);

    /**************************************************************
     * @Description Hiển thị danh sách ghế còn trống của một chuyến bay
     * @param strFlightCode Mã chuyến bay cần tìm
     **************************************************************/
    void showAvailableSeats(const std::string& strFlightCode, const LinkedList<ChuyenBay>& flightList);

    /**************************************************************
     * @Description Thống kê số lượng chuyến bay của một máy bay
     * @param strAircraftId Số hiệu máy bay (VD: VN-A123)
     **************************************************************/
    void countFlightsOfAircraft(const std::string& strAircraftId, const LinkedList<ChuyenBay>& flightList);
};

#endif // STATISTICS_SERVICE_H