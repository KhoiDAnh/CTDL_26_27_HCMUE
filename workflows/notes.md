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