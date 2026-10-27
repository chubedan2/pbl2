#include "KitchenTask.h"

#include <stdexcept>

namespace
{
bool isValidStation(PreparationStation station)
{
	return station == PreparationStation::KITCHEN ||
		   station == PreparationStation::BAR;
}
}

KitchenTask::KitchenTask(int taskId, int orderId, int menuId,
						 const std::string& itemName, int quantity,
						 PreparationStation station, int priority)
	: taskId(taskId), orderId(orderId), menuId(menuId), itemName(itemName),
	  quantity(quantity), station(station), priority(priority),
	  status(KitchenTaskStatus::WAITING)
{
	if (taskId <= 0 || orderId <= 0 || menuId <= 0 || itemName.empty() ||
		quantity <= 0 || priority < 0 || !isValidStation(station))
	{
		throw std::invalid_argument("Invalid kitchen task data.");
	}
}

int KitchenTask::getTaskId() const { return taskId; }
int KitchenTask::getOrderId() const { return orderId; }
int KitchenTask::getMenuId() const { return menuId; }
const std::string& KitchenTask::getItemName() const { return itemName; }
int KitchenTask::getQuantity() const { return quantity; }
PreparationStation KitchenTask::getStation() const { return station; }
int KitchenTask::getPriority() const { return priority; }
KitchenTaskStatus KitchenTask::getStatus() const { return status; }
bool KitchenTask::isPriority() const { return priority > 0; }

bool KitchenTask::startPreparing()
{
	if (status != KitchenTaskStatus::WAITING)
	{
		return false;
	}
	status = KitchenTaskStatus::PREPARING;
	return true;
}

bool KitchenTask::markReady()
{
	if (status != KitchenTaskStatus::PREPARING)
	{
		return false;
	}
	status = KitchenTaskStatus::READY;
	return true;
}

bool KitchenTask::markServed()
{
	if (status != KitchenTaskStatus::READY)
	{
		return false;
	}
	status = KitchenTaskStatus::SERVED;
	return true;
}

bool KitchenTask::cancel()
{
	if (status != KitchenTaskStatus::WAITING)
	{
		return false;
	}
	status = KitchenTaskStatus::CANCELLED;
	return true;
}
