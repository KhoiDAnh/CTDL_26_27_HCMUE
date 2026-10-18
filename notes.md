| Requirement from the PDF                                                         | Lives in                                           |
|:---------------------------------------------------------------------------------|:---------------------------------------------------|
| Show flights as a table                                                          | App + Menu, data from FlightService                |
| Book ticket, with no blank or whitespace input                                   | BookingService::book + input validation in Console |
| Booking only if seats are free and the flight is not complete                    | Flight::isBookable()                               |
| Login with *, 3 tries, ESC                                                       | AuthService + Console::readHidden                  |
| Process booking (remove waiting customer, create ticket, remove seat, save file) | BookingService::processNextWaiting                 |
| Return ticket (flight not complete, ticket removed, seat returned)               | BookingService::cancel                             |
| Stats: passengers of a flight, free seats, flights per aircraft                  | StatisticsService                                  |
| Errors cancel the action and return to the menu                                  | Exceptions caught in App, never exit()             |
| Colors                                                                           | Console only                                       |

# KHACH HANG test case
OK: 1|079203001234|Nguyen Van A\
THROWS: 1|079203001234|Nguyen|Van A -> KhachHang: Du lieu nhap vao phai co dung 3 vung.\
THROWS: 1|07920300123|Nguyen Van A -> KhachHang: CMND phai chinh xac 12 chu so.\
THROWS: 1|07920300123A|Nguyen Van A -> KhachHang: CMND phai chinh xac 12 chu so.\
THROWS: 0|079203001234|Nguyen Van A -> KhachHang: STT don hang phai it nhat la 1.\
THROWS: x|079203001234|Nguyen Van A -> Khong phai la so nguyen hop le: 'x'\
THROWS: 1|079203001234|    -> KhachHang: Ten day du khong duoc trong.\
THROWS: 1|079203001234 -> KhachHang: Du lieu nhap vao phai co dung 3 vung.\