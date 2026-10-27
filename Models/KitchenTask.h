#ifndef KITCHEN_TASK_H
#define KITCHEN_TASK_H

#include <string>

#include "MenuItem.h"

enum class KitchenTaskStatus
{
	WAITING,
	PREPARING,
	READY,
	SERVED,
	CANCELLED
};

class KitchenTask
{
private:
	int taskId;
	int orderId;
	int menuId;
	std::string itemName;
	int quantity;
	PreparationStation station;
	int priority;
	KitchenTaskStatus status;

public:
	KitchenTask(int taskId, int orderId, int menuId,
				const std::string& itemName, int quantity,
				PreparationStation station, int priority = 0);

	int getTaskId() const;
	int getOrderId() const;
	int getMenuId() const;
	const std::string& getItemName() const;
	int getQuantity() const;
	PreparationStation getStation() const;
	int getPriority() const;
	KitchenTaskStatus getStatus() const;
	bool isPriority() const;

	bool startPreparing();
	bool markReady();
	bool markServed();
	bool cancel();
};

#endif
