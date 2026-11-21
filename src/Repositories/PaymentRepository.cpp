#include "PaymentRepository.h"

#include <iomanip>
#include <limits>
#include <stdexcept>

namespace restaurant {

void PaymentCodec::write(std::ostream& output, const Payment& value) {
    output << std::quoted(value.id_) << ' '
           << std::quoted(value.billId_) << ' '
           << value.amount_ << ' '
           << std::quoted(value.method_) << ' '
           << static_cast<long long>(value.paidAt_) << ' '
           << std::quoted(value.referenceCode_) << ' '
           << std::quoted(value.note_);
}

Payment PaymentCodec::read(std::istream& input) {
    std::string id;
    std::string billId;
    long long amount = 0;
    std::string method;
    long long paidAt = 0;
    std::string referenceCode;
    std::string note;
    if (!(input >> std::quoted(id) >> std::quoted(billId) >>
          amount >> std::quoted(method) >> paidAt >>
          std::quoted(referenceCode) >> std::quoted(note))) {
        throw std::runtime_error("Invalid payment record");
    }
    if (paidAt < std::numeric_limits<std::time_t>::min() ||
        paidAt > std::numeric_limits<std::time_t>::max()) {
        throw std::runtime_error("Invalid payment time in repository file");
    }
    return Payment(id, billId, amount, method,
                   static_cast<std::time_t>(paidAt), referenceCode, note);
}

PaymentRepository::PaymentRepository(const std::string& path)
    : FileRepository<Payment>(
          path,
          [](const Payment& value) { return value.getId(); },
          PaymentCodec::write,
          PaymentCodec::read) {
    const DynamicArray<Payment>& payments = getAll();
    for (std::size_t i = 0; i < payments.size(); ++i) {
        for (std::size_t j = i + 1; j < payments.size(); ++j) {
            if (payments[i].getBillId() == payments[j].getBillId()) {
                throw std::runtime_error(
                    "Multiple payments exist for one bill in repository file");
            }
        }
    }
}

bool PaymentRepository::add(const Payment& payment) {
    if (findByBillId(payment.getBillId()) != nullptr) {
        return false;
    }
    return FileRepository<Payment>::add(payment);
}

const Payment* PaymentRepository::findByBillId(const std::string& billId) const {
    const DynamicArray<Payment>& payments = getAll();
    for (std::size_t i = 0; i < payments.size(); ++i) {
        if (payments[i].getBillId() == billId) {
            return &payments[i];
        }
    }
    return nullptr;
}

}  // namespace restaurant
