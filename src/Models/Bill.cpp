#include "Bill.h"

#include <limits>
#include <stdexcept>
#include <utility>

namespace restaurant {

Bill::Bill(const std::string& id,
           const std::string& diningSessionId,
           std::time_t createdAt,
           DynamicArray<BillItem> items)
    : id_(id),
      diningSessionId_(diningSessionId),
      createdAt_(createdAt),
      items_(std::move(items)),
      subtotal_(calculateSubtotal(items_)),
      totalAmount_(subtotal_),
      paymentStatus_(BillPaymentStatus::Unpaid) {
    if (!isNonEmpty(id_)) {
        throw std::invalid_argument("Bill ID must not be empty");
    }
    if (!isNonEmpty(diningSessionId_)) {
        throw std::invalid_argument("Bill dining session ID must not be empty");
    }
    if (items_.empty()) {
        throw std::invalid_argument("Bill must contain at least one item");
    }
    if (totalAmount_ <= 0) {
        throw std::invalid_argument("Bill total amount must be greater than zero");
    }
}

const std::string& Bill::getId() const noexcept {
    return id_;
}

const std::string& Bill::getDiningSessionId() const noexcept {
    return diningSessionId_;
}

std::time_t Bill::getCreatedAt() const noexcept {
    return createdAt_;
}

std::size_t Bill::getItemCount() const noexcept {
    return items_.size();
}

const BillItem& Bill::getItemAt(std::size_t index) const {
    return items_.at(index);
}

const DynamicArray<BillItem>& Bill::getItems() const noexcept {
    return items_;
}

long long Bill::getSubtotal() const noexcept {
    return subtotal_;
}

long long Bill::getTotalAmount() const noexcept {
    return totalAmount_;
}

BillPaymentStatus Bill::getPaymentStatus() const noexcept {
    return paymentStatus_;
}

bool Bill::markPaid(const Payment& payment) noexcept {
    if (paymentStatus_ != BillPaymentStatus::Unpaid ||
        payment.getBillId() != id_ ||
        payment.getAmount() != totalAmount_) {
        return false;
    }
    paymentStatus_ = BillPaymentStatus::Paid;
    return true;
}

bool Bill::isNonEmpty(const std::string& value) noexcept {
    return value.find_first_not_of(" \t\n\r") != std::string::npos;
}

long long Bill::calculateSubtotal(const DynamicArray<BillItem>& items) {
    long long sum = 0;
    for (std::size_t i = 0; i < items.size(); ++i) {
        const long long lineTotal = items[i].getLineTotal();
        if (lineTotal > std::numeric_limits<long long>::max() - sum) {
            throw std::overflow_error("Bill subtotal exceeds supported range");
        }
        sum += lineTotal;
    }
    return sum;
}

}  // namespace restaurant
