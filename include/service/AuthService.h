
#ifndef CTDL_26_27_HCMUE_AUTHSERVICE_H
#define CTDL_26_27_HCMUE_AUTHSERVICE_H

#include <string>

#include "model/Admin.h"
#include "core/LinkedList.h"
#include "storage/FileRepository.h"

class AuthService {
private:
    LinkedList<Admin> admins_;

public:
    AuthService() {
        admins_ = FileRepository<Admin>::loadAll(
            "data/Admin.txt",
            Admin::fromLine
        );
    }

    bool login(const std::string& username,
               const std::string& password) const {
        for (std::size_t i = 0; i < admins_.size(); ++i) {
            if (admins_.at(i).matches(username, password)) {
                return true;
            }
        }

        return false;
    }

    bool authenticate(int& attempts,
                      const std::string& username,
                      const std::string& password) const {
        if (attempts >= 3) {
            return false;
        }

        if (login(username, password)) {
            return true;
        }

        ++attempts;
        return false;
    }

    bool isLocked(int attempts) const {
        return attempts >= 3;
    }
};

#endif
