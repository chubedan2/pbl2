#include "KitchenRoutingPolicy.h"

#include <limits>

PolicyResult KitchenRoutingPolicy::createTasks(
	const Order& order, const Menu& menu, int firstTaskId,
	std::vector<KitchenTask>& tasks, int priority) const
{
	if ((order.getStatus() != OrderStatus::PENDING &&
		 order.getStatus() != OrderStatus::APPROVED) ||
		firstTaskId <= 0 || priority < 0)
	{
		return PolicyResult::deny("INVALID_ROUTING_REQUEST",
								  "Order state, task ID, or priority is invalid.");
	}
	if (order.getItemCount() >
		static_cast<std::size_t>(std::numeric_limits<int>::max() -
								 firstTaskId + 1))
	{
		return PolicyResult::deny("TASK_ID_EXHAUSTED",
								  "No task IDs are available.");
	}

	std::vector<KitchenTask> created;
	const DoublyLinkedList<OrderItem>& items = order.getItems();
	int taskId = firstTaskId;
	bool itemMissing = false;
	items.forEach([&](const OrderItem& orderItem) {
		const MenuItem* item = menu.findById(orderItem.getMenuId());
		if (item == nullptr || !item->isValid())
		{
			itemMissing = true;
			return;
		}
		created.push_back(KitchenTask(
			taskId++, order.getOrderId(), orderItem.getMenuId(),
			orderItem.getName(), orderItem.getQuantity(), item->getStation(),
			priority));
	});

	if (itemMissing)
	{
		return PolicyResult::deny("ITEM_NOT_FOUND",
								  "A menu item required for routing is missing.");
	}
	tasks.swap(created);
	return PolicyResult::allow();
}
