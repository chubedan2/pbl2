#include "BillItem.h"

#include <limits>
#include <stdexcept>

namespace restaurant {

BillItem::BillItem(const std::string& id,
                   const std::string& menuItemId,
                   const std::string& itemNameSnapshot,
                   long long unitPriceSnapshot,
                   int quantity)
    : id_(id),
      menuItemId_(menuItemId),
      itemNameSnapshot_(itemNameSnapshot),
      unitPriceSnapshot_(unitPriceSnapshot),
      quantity_(quantity),
      lineTotal_(calculateLineTotal(unitPriceSnapshot, quantity)) {
    if (!isNonEmpty(id_)) {
        throw std::invalid_argument("Bill item ID must not be empty");
    }
    if (!isNonEmpty(menuItemId_)) {
        throw std::invalid_argument("Bill item menu item ID must not be empty");
    }
    if (!isNonEmpty(itemNameSnapshot_)) {
        throw std::invalid_argument("Bill item name snapshot must not be empty");
    }
}

const std::string& BillItem::getId() const noexcept {
    return id_;
}

const std::string& BillItem::getMenuItemId() const noexcept {
    return menuItemId_;
}

const std::string& BillItem::getItemNameSnapshot() const noexcept {
    return itemNameSnapshot_;
}

long long BillItem::getUnitPriceSnapshot() const noexcept {
    return unitPriceSnapshot_;
}

int BillItem::getQuantity() const noexcept {
    return quantity_;
}

long long BillItem::getLineTotal() const noexcept {
    return lineTotal_;
}

bool BillItem::isNonEmpty(const std::string& value) noexcept {
    return value.find_first_not_of(" \t\n\r") != std::string::npos;
}

long long BillItem::calculateLineTotal(long long unitPrice, int quantity) {
    if (unitPrice < 0) {
        throw std::invalid_argument("Bill item price must not be negative");
    }
    if (quantity <= 0) {
        throw std::invalid_argument("Bill item quantity must be greater than zero");
    }
    if (unitPrice > std::numeric_limits<long long>::max() / quantity) {
        throw std::overflow_error("Bill item line total exceeds supported range");
    }
    return unitPrice * quantity;
}

}  // namespace restaurant
