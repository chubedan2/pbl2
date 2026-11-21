#include "Order.h"

#include <stdexcept>
#include <utility>

namespace restaurant {

Order::Order(const std::string& id,
             const std::string& diningSessionId,
             std::time_t createdAt)
    : id_(id),
      diningSessionId_(diningSessionId),
      createdAt_(createdAt),
      status_(OrderStatus::Draft),
      items_() {
    if (id_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Order ID must not be empty");
    }
    if (diningSessionId_.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Order dining session ID must not be empty");
    }
}

const std::string& Order::getId() const noexcept {
    return id_;
}

const std::string& Order::getDiningSessionId() const noexcept {
    return diningSessionId_;
}

std::time_t Order::getCreatedAt() const noexcept {
    return createdAt_;
}

OrderStatus Order::getStatus() const noexcept {
    return status_;
}

std::size_t Order::getItemCount() const noexcept {
    return items_.size();
}

bool Order::hasPendingItems() const noexcept {
    for (std::size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].getStatus() == OrderItemStatus::Pending) {
            return true;
        }
    }
    return false;
}

const OrderItem& Order::getItemAt(std::size_t index) const {
    return items_.at(index);
}

const DynamicArray<OrderItem>& Order::getItems() const noexcept {
    return items_;
}

bool Order::addItem(OrderItem item) {
    if (status_ != OrderStatus::Draft ||
        item.getStatus() != OrderItemStatus::Pending ||
        findItemIndex(item.getId()) != DynamicArray<OrderItem>::npos) {
        return false;
    }
    items_.pushBack(std::move(item));
    return true;
}

bool Order::removeDraftItem(const std::string& orderItemId) {
    if (status_ != OrderStatus::Draft) {
        return false;
    }
    const std::size_t index = findItemIndex(orderItemId);
    return index != DynamicArray<OrderItem>::npos && items_.removeAt(index);
}

bool Order::confirm() noexcept {
    if (status_ != OrderStatus::Draft || items_.empty()) {
        return false;
    }
    status_ = OrderStatus::Pending;
    return true;
}

bool Order::markItemServed(const std::string& orderItemId) {
    if (status_ != OrderStatus::Pending && status_ != OrderStatus::InProgress) {
        return false;
    }
    const std::size_t index = findItemIndex(orderItemId);
    if (index == DynamicArray<OrderItem>::npos ||
        !items_[index].markServed()) {
        return false;
    }
    updateStatusFromItems();
    return true;
}

bool Order::cancelItem(const std::string& orderItemId) {
    if (status_ != OrderStatus::Pending && status_ != OrderStatus::InProgress) {
        return false;
    }
    const std::size_t index = findItemIndex(orderItemId);
    if (index == DynamicArray<OrderItem>::npos ||
        !items_[index].cancel()) {
        return false;
    }
    updateStatusFromItems();
    return true;
}

bool Order::cancel() noexcept {
    if (status_ != OrderStatus::Draft && status_ != OrderStatus::Pending) {
        return false;
    }
    if (status_ == OrderStatus::Draft) {
        items_.clear();
        status_ = OrderStatus::Cancelled;
        return true;
    }

    for (std::size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].getStatus() == OrderItemStatus::Served) {
            return false;
        }
    }
    for (std::size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].getStatus() == OrderItemStatus::Pending) {
            items_[i].cancel();
        }
    }
    updateStatusFromItems();
    return status_ == OrderStatus::Cancelled;
}

bool Order::updateItemQuantity(const std::string& orderItemId,
                               int quantity) noexcept {
    if (status_ != OrderStatus::Draft &&
        status_ != OrderStatus::Pending &&
        status_ != OrderStatus::InProgress) {
        return false;
    }
    const std::size_t index = findItemIndex(orderItemId);
    return index != DynamicArray<OrderItem>::npos &&
           items_[index].setQuantity(quantity);
}

bool Order::updateItemNote(const std::string& orderItemId,
                           const std::string& note) {
    if (status_ != OrderStatus::Draft &&
        status_ != OrderStatus::Pending &&
        status_ != OrderStatus::InProgress) {
        return false;
    }
    const std::size_t index = findItemIndex(orderItemId);
    if (index == DynamicArray<OrderItem>::npos ||
        items_[index].getStatus() != OrderItemStatus::Pending) {
        return false;
    }
    return items_[index].setNote(note);
}

std::size_t Order::findItemIndex(const std::string& orderItemId) const noexcept {
    for (std::size_t i = 0; i < items_.size(); ++i) {
        if (items_[i].getId() == orderItemId) {
            return i;
        }
    }
    return DynamicArray<OrderItem>::npos;
}

void Order::updateStatusFromItems() noexcept {
    std::size_t pendingCount = 0;
    std::size_t servedCount = 0;
    std::size_t cancelledCount = 0;

    for (std::size_t i = 0; i < items_.size(); ++i) {
        switch (items_[i].getStatus()) {
            case OrderItemStatus::Pending:
                ++pendingCount;
                break;
            case OrderItemStatus::Served:
                ++servedCount;
                break;
            case OrderItemStatus::Cancelled:
                ++cancelledCount;
                break;
        }
    }

    if (servedCount > 0) {
        status_ = pendingCount > 0 ? OrderStatus::InProgress : OrderStatus::Completed;
    } else if (pendingCount > 0) {
        status_ = OrderStatus::Pending;
    } else if (cancelledCount > 0) {
        status_ = OrderStatus::Cancelled;
    }
}

}  // namespace restaurant
