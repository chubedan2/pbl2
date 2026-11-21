#pragma once

#include <string>

namespace restaurant {

struct MenuItemCodec;

class MenuItem {
public:
    MenuItem(const std::string& id,
             const std::string& name,
             long long price,
             const std::string& description = std::string());

    const std::string& getId() const noexcept;
    const std::string& getName() const noexcept;
    long long getPrice() const noexcept;
    const std::string& getDescription() const noexcept;

    bool setName(const std::string& name);
    bool setPrice(long long price) noexcept;
    void setDescription(const std::string& description);

private:
    friend struct MenuItemCodec;

    static bool isNonEmpty(const std::string& value) noexcept;

    std::string id_;
    std::string name_;
    long long price_;
    std::string description_;
};

}  // namespace restaurant
