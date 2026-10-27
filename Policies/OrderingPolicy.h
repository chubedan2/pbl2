#ifndef ORDERING_POLICY_H
#define ORDERING_POLICY_H

#include "InventoryPolicy.h"
#include "../Models/DiningSession.h"

class OrderingPolicy
{
private:
	InventoryPolicy inventoryPolicy;

public:
	PolicyResult validateSubmission(const DiningSession& session,
									const Order& order,
									const Menu& menu) const;
};

#endif
