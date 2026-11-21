#include "MenuItemRepository.h"

#include <iomanip>
#include <stdexcept>

namespace restaurant {

void MenuItemCodec::write(std::ostream& output, const MenuItem& value) {
    output << std::quoted(value.getId()) << ' '
           << std::quoted(value.getName()) << ' '
           << value.getPrice() << ' '
           << std::quoted(value.getDescription());
}

MenuItem MenuItemCodec::read(std::istream& input) {
    std::string id;
    std::string name;
    long long price = 0;
    std::string description;
    if (!(input >> std::quoted(id) >> std::quoted(name) >>
          price >> std::quoted(description))) {
        throw std::runtime_error("Invalid menu item record");
    }
    return MenuItem(id, name, price, description);
}

MenuItemRepository::MenuItemRepository(const std::string& path)
    : FileRepository<MenuItem>(
          path,
          [](const MenuItem& value) { return value.getId(); },
          MenuItemCodec::write,
          MenuItemCodec::read) {}

}  // namespace restaurant
