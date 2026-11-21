#pragma once

#include "FileRepository.h"
#include "../Models/Order.h"

namespace restaurant {

struct OrderCodec {
    static void write(std::ostream& output, const Order& value);
    static Order read(std::istream& input);
};

class OrderRepository : public FileRepository<Order> {
public:
    explicit OrderRepository(const std::string& path);
};

}  // namespace restaurant
