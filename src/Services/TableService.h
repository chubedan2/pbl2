#pragma once

#include "../Repositories/TableRepository.h"
#include "ServiceResult.h"

namespace restaurant {

class TableService {
public:
    explicit TableService(TableRepository& tables);

    ServiceResult addTable(const std::string& id, const std::string& code);
    ServiceResult changeCode(const std::string& id, const std::string& code);
    ServiceResult changeStatus(const std::string& id, TableStatus status);
    ServiceResult deactivate(const std::string& id);
    DynamicArray<Table> getAvailableTables() const;
    const DynamicArray<Table>& getAll() const noexcept;
    const Table* findById(const std::string& id) const;

private:
    TableRepository& tables_;
};

}  // namespace restaurant
