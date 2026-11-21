#include "OrderService.h"

#include <exception>
#include <utility>

namespace restaurant {

OrderService::OrderService(OrderRepository& orders,
                           MenuItemRepository& menuItems,
                           DiningSessionRepository& sessions)
    : orders_(orders), menuItems_(menuItems), sessions_(sessions) {}

ServiceResult OrderService::createOrder(const std::string& orderId,
                                        const std::string& diningSessionId,
                                        std::time_t createdAt) {
    const DiningSession* session = sessions_.findById(diningSessionId);
    if (session == nullptr) {
        return ServiceResult::failure("Dining session was not found.");
    }
    if (session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Orders can only be created for an active session.");
    }
    try {
        if (!orders_.add(Order(orderId, diningSessionId, createdAt))) {
            return ServiceResult::failure("An order with this ID already exists.");
        }
        return ServiceResult::success("Order created as draft.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

ServiceResult OrderService::addItem(const std::string& orderId,
                                    const std::string& orderItemId,
                                    const std::string& menuItemId,
                                    int quantity,
                                    const std::string& note) {
    const Order* storedOrder = orders_.findById(orderId);
    if (storedOrder == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    if (storedOrder->getStatus() != OrderStatus::Draft) {
        return ServiceResult::failure("Items can only be added to a draft order.");
    }
    const DiningSession* session =
        sessions_.findById(storedOrder->getDiningSessionId());
    if (session == nullptr || session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Order's dining session is not active.");
    }
    const MenuItem* menuItem = menuItems_.findById(menuItemId);
    if (menuItem == nullptr) {
        return ServiceResult::failure("Menu item was not found.");
    }
    if (quantity <= 0) {
        return ServiceResult::failure("Order item quantity must be greater than zero.");
    }

    const DynamicArray<Order>& allOrders = orders_.getAll();
    for (std::size_t i = 0; i < allOrders.size(); ++i) {
        for (std::size_t j = 0; j < allOrders[i].getItemCount(); ++j) {
            if (allOrders[i].getItemAt(j).getId() == orderItemId) {
                return ServiceResult::failure("An order item with this ID already exists.");
            }
        }
    }

    try {
        Order updated(*storedOrder);
        if (!updated.addItem(OrderItem(orderItemId, menuItem->getId(),
                                       menuItem->getName(), menuItem->getPrice(),
                                       quantity, note))) {
            return ServiceResult::failure("Order item could not be added.");
        }
        return saveUpdatedOrder(updated);
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

ServiceResult OrderService::removeDraftItem(const std::string& orderId,
                                            const std::string& orderItemId) {
    const Order* stored = orders_.findById(orderId);
    if (stored == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    Order updated(*stored);
    if (!updated.removeDraftItem(orderItemId)) {
        return ServiceResult::failure(
            "Item could not be removed; only an existing item in a draft order can be removed.");
    }
    return saveUpdatedOrder(updated);
}

ServiceResult OrderService::confirmOrder(const std::string& orderId) {
    const Order* stored = orders_.findById(orderId);
    if (stored == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    const DiningSession* session =
        sessions_.findById(stored->getDiningSessionId());
    if (session == nullptr || session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Order's dining session is not active.");
    }
    Order updated(*stored);
    if (!updated.confirm()) {
        return ServiceResult::failure(
            "Only a non-empty draft order can be confirmed.");
    }
    return saveUpdatedOrder(updated);
}

ServiceResult OrderService::markItemServed(const std::string& orderId,
                                           const std::string& orderItemId) {
    const Order* stored = orders_.findById(orderId);
    if (stored == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    const DiningSession* session =
        sessions_.findById(stored->getDiningSessionId());
    if (session == nullptr || session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Items can only be served during an active session.");
    }
    Order updated(*stored);
    if (!updated.markItemServed(orderItemId)) {
        return ServiceResult::failure(
            "Item could not be served; it must be pending in a confirmed order.");
    }
    return saveUpdatedOrder(updated);
}

ServiceResult OrderService::cancelItem(const std::string& orderId,
                                       const std::string& orderItemId) {
    const Order* stored = orders_.findById(orderId);
    if (stored == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    const DiningSession* session =
        sessions_.findById(stored->getDiningSessionId());
    if (session == nullptr || session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Items can only be cancelled during an active session.");
    }
    Order updated(*stored);
    if (!updated.cancelItem(orderItemId)) {
        return ServiceResult::failure(
            "Only an unserved item in a confirmed order can be cancelled.");
    }
    return saveUpdatedOrder(updated);
}

ServiceResult OrderService::cancelOrder(const std::string& orderId) {
    const Order* stored = orders_.findById(orderId);
    if (stored == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    const DiningSession* session =
        sessions_.findById(stored->getDiningSessionId());
    if (session == nullptr || session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Orders can only be cancelled during an active session.");
    }
    Order updated(*stored);
    if (!updated.cancel()) {
        return ServiceResult::failure(
            "Order cannot be cancelled after an item has been served or completed.");
    }
    return saveUpdatedOrder(updated);
}

ServiceResult OrderService::updateItemQuantity(const std::string& orderId,
                                               const std::string& orderItemId,
                                               int quantity) {
    const Order* stored = orders_.findById(orderId);
    if (stored == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    const DiningSession* session =
        sessions_.findById(stored->getDiningSessionId());
    if (session == nullptr || session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Items can only be edited during an active session.");
    }
    Order updated(*stored);
    if (!updated.updateItemQuantity(orderItemId, quantity)) {
        return ServiceResult::failure(
            "Quantity can only be changed to a positive value while the item is pending.");
    }
    return saveUpdatedOrder(updated);
}

ServiceResult OrderService::updateItemNote(const std::string& orderId,
                                           const std::string& orderItemId,
                                           const std::string& note) {
    const Order* stored = orders_.findById(orderId);
    if (stored == nullptr) {
        return ServiceResult::failure("Order was not found.");
    }
    const DiningSession* session =
        sessions_.findById(stored->getDiningSessionId());
    if (session == nullptr || session->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Items can only be edited during an active session.");
    }
    Order updated(*stored);
    if (!updated.updateItemNote(orderItemId, note)) {
        return ServiceResult::failure(
            "Note can only be changed while the item is pending.");
    }
    return saveUpdatedOrder(updated);
}

DynamicArray<Order> OrderService::getOrdersForSession(
    const std::string& diningSessionId) const {
    DynamicArray<Order> result;
    const DynamicArray<Order>& all = orders_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].getDiningSessionId() == diningSessionId) {
            result.pushBack(all[i]);
        }
    }
    return result;
}

DynamicArray<OrderItem> OrderService::getUndeliveredItems(
    const std::string& diningSessionId) const {
    DynamicArray<OrderItem> result;
    const DynamicArray<Order>& all = orders_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].getDiningSessionId() != diningSessionId) {
            continue;
        }
        for (std::size_t j = 0; j < all[i].getItemCount(); ++j) {
            if (all[i].getItemAt(j).getStatus() == OrderItemStatus::Pending) {
                result.pushBack(all[i].getItemAt(j));
            }
        }
    }
    return result;
}

const Order* OrderService::findById(const std::string& orderId) const {
    return orders_.findById(orderId);
}

ServiceResult OrderService::saveUpdatedOrder(const Order& order) {
    try {
        return orders_.update(order)
                   ? ServiceResult::success("Order updated.")
                   : ServiceResult::failure("Order could not be saved.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

}  // namespace restaurant
