#include "MenuItem.h"

#include <stdexcept>

namespace restaurant {

MenuItem::MenuItem(const std::string& id,
                   const std::string& name,
                   long long price,
                   const std::string& description)
    : id_(id), name_(name), price_(price), description_(description) {
    if (!isNonEmpty(id_)) {
        throw std::invalid_argument("Menu item ID must not be empty");
    }
    if (!isNonEmpty(name_)) {
        throw std::invalid_argument("Menu item name must not be empty");
    }
    if (price_ < 0) {
        throw std::invalid_argument("Menu item price must not be negative");
    }
}

const std::string& MenuItem::getId() const noexcept {
    return id_;
}

const std::string& MenuItem::getName() const noexcept {
    return name_;
}

long long MenuItem::getPrice() const noexcept {
    return price_;
}

const std::string& MenuItem::getDescription() const noexcept {
    return description_;
}

bool MenuItem::setName(const std::string& name) {
    if (!isNonEmpty(name)) {
        return false;
    }
    name_ = name;
    return true;
}

bool MenuItem::setPrice(long long price) noexcept {
    if (price < 0) {
        return false;
    }
    price_ = price;
    return true;
}

void MenuItem::setDescription(const std::string& description) {
    description_ = description;
}

bool MenuItem::isNonEmpty(const std::string& value) noexcept {
    return value.find_first_not_of(" \t\n\r") != std::string::npos;
}

}  // namespace restaurant
