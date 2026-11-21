#include "TableRepository.h"

#include <iomanip>
#include <stdexcept>

namespace restaurant {

void TableCodec::write(std::ostream& output, const Table& value) {
    output << std::quoted(value.id_) << ' '
           << std::quoted(value.code_) << ' '
           << static_cast<int>(value.status_) << ' '
           << value.isActive_;
}

Table TableCodec::read(std::istream& input) {
    std::string id;
    std::string code;
    int status = -1;
    bool active = false;
    if (!(input >> std::quoted(id) >> std::quoted(code) >> status >> active)) {
        throw std::runtime_error("Invalid table record");
    }
    if (status < static_cast<int>(TableStatus::Available) ||
        status > static_cast<int>(TableStatus::Maintenance)) {
        throw std::runtime_error("Invalid table status in repository file");
    }

    Table value(id, code);
    value.status_ = static_cast<TableStatus>(status);
    value.isActive_ = active;
    if (!active && value.status_ == TableStatus::Occupied) {
        throw std::runtime_error("Inactive table cannot be occupied");
    }
    return value;
}

TableRepository::TableRepository(const std::string& path)
    : FileRepository<Table>(
          path,
          [](const Table& value) { return value.getId(); },
          TableCodec::write,
          TableCodec::read) {}

}  // namespace restaurant
