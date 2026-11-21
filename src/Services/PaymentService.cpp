#include "PaymentService.h"

#include <exception>
#include <string>

namespace restaurant {

PaymentService::PaymentService(PaymentRepository& payments,
                               BillRepository& bills,
                               DiningSessionRepository& sessions,
                               TableRepository& tables)
    : payments_(payments), bills_(bills), sessions_(sessions), tables_(tables) {}

ServiceResult PaymentService::payBill(const std::string& paymentId,
                                      const std::string& billId,
                                      long long amount,
                                      const std::string& method,
                                      std::time_t paidAt,
                                      const std::string& referenceCode,
                                      const std::string& note) {
    if (amount <= 0) {
        return ServiceResult::failure("Payment amount must be greater than zero.");
    }
    if (payments_.findById(paymentId) != nullptr) {
        return ServiceResult::failure("A payment with this ID already exists.");
    }

    const Bill* storedBill = bills_.findById(billId);
    if (storedBill == nullptr) {
        return ServiceResult::failure("Bill was not found.");
    }
    if (storedBill->getPaymentStatus() != BillPaymentStatus::Unpaid ||
        payments_.findByBillId(billId) != nullptr) {
        return ServiceResult::failure("Bill has already been paid or has a payment record.");
    }
    if (amount != storedBill->getTotalAmount()) {
        return ServiceResult::failure(
            "Payment amount must exactly match the bill total.");
    }

    const DiningSession* storedSession =
        sessions_.findById(storedBill->getDiningSessionId());
    if (storedSession == nullptr ||
        storedSession->getStatus() != DiningSessionStatus::WaitingForPayment) {
        return ServiceResult::failure(
            "Bill's dining session is not waiting for payment.");
    }
    const Table* storedTable = tables_.findById(storedSession->getTableId());
    if (storedTable == nullptr || storedTable->getStatus() != TableStatus::Occupied) {
        return ServiceResult::failure("Bill's table is missing or not occupied.");
    }

    try {
        Payment payment(paymentId, billId, amount, method, paidAt,
                        referenceCode, note);
        Bill updatedBill(*storedBill);
        if (!updatedBill.markPaid(payment)) {
            return ServiceResult::failure("Payment does not satisfy bill requirements.");
        }
        DiningSession updatedSession(*storedSession);
        if (!updatedSession.close(paidAt)) {
            return ServiceResult::failure("Dining session could not be closed.");
        }
        Table updatedTable(*storedTable);
        if (!updatedTable.setStatus(TableStatus::Available)) {
            return ServiceResult::failure("Dining session table could not be released.");
        }

        const Bill oldBill(*storedBill);
        const DiningSession oldSession(*storedSession);
        const Table oldTable(*storedTable);
        bool billSaved = false;
        bool sessionSaved = false;
        bool tableSaved = false;
        bool paymentSaved = false;

        try {
            if (!bills_.update(updatedBill)) {
                throw std::runtime_error("Could not save paid bill.");
            }
            billSaved = true;
            if (!sessions_.update(updatedSession)) {
                throw std::runtime_error("Could not close dining session.");
            }
            sessionSaved = true;
            if (!tables_.update(updatedTable)) {
                throw std::runtime_error("Could not release dining session table.");
            }
            tableSaved = true;
            if (!payments_.add(payment)) {
                throw std::runtime_error("Could not save payment.");
            }
            paymentSaved = true;
            return ServiceResult::success(
                "Payment recorded; bill paid, session closed, and table released.");
        } catch (const std::exception& error) {
            bool rollbackSucceeded = true;
            if (paymentSaved && !payments_.erase(paymentId)) {
                rollbackSucceeded = false;
            }
            if (tableSaved && !tables_.update(oldTable)) {
                rollbackSucceeded = false;
            }
            if (sessionSaved && !sessions_.update(oldSession)) {
                rollbackSucceeded = false;
            }
            if (billSaved && !bills_.update(oldBill)) {
                rollbackSucceeded = false;
            }
            if (!rollbackSucceeded) {
                return ServiceResult::failure(
                    std::string(error.what()) + "; one or more rollback operations failed.");
            }
            return ServiceResult::failure(error.what());
        }
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

const Payment* PaymentService::findById(const std::string& paymentId) const {
    return payments_.findById(paymentId);
}

const Payment* PaymentService::findByBillId(const std::string& billId) const {
    return payments_.findByBillId(billId);
}

}  // namespace restaurant
