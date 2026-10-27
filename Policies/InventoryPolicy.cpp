#include "InventoryPolicy.h"

PolicyResult InventoryPolicy::checkItem(const Menu& menu, int menuId) const
{
	const MenuItem* item = menu.findById(menuId);
	if (item == nullptr)
	{
		return PolicyResult::deny("ITEM_NOT_FOUND", "Menu item was not found.");
	}
	if (!item->isAvailable())
	{
		return PolicyResult::deny("ITEM_UNAVAILABLE",
								  "Menu item is not currently served.");
	}
	return PolicyResult::allow();
}
