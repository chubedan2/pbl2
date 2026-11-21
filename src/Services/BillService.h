#pragma once

#include "../Repositories/BillRepository.h"
#include "../Repositories/DiningSessionRepository.h"
#include "../Repositories/OrderRepository.h"
#include "ServiceResult.h"

namespace restaurant {

class BillService {
public:
    BillService(BillRepository& bills,
                OrderRepository& orders,
                DiningSessionRepository& sessions);

    ServiceResult createBill(const std::string& billId,
                             const std::string& diningSessionId,
                             std::time_t createdAt);
    const DynamicArray<Bill>& getAll() const noexcept;
    const Bill* findById(const std::string& billId) const;
    const Bill* findByDiningSessionId(const std::string& diningSessionId) const;

private:
    BillRepository& bills_;
    OrderRepository& orders_;
    DiningSessionRepository& sessions_;
};

}  // namespace restaurant
