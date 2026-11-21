#include "BillRepository.h"

#include <iomanip>
#include <limits>
#include <stdexcept>
#include <utility>

namespace restaurant {

void BillCodec::write(std::ostream& output, const Bill& value) {
    output << std::quoted(value.id_) << ' '
           << std::quoted(value.diningSessionId_) << ' '
           << static_cast<long long>(value.createdAt_) << ' '
           << static_cast<int>(value.paymentStatus_) << ' '
           << value.items_.size();
    for (std::size_t i = 0; i < value.items_.size(); ++i) {
        const BillItem& item = value.items_[i];
        output << ' ' << std::quoted(item.id_)
               << ' ' << std::quoted(item.menuItemId_)
               << ' ' << std::quoted(item.itemNameSnapshot_)
               << ' ' << item.unitPriceSnapshot_
               << ' ' << item.quantity_;
    }
}

Bill BillCodec::read(std::istream& input) {
    std::string id;
    std::string sessionId;
    long long createdAt = 0;
    int status = -1;
    std::size_t itemCount = 0;
    if (!(input >> std::quoted(id) >> std::quoted(sessionId) >>
          createdAt >> status >> itemCount)) {
        throw std::runtime_error("Invalid bill record");
    }
    if (createdAt < std::numeric_limits<std::time_t>::min() ||
        createdAt > std::numeric_limits<std::time_t>::max() ||
        (status != static_cast<int>(BillPaymentStatus::Unpaid) &&
         status != static_cast<int>(BillPaymentStatus::Paid))) {
        throw std::runtime_error("Invalid bill data in repository file");
    }

    DynamicArray<BillItem> items;
    for (std::size_t i = 0; i < itemCount; ++i) {
        std::string itemId;
        std::string menuItemId;
        std::string name;
        long long unitPrice = 0;
        int quantity = 0;
        if (!(input >> std::quoted(itemId) >> std::quoted(menuItemId) >>
              std::quoted(name) >> unitPrice >> quantity)) {
            throw std::runtime_error("Invalid bill item record");
        }
        BillItem item(itemId, menuItemId, name, unitPrice, quantity);
        for (std::size_t existing = 0; existing < items.size(); ++existing) {
            if (items[existing].getId() == itemId) {
                throw std::runtime_error("Duplicate bill item ID in repository file");
            }
        }
        items.pushBack(std::move(item));
    }
    Bill value(id, sessionId, static_cast<std::time_t>(createdAt), std::move(items));
    value.paymentStatus_ = static_cast<BillPaymentStatus>(status);
    return value;
}

BillRepository::BillRepository(const std::string& path)
    : FileRepository<Bill>(
          path,
          [](const Bill& value) { return value.getId(); },
          BillCodec::write,
          BillCodec::read) {
    const DynamicArray<Bill>& bills = getAll();
    for (std::size_t i = 0; i < bills.size(); ++i) {
        for (std::size_t j = i + 1; j < bills.size(); ++j) {
            if (bills[i].getDiningSessionId() == bills[j].getDiningSessionId()) {
                throw std::runtime_error(
                    "Multiple bills exist for one dining session in repository file");
            }
        }
    }
}

bool BillRepository::add(const Bill& bill) {
    const Bill* existing = findByDiningSessionId(bill.getDiningSessionId());
    if (existing != nullptr) {
        return false;
    }
    return FileRepository<Bill>::add(bill);
}

bool BillRepository::update(const Bill& bill) {
    const Bill* existing = findByDiningSessionId(bill.getDiningSessionId());
    if (existing != nullptr && existing->getId() != bill.getId()) {
        return false;
    }
    return FileRepository<Bill>::update(bill);
}

const Bill* BillRepository::findByDiningSessionId(
    const std::string& diningSessionId) const {
    const DynamicArray<Bill>& bills = getAll();
    for (std::size_t i = 0; i < bills.size(); ++i) {
        if (bills[i].getDiningSessionId() == diningSessionId) {
            return &bills[i];
        }
    }
    return nullptr;
}

}  // namespace restaurant
