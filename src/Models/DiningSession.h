#pragma once

#include <ctime>
#include <string>

namespace restaurant {

struct DiningSessionCodec;

enum class DiningSessionStatus {
    Active,
    WaitingForPayment,
    Closed,
    Cancelled
};

class DiningSession {
public:
    DiningSession(const std::string& id,
                  const std::string& tableId,
                  int guestCount,
                  std::time_t openedAt,
                  const std::string& note = std::string());

    const std::string& getId() const noexcept;
    const std::string& getTableId() const noexcept;
    int getGuestCount() const noexcept;
    std::time_t getOpenedAt() const noexcept;
    bool hasClosedAt() const noexcept;
    std::time_t getClosedAt() const;
    DiningSessionStatus getStatus() const noexcept;
    const std::string& getNote() const noexcept;

    bool startWaitingForPayment() noexcept;
    bool close(std::time_t closedAt) noexcept;
    bool cancel(std::time_t closedAt) noexcept;

private:
    friend struct DiningSessionCodec;

    static bool isNonEmpty(const std::string& value) noexcept;

    std::string id_;
    std::string tableId_;
    int guestCount_;
    std::time_t openedAt_;
    std::time_t closedAt_;
    bool hasClosedAt_;
    DiningSessionStatus status_;
    std::string note_;
};

}  // namespace restaurant
