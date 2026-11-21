#pragma once

#include "FileRepository.h"
#include "../Models/DiningSession.h"

namespace restaurant {

struct DiningSessionCodec {
    static void write(std::ostream& output, const DiningSession& value);
    static DiningSession read(std::istream& input);
};

class DiningSessionRepository : public FileRepository<DiningSession> {
public:
    explicit DiningSessionRepository(const std::string& path);
};

}  // namespace restaurant
