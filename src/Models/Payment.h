#pragma once

#include <ctime>
#include <string>

namespace restaurant {

struct PaymentCodec;

class Payment {
public:
    Payment(const std::string& id,
            const std::string& billId,
            long long amount,
            const std::string& method,
            std::time_t paidAt,
            const std::string& referenceCode = std::string(),
            const std::string& note = std::string());

    const std::string& getId() const noexcept;
    const std::string& getBillId() const noexcept;
    long long getAmount() const noexcept;
    const std::string& getMethod() const noexcept;
    std::time_t getPaidAt() const noexcept;
    const std::string& getReferenceCode() const noexcept;
    const std::string& getNote() const noexcept;

private:
    friend struct PaymentCodec;

    static bool isNonEmpty(const std::string& value) noexcept;

    std::string id_;
    std::string billId_;
    long long amount_;
    std::string method_;
    std::time_t paidAt_;
    std::string referenceCode_;
    std::string note_;
};

}  // namespace restaurant
