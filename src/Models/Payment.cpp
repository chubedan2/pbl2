#include "Payment.h"

#include <stdexcept>

namespace restaurant {

Payment::Payment(const std::string& id,
                 const std::string& billId,
                 long long amount,
                 const std::string& method,
                 std::time_t paidAt,
                 const std::string& referenceCode,
                 const std::string& note)
    : id_(id),
      billId_(billId),
      amount_(amount),
      method_(method),
      paidAt_(paidAt),
      referenceCode_(referenceCode),
      note_(note) {
    if (!isNonEmpty(id_)) {
        throw std::invalid_argument("Payment ID must not be empty");
    }
    if (!isNonEmpty(billId_)) {
        throw std::invalid_argument("Payment bill ID must not be empty");
    }
    if (amount_ <= 0) {
        throw std::invalid_argument("Payment amount must be greater than zero");
    }
    if (!isNonEmpty(method_)) {
        throw std::invalid_argument("Payment method must not be empty");
    }
}

const std::string& Payment::getId() const noexcept {
    return id_;
}

const std::string& Payment::getBillId() const noexcept {
    return billId_;
}

long long Payment::getAmount() const noexcept {
    return amount_;
}

const std::string& Payment::getMethod() const noexcept {
    return method_;
}

std::time_t Payment::getPaidAt() const noexcept {
    return paidAt_;
}

const std::string& Payment::getReferenceCode() const noexcept {
    return referenceCode_;
}

const std::string& Payment::getNote() const noexcept {
    return note_;
}

bool Payment::isNonEmpty(const std::string& value) noexcept {
    return value.find_first_not_of(" \t\n\r") != std::string::npos;
}

}  // namespace restaurant
