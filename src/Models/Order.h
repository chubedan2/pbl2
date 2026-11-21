#pragma once

#include "OrderItem.h"
#include "../Structure/DynamicArray.h"

#include <ctime>
#include <string>

namespace restaurant {

struct OrderCodec;

enum class OrderStatus {
    Draft,
    Pending,
    InProgress,
    Completed,
    Cancelled
};

class Order {
public:
    Order(const std::string& id,
          const std::string& diningSessionId,
          std::time_t createdAt);

    const std::string& getId() const noexcept;
    const std::string& getDiningSessionId() const noexcept;
    std::time_t getCreatedAt() const noexcept;
    OrderStatus getStatus() const noexcept;
    std::size_t getItemCount() const noexcept;
    bool hasPendingItems() const noexcept;
    const OrderItem& getItemAt(std::size_t index) const;
    const DynamicArray<OrderItem>& getItems() const noexcept;

    bool addItem(OrderItem item);
    bool removeDraftItem(const std::string& orderItemId);
    bool confirm() noexcept;
    bool markItemServed(const std::string& orderItemId);
    bool cancelItem(const std::string& orderItemId);
    bool cancel() noexcept;
    bool updateItemQuantity(const std::string& orderItemId, int quantity) noexcept;
    bool updateItemNote(const std::string& orderItemId, const std::string& note);

private:
    friend struct OrderCodec;

    std::size_t findItemIndex(const std::string& orderItemId) const noexcept;
    void updateStatusFromItems() noexcept;

    std::string id_;
    std::string diningSessionId_;
    std::time_t createdAt_;
    OrderStatus status_;
    DynamicArray<OrderItem> items_;
};

}  // namespace restaurant
