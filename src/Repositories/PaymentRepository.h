#pragma once

#include "FileRepository.h"
#include "../Models/Payment.h"

namespace restaurant {

struct PaymentCodec {
    static void write(std::ostream& output, const Payment& value);
    static Payment read(std::istream& input);
};

class PaymentRepository : public FileRepository<Payment> {
public:
    explicit PaymentRepository(const std::string& path);

    bool add(const Payment& payment);
    const Payment* findByBillId(const std::string& billId) const;
};

}  // namespace restaurant
