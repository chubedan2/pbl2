#include "BillService.h"

#include <exception>
#include <string>
#include <utility>

namespace restaurant {

BillService::BillService(BillRepository& bills,
                         OrderRepository& orders,
                         DiningSessionRepository& sessions)
    : bills_(bills), orders_(orders), sessions_(sessions) {}

ServiceResult BillService::createBill(const std::string& billId,
                                      const std::string& diningSessionId,
                                      std::time_t createdAt) {
    const DiningSession* storedSession = sessions_.findById(diningSessionId);
    if (storedSession == nullptr) {
        return ServiceResult::failure("Dining session was not found.");
    }
    if (storedSession->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("A bill can only be created for an active session.");
    }
    if (bills_.findById(billId) != nullptr) {
        return ServiceResult::failure("A bill with this ID already exists.");
    }
    if (bills_.findByDiningSessionId(diningSessionId) != nullptr) {
        return ServiceResult::failure("This dining session already has a bill.");
    }

    DynamicArray<BillItem> items;
    bool hasServedItem = false;
    const DynamicArray<Order>& orders = orders_.getAll();
    for (std::size_t i = 0; i < orders.size(); ++i) {
        if (orders[i].getDiningSessionId() != diningSessionId) {
            continue;
        }
        for (std::size_t j = 0; j < orders[i].getItemCount(); ++j) {
            const OrderItem& orderItem = orders[i].getItemAt(j);
            if (orderItem.getStatus() == OrderItemStatus::Pending) {
                return ServiceResult::failure(
                    "Resolve all pending order items before creating a bill.");
            }
            if (orderItem.getStatus() != OrderItemStatus::Served) {
                continue;
            }
            hasServedItem = true;
            const std::string itemId =
                billId + "_item_" + std::to_string(items.size() + 1);
            try {
                items.pushBack(BillItem(itemId, orderItem.getMenuItemId(),
                                        orderItem.getItemNameSnapshot(),
                                        orderItem.getUnitPriceAtOrder(),
                                        orderItem.getQuantity()));
            } catch (const std::exception& error) {
                return ServiceResult::failure(error.what());
            }
        }
    }
    if (!hasServedItem) {
        return ServiceResult::failure(
            "A bill requires at least one served item; cancel the session if none were served.");
    }

    try {
        Bill bill(billId, diningSessionId, createdAt, std::move(items));
        if (bill.getTotalAmount() <= 0) {
            return ServiceResult::failure("A zero-value bill cannot be created.");
        }
        DiningSession updatedSession(*storedSession);
        if (!updatedSession.startWaitingForPayment()) {
            return ServiceResult::failure(
                "Dining session cannot enter the payment process.");
        }
        if (!bills_.add(bill)) {
            return ServiceResult::failure("Bill could not be saved.");
        }
        try {
            if (!sessions_.update(updatedSession)) {
                throw std::runtime_error("Dining session could not be updated.");
            }
        } catch (const std::exception& error) {
            try {
                if (!bills_.erase(billId)) {
                    return ServiceResult::failure(
                        std::string(error.what()) +
                        "; rollback could not remove the bill.");
                }
            } catch (const std::exception& rollbackError) {
                return ServiceResult::failure(
                    std::string(error.what()) + "; rollback failed: " +
                    rollbackError.what());
            }
            return ServiceResult::failure(error.what());
        }
        return ServiceResult::success("Bill created and session is waiting for payment.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

const Bill* BillService::findById(const std::string& billId) const {
    return bills_.findById(billId);
}

const DynamicArray<Bill>& BillService::getAll() const noexcept {
    return bills_.getAll();
}

const Bill* BillService::findByDiningSessionId(
    const std::string& diningSessionId) const {
    return bills_.findByDiningSessionId(diningSessionId);
}

}  // namespace restaurant
