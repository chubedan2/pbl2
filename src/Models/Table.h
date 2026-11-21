#pragma once

#include <string>

namespace restaurant {

struct TableCodec;

enum class TableStatus {
    Available,
    Occupied,
    Maintenance
};

class Table {
public:
    Table(const std::string& id, const std::string& code);

    const std::string& getId() const noexcept;
    const std::string& getCode() const noexcept;
    TableStatus getStatus() const noexcept;
    bool isActive() const noexcept;

    bool setCode(const std::string& code);
    bool setStatus(TableStatus status) noexcept;
    bool deactivate() noexcept;
    void activate() noexcept;

private:
    friend struct TableCodec;

    static bool isNonEmpty(const std::string& value) noexcept;

    std::string id_;
    std::string code_;
    TableStatus status_;
    bool isActive_;
};

}  // namespace restaurant
