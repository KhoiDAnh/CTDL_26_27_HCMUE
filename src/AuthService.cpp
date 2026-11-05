#include "service/AuthService.h"
#include "core/LinkedList.h"

AuthService::AuthService(const LinkedList<Admin>& admins)
    : admins_(admins), failedAttempts_(0) {
}

bool AuthService::login(const std::string& username,
                        const std::string& password) {
    if (isLocked()) {
        return false;
    }

    for (std::size_t i = 0; i < admins_.size(); ++i) {
        if (admins_.at(i).matches(username, password)) {
            failedAttempts_ = 0;
            return true;
        }
    }

    ++failedAttempts_;
    return false;
}

bool AuthService::isLocked() const {
    return failedAttempts_ >= 3;
}

int AuthService::remainingAttempts() const {
    int remaining = 3 - failedAttempts_;

    if (remaining < 0) {
        return 0;
    }

    return remaining;
}
