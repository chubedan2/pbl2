#pragma once

#include "FileRepository.h"
#include "../Models/Bill.h"

namespace restaurant {

struct BillCodec {
    static void write(std::ostream& output, const Bill& value);
    static Bill read(std::istream& input);
};

class BillRepository : public FileRepository<Bill> {
public:
    explicit BillRepository(const std::string& path);

    bool add(const Bill& bill);
    bool update(const Bill& bill);
    const Bill* findByDiningSessionId(const std::string& diningSessionId) const;
};

}  // namespace restaurant
