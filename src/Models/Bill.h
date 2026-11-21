#pragma once

#include "BillItem.h"
#include "Payment.h"
#include "../Structure/DynamicArray.h"

#include <ctime>
#include <string>

namespace restaurant {

struct BillCodec;

enum class BillPaymentStatus {
    Unpaid,
    Paid
};

class Bill {
public:
    Bill(const std::string& id,
         const std::string& diningSessionId,
         std::time_t createdAt,
         DynamicArray<BillItem> items);

    const std::string& getId() const noexcept;
    const std::string& getDiningSessionId() const noexcept;
    std::time_t getCreatedAt() const noexcept;
    std::size_t getItemCount() const noexcept;
    const BillItem& getItemAt(std::size_t index) const;
    const DynamicArray<BillItem>& getItems() const noexcept;
    long long getSubtotal() const noexcept;
    long long getTotalAmount() const noexcept;
    BillPaymentStatus getPaymentStatus() const noexcept;

    bool markPaid(const Payment& payment) noexcept;

private:
    friend struct BillCodec;

    static bool isNonEmpty(const std::string& value) noexcept;
    static long long calculateSubtotal(const DynamicArray<BillItem>& items);

    std::string id_;
    std::string diningSessionId_;
    std::time_t createdAt_;
    DynamicArray<BillItem> items_;
    long long subtotal_;
    long long totalAmount_;
    BillPaymentStatus paymentStatus_;
};

}  // namespace restaurant
