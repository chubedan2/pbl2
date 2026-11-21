#include "DiningSession.h"

#include <stdexcept>

namespace restaurant {

DiningSession::DiningSession(const std::string& id,
                             const std::string& tableId,
                             int guestCount,
                             std::time_t openedAt,
                             const std::string& note)
    : id_(id),
      tableId_(tableId),
      guestCount_(guestCount),
      openedAt_(openedAt),
      closedAt_(),
      hasClosedAt_(false),
      status_(DiningSessionStatus::Active),
      note_(note) {
    if (!isNonEmpty(id_)) {
        throw std::invalid_argument("Dining session ID must not be empty");
    }
    if (!isNonEmpty(tableId_)) {
        throw std::invalid_argument("Dining session table ID must not be empty");
    }
    if (guestCount_ <= 0) {
        throw std::invalid_argument("Guest count must be greater than zero");
    }
}

const std::string& DiningSession::getId() const noexcept {
    return id_;
}

const std::string& DiningSession::getTableId() const noexcept {
    return tableId_;
}

int DiningSession::getGuestCount() const noexcept {
    return guestCount_;
}

std::time_t DiningSession::getOpenedAt() const noexcept {
    return openedAt_;
}

bool DiningSession::hasClosedAt() const noexcept {
    return hasClosedAt_;
}

std::time_t DiningSession::getClosedAt() const {
    if (!hasClosedAt_) {
        throw std::logic_error("Dining session has not been closed");
    }
    return closedAt_;
}

DiningSessionStatus DiningSession::getStatus() const noexcept {
    return status_;
}

const std::string& DiningSession::getNote() const noexcept {
    return note_;
}

bool DiningSession::startWaitingForPayment() noexcept {
    if (status_ != DiningSessionStatus::Active) {
        return false;
    }
    status_ = DiningSessionStatus::WaitingForPayment;
    return true;
}

bool DiningSession::close(std::time_t closedAt) noexcept {
    if (status_ != DiningSessionStatus::WaitingForPayment || closedAt < openedAt_) {
        return false;
    }
    closedAt_ = closedAt;
    hasClosedAt_ = true;
    status_ = DiningSessionStatus::Closed;
    return true;
}

bool DiningSession::cancel(std::time_t closedAt) noexcept {
    if (status_ != DiningSessionStatus::Active || closedAt < openedAt_) {
        return false;
    }
    closedAt_ = closedAt;
    hasClosedAt_ = true;
    status_ = DiningSessionStatus::Cancelled;
    return true;
}

bool DiningSession::isNonEmpty(const std::string& value) noexcept {
    return value.find_first_not_of(" \t\n\r") != std::string::npos;
}

}  // namespace restaurant
