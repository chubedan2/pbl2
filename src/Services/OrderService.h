#pragma once

#include "../Repositories/DiningSessionRepository.h"
#include "../Repositories/MenuItemRepository.h"
#include "../Repositories/OrderRepository.h"
#include "ServiceResult.h"

namespace restaurant {

class OrderService {
public:
    OrderService(OrderRepository& orders,
                 MenuItemRepository& menuItems,
                 DiningSessionRepository& sessions);

    ServiceResult createOrder(const std::string& orderId,
                              const std::string& diningSessionId,
                              std::time_t createdAt);
    ServiceResult addItem(const std::string& orderId,
                          const std::string& orderItemId,
                          const std::string& menuItemId,
                          int quantity,
                          const std::string& note = std::string());
    ServiceResult removeDraftItem(const std::string& orderId,
                                   const std::string& orderItemId);
    ServiceResult confirmOrder(const std::string& orderId);
    ServiceResult markItemServed(const std::string& orderId,
                                 const std::string& orderItemId);
    ServiceResult cancelItem(const std::string& orderId,
                             const std::string& orderItemId);
    ServiceResult cancelOrder(const std::string& orderId);
    ServiceResult updateItemQuantity(const std::string& orderId,
                                     const std::string& orderItemId,
                                     int quantity);
    ServiceResult updateItemNote(const std::string& orderId,
                                 const std::string& orderItemId,
                                 const std::string& note);

    DynamicArray<Order> getOrdersForSession(const std::string& diningSessionId) const;
    DynamicArray<OrderItem> getUndeliveredItems(
        const std::string& diningSessionId) const;
    const Order* findById(const std::string& orderId) const;

private:
    ServiceResult saveUpdatedOrder(const Order& order);

    OrderRepository& orders_;
    MenuItemRepository& menuItems_;
    DiningSessionRepository& sessions_;
};

}  // namespace restaurant
