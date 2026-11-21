#pragma once

#include "../Repositories/DiningSessionRepository.h"
#include "../Repositories/OrderRepository.h"
#include "../Repositories/TableRepository.h"
#include "ServiceResult.h"

namespace restaurant {

class DiningSessionService {
public:
    DiningSessionService(TableRepository& tables,
                         DiningSessionRepository& sessions,
                         OrderRepository& orders);

    ServiceResult openSession(const std::string& sessionId,
                              const std::string& tableId,
                              int guestCount,
                              std::time_t openedAt,
                              const std::string& note = std::string());
    ServiceResult cancelSession(const std::string& sessionId,
                                std::time_t cancelledAt);
    const DiningSession* findById(const std::string& id) const;
    const DynamicArray<DiningSession>& getAll() const noexcept;

private:
    TableRepository& tables_;
    DiningSessionRepository& sessions_;
    OrderRepository& orders_;
};

}  // namespace restaurant
