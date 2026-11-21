#include "DiningSessionRepository.h"

#include <iomanip>
#include <limits>
#include <stdexcept>

namespace restaurant {

void DiningSessionCodec::write(std::ostream& output, const DiningSession& value) {
    output << std::quoted(value.id_) << ' '
           << std::quoted(value.tableId_) << ' '
           << value.guestCount_ << ' '
           << static_cast<long long>(value.openedAt_) << ' '
           << value.hasClosedAt_ << ' '
           << static_cast<long long>(value.closedAt_) << ' '
           << static_cast<int>(value.status_) << ' '
           << std::quoted(value.note_);
}

DiningSession DiningSessionCodec::read(std::istream& input) {
    std::string id;
    std::string tableId;
    int guestCount = 0;
    long long openedAt = 0;
    bool hasClosedAt = false;
    long long closedAt = 0;
    int status = -1;
    std::string note;
    if (!(input >> std::quoted(id) >> std::quoted(tableId) >>
          guestCount >> openedAt >> hasClosedAt >> closedAt >>
          status >> std::quoted(note))) {
        throw std::runtime_error("Invalid dining session record");
    }
    if (status < static_cast<int>(DiningSessionStatus::Active) ||
        status > static_cast<int>(DiningSessionStatus::Cancelled) ||
        openedAt < std::numeric_limits<std::time_t>::min() ||
        openedAt > std::numeric_limits<std::time_t>::max() ||
        closedAt < std::numeric_limits<std::time_t>::min() ||
        closedAt > std::numeric_limits<std::time_t>::max()) {
        throw std::runtime_error("Invalid dining session data in repository file");
    }

    DiningSession value(id, tableId, guestCount,
                        static_cast<std::time_t>(openedAt), note);
    value.status_ = static_cast<DiningSessionStatus>(status);
    value.hasClosedAt_ = hasClosedAt;
    value.closedAt_ = static_cast<std::time_t>(closedAt);
    if ((value.status_ == DiningSessionStatus::Closed ||
         value.status_ == DiningSessionStatus::Cancelled) != value.hasClosedAt_ ||
        (value.hasClosedAt_ && value.closedAt_ < value.openedAt_)) {
        throw std::runtime_error("Inconsistent dining session state in repository file");
    }
    return value;
}

DiningSessionRepository::DiningSessionRepository(const std::string& path)
    : FileRepository<DiningSession>(
          path,
          [](const DiningSession& value) { return value.getId(); },
          DiningSessionCodec::write,
          DiningSessionCodec::read) {}

}  // namespace restaurant
