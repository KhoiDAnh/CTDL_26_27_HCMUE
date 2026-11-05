#pragma once

#include <string>

#include "model/Admin.h"
#include "core/LinkedList.h"

class AuthService {
private:
    LinkedList<Admin> admins_;
    int failedAttempts_;

public:
    explicit AuthService(const LinkedList<Admin>& admins);

    bool login(const std::string& username,
               const std::string& password);

    bool isLocked() const;

    int remainingAttempts() const;
};
