#include "OrderItem.h"

#include <stdexcept>

namespace restaurant {

OrderItem::OrderItem(const std::string& id,
                     const std::string& menuItemId,
                     const std::string& itemNameSnapshot,
                     long long unitPriceAtOrder,
                     int quantity,
                     const std::string& note)
    : id_(id),
      menuItemId_(menuItemId),
      itemNameSnapshot_(itemNameSnapshot),
      unitPriceAtOrder_(unitPriceAtOrder),
      quantity_(quantity),
      note_(note),
      status_(OrderItemStatus::Pending) {
    if (!isNonEmpty(id_)) {
        throw std::invalid_argument("Order item ID must not be empty");
    }
    if (!isNonEmpty(menuItemId_)) {
        throw std::invalid_argument("Order item menu item ID must not be empty");
    }
    if (!isNonEmpty(itemNameSnapshot_)) {
        throw std::invalid_argument("Order item name snapshot must not be empty");
    }
    if (unitPriceAtOrder_ < 0) {
        throw std::invalid_argument("Order item price must not be negative");
    }
    if (quantity_ <= 0) {
        throw std::invalid_argument("Order item quantity must be greater than zero");
    }
}

const std::string& OrderItem::getId() const noexcept {
    return id_;
}

const std::string& OrderItem::getMenuItemId() const noexcept {
    return menuItemId_;
}

const std::string& OrderItem::getItemNameSnapshot() const noexcept {
    return itemNameSnapshot_;
}

long long OrderItem::getUnitPriceAtOrder() const noexcept {
    return unitPriceAtOrder_;
}

int OrderItem::getQuantity() const noexcept {
    return quantity_;
}

const std::string& OrderItem::getNote() const noexcept {
    return note_;
}

OrderItemStatus OrderItem::getStatus() const noexcept {
    return status_;
}

bool OrderItem::setQuantity(int quantity) noexcept {
    if (quantity <= 0 || status_ != OrderItemStatus::Pending) {
        return false;
    }
    quantity_ = quantity;
    return true;
}

bool OrderItem::setNote(const std::string& note) {
    if (status_ != OrderItemStatus::Pending) {
        return false;
    }
    note_ = note;
    return true;
}

bool OrderItem::markServed() noexcept {
    if (status_ != OrderItemStatus::Pending) {
        return false;
    }
    status_ = OrderItemStatus::Served;
    return true;
}

bool OrderItem::cancel() noexcept {
    if (status_ != OrderItemStatus::Pending) {
        return false;
    }
    status_ = OrderItemStatus::Cancelled;
    return true;
}

bool OrderItem::isNonEmpty(const std::string& value) noexcept {
    return value.find_first_not_of(" \t\n\r") != std::string::npos;
}

}  // namespace restaurant
