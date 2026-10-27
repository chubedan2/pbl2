#include "Table.h"

#include <stdexcept>

Table::Table(const std::string& tableId, int capacity)
	: tableId(tableId), capacity(capacity), sessionId(0),
	  status(TableStatus::AVAILABLE)
{
	if (tableId.empty() || capacity <= 0)
	{
		throw std::invalid_argument(
			"Table ID must not be empty and capacity must be positive.");
	}
}

const std::string& Table::getTableId() const
{
	return tableId;
}

int Table::getCapacity() const
{
	return capacity;
}

int Table::getSessionId() const
{
	return sessionId;
}

TableStatus Table::getStatus() const
{
	return status;
}

bool Table::isAvailable() const
{
	return status == TableStatus::AVAILABLE;
}

bool Table::isOccupied() const
{
	return status == TableStatus::OCCUPIED;
}

bool Table::openSession(int sessionId)
{
	if (sessionId <= 0 || !isAvailable() || this->sessionId != 0)
	{
		return false;
	}

	this->sessionId = sessionId;
	status = TableStatus::OCCUPIED;
	return true;
}

bool Table::closeSession(int sessionId, bool billPaid,
						 bool noPendingOrders, bool noUnfinishedTasks)
{
	if (sessionId <= 0 || !isOccupied() || this->sessionId != sessionId ||
		!billPaid || !noPendingOrders || !noUnfinishedTasks)
	{
		return false;
	}

	this->sessionId = 0;
	status = TableStatus::AVAILABLE;
	return true;
}
