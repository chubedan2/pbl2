#include "OrderRepository.h"

#include <iomanip>
#include <limits>
#include <stdexcept>
#include <utility>

namespace restaurant {

void OrderCodec::write(std::ostream& output, const Order& value) {
    output << std::quoted(value.id_) << ' '
           << std::quoted(value.diningSessionId_) << ' '
           << static_cast<long long>(value.createdAt_) << ' '
           << static_cast<int>(value.status_) << ' '
           << value.items_.size();
    for (std::size_t i = 0; i < value.items_.size(); ++i) {
        const OrderItem& item = value.items_[i];
        output << ' ' << std::quoted(item.id_)
               << ' ' << std::quoted(item.menuItemId_)
               << ' ' << std::quoted(item.itemNameSnapshot_)
               << ' ' << item.unitPriceAtOrder_
               << ' ' << item.quantity_
               << ' ' << std::quoted(item.note_)
               << ' ' << static_cast<int>(item.status_);
    }
}

Order OrderCodec::read(std::istream& input) {
    std::string id;
    std::string sessionId;
    long long createdAt = 0;
    int status = -1;
    std::size_t itemCount = 0;
    if (!(input >> std::quoted(id) >> std::quoted(sessionId) >>
          createdAt >> status >> itemCount)) {
        throw std::runtime_error("Invalid order record");
    }
    if (status < static_cast<int>(OrderStatus::Draft) ||
        status > static_cast<int>(OrderStatus::Cancelled) ||
        createdAt < std::numeric_limits<std::time_t>::min() ||
        createdAt > std::numeric_limits<std::time_t>::max()) {
        throw std::runtime_error("Invalid order data in repository file");
    }

    Order value(id, sessionId, static_cast<std::time_t>(createdAt));
    value.status_ = static_cast<OrderStatus>(status);
    std::size_t pendingCount = 0;
    std::size_t servedCount = 0;
    std::size_t cancelledCount = 0;

    for (std::size_t i = 0; i < itemCount; ++i) {
        std::string itemId;
        std::string menuItemId;
        std::string itemName;
        long long unitPrice = 0;
        int quantity = 0;
        std::string note;
        int itemStatus = -1;
        if (!(input >> std::quoted(itemId) >> std::quoted(menuItemId) >>
              std::quoted(itemName) >> unitPrice >> quantity >>
              std::quoted(note) >> itemStatus)) {
            throw std::runtime_error("Invalid order item record");
        }
        if (itemStatus < static_cast<int>(OrderItemStatus::Pending) ||
            itemStatus > static_cast<int>(OrderItemStatus::Cancelled) ||
            itemId.empty() || menuItemId.empty() || itemName.empty() ||
            unitPrice < 0 || quantity <= 0) {
            throw std::runtime_error("Invalid order item data in repository file");
        }

        OrderItem item(itemId, menuItemId, itemName, unitPrice, quantity, note);
        item.status_ = static_cast<OrderItemStatus>(itemStatus);
        for (std::size_t existing = 0; existing < value.items_.size(); ++existing) {
            if (value.items_[existing].id_ == itemId) {
                throw std::runtime_error("Duplicate order item ID in repository file");
            }
        }
        switch (item.status_) {
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
        value.items_.pushBack(std::move(item));
    }

    const bool validState =
        (value.status_ == OrderStatus::Draft && servedCount == 0 && cancelledCount == 0) ||
        (value.status_ == OrderStatus::Pending && servedCount == 0 && pendingCount > 0) ||
        (value.status_ == OrderStatus::InProgress && servedCount > 0 && pendingCount > 0) ||
        (value.status_ == OrderStatus::Completed && servedCount > 0 && pendingCount == 0) ||
        (value.status_ == OrderStatus::Cancelled && servedCount == 0 && pendingCount == 0);
    if (!validState) {
        throw std::runtime_error("Inconsistent order state in repository file");
    }
    return value;
}

OrderRepository::OrderRepository(const std::string& path)
    : FileRepository<Order>(
          path,
          [](const Order& value) { return value.getId(); },
          OrderCodec::write,
          OrderCodec::read) {}

}  // namespace restaurant
