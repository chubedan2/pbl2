#ifndef INVENTORY_POLICY_H
#define INVENTORY_POLICY_H

#include "PolicyResult.h"
#include "../Models/Menu.h"

class InventoryPolicy
{
public:
	PolicyResult checkItem(const Menu& menu, int menuId) const;
};

#endif
