#pragma once

#include <string>

namespace restaurant {

struct OrderCodec;

enum class OrderItemStatus {
    Pending,
    Served,
    Cancelled
};

class OrderItem {
public:
    OrderItem(const std::string& id,
              const std::string& menuItemId,
              const std::string& itemNameSnapshot,
              long long unitPriceAtOrder,
              int quantity,
              const std::string& note = std::string());

    const std::string& getId() const noexcept;
    const std::string& getMenuItemId() const noexcept;
    const std::string& getItemNameSnapshot() const noexcept;
    long long getUnitPriceAtOrder() const noexcept;
    int getQuantity() const noexcept;
    const std::string& getNote() const noexcept;
    OrderItemStatus getStatus() const noexcept;

    bool setQuantity(int quantity) noexcept;
    bool setNote(const std::string& note);
    bool markServed() noexcept;
    bool cancel() noexcept;

private:
    friend struct OrderCodec;

    static bool isNonEmpty(const std::string& value) noexcept;

    std::string id_;
    std::string menuItemId_;
    std::string itemNameSnapshot_;
    long long unitPriceAtOrder_;
    int quantity_;
    std::string note_;
    OrderItemStatus status_;
};

}  // namespace restaurant
