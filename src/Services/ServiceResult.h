#pragma once

#include <string>

namespace restaurant {

struct ServiceResult {
    bool succeeded;
    std::string message;

    static ServiceResult success(const std::string& message = std::string()) {
        return ServiceResult{true, message};
    }

    static ServiceResult failure(const std::string& message) {
        return ServiceResult{false, message};
    }
};

}  // namespace restaurant
