#include "TableService.h"

#include <exception>
#include <utility>

namespace restaurant {

TableService::TableService(TableRepository& tables)
    : tables_(tables) {}

ServiceResult TableService::addTable(const std::string& id,
                                     const std::string& code) {
    try {
        if (!tables_.add(Table(id, code))) {
            return ServiceResult::failure("A table with this ID already exists.");
        }
        return ServiceResult::success("Table added.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

ServiceResult TableService::changeCode(const std::string& id,
                                       const std::string& code) {
    Table* stored = tables_.findById(id);
    if (stored == nullptr) {
        return ServiceResult::failure("Table was not found.");
    }
    Table updated(*stored);
    if (!updated.setCode(code)) {
        return ServiceResult::failure("Table code must not be empty.");
    }
    try {
        return tables_.update(updated)
                   ? ServiceResult::success("Table updated.")
                   : ServiceResult::failure("Table could not be updated.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

ServiceResult TableService::changeStatus(const std::string& id,
                                         TableStatus status) {
    if (status == TableStatus::Occupied) {
        return ServiceResult::failure(
            "Only DiningSessionService may occupy a table.");
    }
    Table* stored = tables_.findById(id);
    if (stored == nullptr) {
        return ServiceResult::failure("Table was not found.");
    }
    Table updated(*stored);
    if (!updated.setStatus(status)) {
        return ServiceResult::failure("The requested table status transition is not allowed.");
    }
    try {
        return tables_.update(updated)
                   ? ServiceResult::success("Table status updated.")
                   : ServiceResult::failure("Table could not be updated.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

ServiceResult TableService::deactivate(const std::string& id) {
    Table* stored = tables_.findById(id);
    if (stored == nullptr) {
        return ServiceResult::failure("Table was not found.");
    }
    Table updated(*stored);
    if (!updated.deactivate()) {
        return ServiceResult::failure("An occupied table cannot be deactivated.");
    }
    try {
        return tables_.update(updated)
                   ? ServiceResult::success("Table deactivated.")
                   : ServiceResult::failure("Table could not be updated.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

DynamicArray<Table> TableService::getAvailableTables() const {
    DynamicArray<Table> available;
    const DynamicArray<Table>& all = tables_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].isActive() && all[i].getStatus() == TableStatus::Available) {
            available.pushBack(all[i]);
        }
    }
    return available;
}

const DynamicArray<Table>& TableService::getAll() const noexcept {
    return tables_.getAll();
}

const Table* TableService::findById(const std::string& id) const {
    return tables_.findById(id);
}

}  // namespace restaurant
