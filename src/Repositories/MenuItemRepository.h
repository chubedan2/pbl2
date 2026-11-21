#pragma once

#include "FileRepository.h"
#include "../Models/MenuItem.h"

namespace restaurant {

struct MenuItemCodec {
    static void write(std::ostream& output, const MenuItem& value);
    static MenuItem read(std::istream& input);
};

class MenuItemRepository : public FileRepository<MenuItem> {
public:
    explicit MenuItemRepository(const std::string& path);
};

}  // namespace restaurant
