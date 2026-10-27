#ifndef PAYMENT_POLICY_H
#define PAYMENT_POLICY_H

#include "PolicyResult.h"
#include "../Models/Bill.h"
#include "../Models/DiningSession.h"
#include "../Models/KitchenTask.h"

#include <memory>
#include <vector>

class PaymentPolicy
{
public:
	PolicyResult createBill(const DiningSession& session, int billId,
							const std::vector<const KitchenTask*>& tasks,
							PaymentMethod method,
							std::unique_ptr<Bill>& bill) const;
};

#endif
