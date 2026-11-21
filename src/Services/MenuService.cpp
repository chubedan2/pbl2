#include "MenuService.h"

#include <exception>

namespace restaurant {

MenuService::MenuService(MenuItemRepository& items)
    : items_(items) {}

ServiceResult MenuService::addItem(const std::string& id,
                                   const std::string& name,
                                   long long price,
                                   const std::string& description) {
    try {
        if (!items_.add(MenuItem(id, name, price, description))) {
            return ServiceResult::failure("A menu item with this ID already exists.");
        }
        return ServiceResult::success("Menu item added.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

ServiceResult MenuService::updateItem(const std::string& id,
                                      const std::string& name,
                                      long long price,
                                      const std::string& description) {
    const MenuItem* stored = items_.findById(id);
    if (stored == nullptr) {
        return ServiceResult::failure("Menu item was not found.");
    }
    MenuItem updated(*stored);
    if (!updated.setName(name)) {
        return ServiceResult::failure("Menu item name must not be empty.");
    }
    if (!updated.setPrice(price)) {
        return ServiceResult::failure("Menu item price must not be negative.");
    }
    updated.setDescription(description);
    try {
        return items_.update(updated)
                   ? ServiceResult::success("Menu item updated.")
                   : ServiceResult::failure("Menu item could not be updated.");
    } catch (const std::exception& error) {
        return ServiceResult::failure(error.what());
    }
}

const MenuItem* MenuService::findById(const std::string& id) const {
    return items_.findById(id);
}

const DynamicArray<MenuItem>& MenuService::getAll() const noexcept {
    return items_.getAll();
}

}  // namespace restaurant
