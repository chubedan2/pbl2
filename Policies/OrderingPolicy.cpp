#include "OrderingPolicy.h"

#include <stdexcept>

PolicyResult OrderingPolicy::validateSubmission(
	const DiningSession& session, const Order& order, const Menu& menu) const
{
	if (!session.isActive() || order.getSessionId() != session.getSessionId())
	{
		return PolicyResult::deny("SESSION_NOT_ACTIVE",
								  "Dining session is not active.");
	}
	if (order.getStatus() != OrderStatus::DRAFT)
	{
		return PolicyResult::deny("ORDER_NOT_EDITABLE",
								  "Order is not in the draft state.");
	}
	if (order.isEmpty())
	{
		return PolicyResult::deny("EMPTY_ORDER", "Order has no items.");
	}

	const DoublyLinkedList<OrderItem>& items = order.getItems();
	bool foundInvalid = false;
	std::string denyCode;
	std::string message;
	items.forEach([&](const OrderItem& orderItem) {
		if (!foundInvalid)
		{
			const PolicyResult result =
				inventoryPolicy.checkItem(menu, orderItem.getMenuId());
			if (!result.isSuccess())
			{
				foundInvalid = true;
				denyCode = result.getDenyCode();
				message = result.getMessage();
			}
		}
	});

	if (foundInvalid)
	{
		return PolicyResult::deny(denyCode, message);
	}

	try
	{
		order.getTotal();
	}
	catch (const std::overflow_error&)
	{
		return PolicyResult::deny("TOTAL_OVERFLOW",
								  "Order total exceeds the supported range.");
	}
	return PolicyResult::allow();
}
