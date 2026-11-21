#pragma once

#include "../Repositories/BillRepository.h"
#include "../Repositories/DiningSessionRepository.h"
#include "../Repositories/PaymentRepository.h"
#include "../Repositories/TableRepository.h"
#include "ServiceResult.h"

namespace restaurant {

class PaymentService {
public:
    PaymentService(PaymentRepository& payments,
                   BillRepository& bills,
                   DiningSessionRepository& sessions,
                   TableRepository& tables);

    ServiceResult payBill(const std::string& paymentId,
                          const std::string& billId,
                          long long amount,
                          const std::string& method,
                          std::time_t paidAt,
                          const std::string& referenceCode = std::string(),
                          const std::string& note = std::string());

    const Payment* findById(const std::string& paymentId) const;
    const Payment* findByBillId(const std::string& billId) const;

private:
    PaymentRepository& payments_;
    BillRepository& bills_;
    DiningSessionRepository& sessions_;
    TableRepository& tables_;
};

}  // namespace restaurant
