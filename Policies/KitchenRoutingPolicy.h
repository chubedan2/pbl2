#ifndef KITCHEN_ROUTING_POLICY_H
#define KITCHEN_ROUTING_POLICY_H

#include "PolicyResult.h"
#include "../Models/Menu.h"
#include "../Models/KitchenTask.h"
#include "../Models/Order.h"

#include <vector>

class KitchenRoutingPolicy
{
public:
	PolicyResult createTasks(const Order& order, const Menu& menu,
							 int firstTaskId,
							 std::vector<KitchenTask>& tasks,
							 int priority = 0) const;
};

#endif
