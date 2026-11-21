#pragma once

#include "../Repositories/MenuItemRepository.h"
#include "ServiceResult.h"

namespace restaurant {

class MenuService {
public:
    explicit MenuService(MenuItemRepository& items);

    ServiceResult addItem(const std::string& id,
                          const std::string& name,
                          long long price,
                          const std::string& description = std::string());
    ServiceResult updateItem(const std::string& id,
                             const std::string& name,
                             long long price,
                             const std::string& description);
    const MenuItem* findById(const std::string& id) const;
    const DynamicArray<MenuItem>& getAll() const noexcept;

private:
    MenuItemRepository& items_;
};

}  // namespace restaurant
