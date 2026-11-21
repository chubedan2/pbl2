#pragma once

#include "FileRepository.h"
#include "../Models/Table.h"

namespace restaurant {

struct TableCodec {
    static void write(std::ostream& output, const Table& value);
    static Table read(std::istream& input);
};

class TableRepository : public FileRepository<Table> {
public:
    explicit TableRepository(const std::string& path);
};

}  // namespace restaurant
