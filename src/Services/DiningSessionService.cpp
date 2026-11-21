#include "DiningSessionService.h"

#include <exception>

namespace restaurant {

DiningSessionService::DiningSessionService(TableRepository& tables,
                                           DiningSessionRepository& sessions,
                                           OrderRepository& orders)
    : tables_(tables), sessions_(sessions), orders_(orders) {}

ServiceResult DiningSessionService::openSession(const std::string& sessionId,
                                                const std::string& tableId,
                                                int guestCount,
                                                std::time_t openedAt,
                                                const std::string& note) {
    if (guestCount <= 0) {
        return ServiceResult::failure("Guest count must be greater than zero.");
    }
    if (sessions_.findById(sessionId) != nullptr) {
        return ServiceResult::failure("A dining session with this ID already exists.");
    }
    const Table* storedTable = tables_.findById(tableId);
    if (storedTable == nullptr) {
        return ServiceResult::failure("Selected table was not found.");
    }
    if (!storedTable->isActive() || storedTable->getStatus() != TableStatus::Available) {
        return ServiceResult::failure("Selected table is no longer available.");
    }

    const Table oldTable(*storedTable);
    try {
        DiningSession session(sessionId, tableId, guestCount, openedAt, note);
        Table updatedTable(*storedTable);
        if (!updatedTable.setStatus(TableStatus::Occupied)) {
            return ServiceResult::failure("Selected table cannot be occupied.");
        }
        if (!tables_.update(updatedTable)) {
            return ServiceResult::failure("Could not update selected table.");
        }
        if (!sessions_.add(session)) {
            if (!tables_.update(oldTable)) {
                return ServiceResult::failure(
                    "Session creation failed and the table rollback also failed.");
            }
            return ServiceResult::failure("Could not save dining session.");
        }
        return ServiceResult::success("Dining session opened.");
    } catch (const std::exception& error) {
        const bool restored = tables_.update(oldTable);
        if (!restored) {
            return ServiceResult::failure(
                std::string(error.what()) + "; table rollback failed.");
        }
        return ServiceResult::failure(error.what());
    }
}

ServiceResult DiningSessionService::cancelSession(const std::string& sessionId,
                                                  std::time_t cancelledAt) {
    const DiningSession* storedSession = sessions_.findById(sessionId);
    if (storedSession == nullptr) {
        return ServiceResult::failure("Dining session was not found.");
    }
    if (storedSession->getStatus() != DiningSessionStatus::Active) {
        return ServiceResult::failure("Only an active dining session can be cancelled.");
    }

    const Table* storedTable = tables_.findById(storedSession->getTableId());
    if (storedTable == nullptr || storedTable->getStatus() != TableStatus::Occupied) {
        return ServiceResult::failure("The session table is missing or not occupied.");
    }
    const Table oldTable(*storedTable);

    DynamicArray<Order> oldOrders;
    DynamicArray<Order> cancelledOrders;
    const DynamicArray<Order>& allOrders = orders_.getAll();
    for (std::size_t i = 0; i < allOrders.size(); ++i) {
        if (allOrders[i].getDiningSessionId() != sessionId) {
            continue;
        }
        Order updated(allOrders[i]);
        for (std::size_t itemIndex = 0; itemIndex < updated.getItemCount(); ++itemIndex) {
            if (updated.getItemAt(itemIndex).getStatus() == OrderItemStatus::Served) {
                return ServiceResult::failure(
                    "A session with served items cannot be cancelled.");
            }
        }
        if (updated.getStatus() != OrderStatus::Cancelled && !updated.cancel()) {
            return ServiceResult::failure("An order in the session cannot be cancelled.");
        }
        oldOrders.pushBack(allOrders[i]);
        cancelledOrders.pushBack(updated);
    }

    DiningSession updatedSession(*storedSession);
    if (!updatedSession.cancel(cancelledAt)) {
        return ServiceResult::failure("Dining session cannot be cancelled at this time.");
    }
    Table updatedTable(*storedTable);
    if (!updatedTable.setStatus(TableStatus::Available)) {
        return ServiceResult::failure("Session table cannot be released.");
    }

    std::size_t ordersSaved = 0;
    bool tableSaved = false;
    try {
        for (; ordersSaved < cancelledOrders.size(); ++ordersSaved) {
            if (!orders_.update(cancelledOrders[ordersSaved])) {
                throw std::runtime_error("Could not update an order during session cancellation.");
            }
        }
        if (!tables_.update(updatedTable)) {
            throw std::runtime_error("Could not release the session table.");
        }
        tableSaved = true;
        if (!sessions_.update(updatedSession)) {
            throw std::runtime_error("Could not save cancelled dining session.");
        }
        return ServiceResult::success("Dining session cancelled.");
    } catch (const std::exception& error) {
        bool rollbackSucceeded = true;
        if (tableSaved && !tables_.update(oldTable)) {
            rollbackSucceeded = false;
        }
        while (ordersSaved > 0) {
            --ordersSaved;
            if (!orders_.update(oldOrders[ordersSaved])) {
                rollbackSucceeded = false;
            }
        }
        if (!rollbackSucceeded) {
            return ServiceResult::failure(
                std::string(error.what()) + "; one or more rollback operations failed.");
        }
        return ServiceResult::failure(error.what());
    }
}

const DiningSession* DiningSessionService::findById(const std::string& id) const {
    return sessions_.findById(id);
}

const DynamicArray<DiningSession>& DiningSessionService::getAll() const noexcept {
    return sessions_.getAll();
}

}  // namespace restaurant
