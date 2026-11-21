#include "Table.h"

#include <stdexcept>

namespace restaurant {

Table::Table(const std::string& id, const std::string& code)
    : id_(id), code_(code), status_(TableStatus::Available), isActive_(true) {
    if (!isNonEmpty(id_)) {
        throw std::invalid_argument("Table ID must not be empty");
    }
    if (!isNonEmpty(code_)) {
        throw std::invalid_argument("Table code must not be empty");
    }
}

const std::string& Table::getId() const noexcept {
    return id_;
}

const std::string& Table::getCode() const noexcept {
    return code_;
}

TableStatus Table::getStatus() const noexcept {
    return status_;
}

bool Table::isActive() const noexcept {
    return isActive_;
}

bool Table::setCode(const std::string& code) {
    if (!isNonEmpty(code)) {
        return false;
    }
    code_ = code;
    return true;
}

bool Table::setStatus(TableStatus status) noexcept {
    if (!isActive_ && status == TableStatus::Occupied) {
        return false;
    }

    const bool validTransition =
        (status_ == TableStatus::Available &&
         (status == TableStatus::Occupied || status == TableStatus::Maintenance)) ||
        (status_ == TableStatus::Occupied && status == TableStatus::Available) ||
        (status_ == TableStatus::Maintenance && status == TableStatus::Available);

    if (!validTransition) {
        return false;
    }

    status_ = status;
    return true;
}

bool Table::deactivate() noexcept {
    if (status_ == TableStatus::Occupied) {
        return false;
    }
    isActive_ = false;
    return true;
}

void Table::activate() noexcept {
    isActive_ = true;
}

bool Table::isNonEmpty(const std::string& value) noexcept {
    return value.find_first_not_of(" \t\n\r") != std::string::npos;
}

}  // namespace restaurant
