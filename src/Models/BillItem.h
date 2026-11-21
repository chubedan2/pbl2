#pragma once

#include <string>

namespace restaurant {

struct BillCodec;

class BillItem {
public:
    BillItem(const std::string& id,
             const std::string& menuItemId,
             const std::string& itemNameSnapshot,
             long long unitPriceSnapshot,
             int quantity);

    const std::string& getId() const noexcept;
    const std::string& getMenuItemId() const noexcept;
    const std::string& getItemNameSnapshot() const noexcept;
    long long getUnitPriceSnapshot() const noexcept;
    int getQuantity() const noexcept;
    long long getLineTotal() const noexcept;

private:
    friend struct BillCodec;

    static bool isNonEmpty(const std::string& value) noexcept;
    static long long calculateLineTotal(long long unitPrice, int quantity);

    std::string id_;
    std::string menuItemId_;
    std::string itemNameSnapshot_;
    long long unitPriceSnapshot_;
    int quantity_;
    long long lineTotal_;
};

}  // namespace restaurant
