#include "DiningSession.h"

#include <limits>
#include <memory>
#include <new>
#include <stdexcept>
#include <utility>

DiningSession::DiningSession(int sessionId, const std::string& tableId)
	: sessionId(sessionId),
	  tableId(tableId),
	  status(DiningSessionStatus::ACTIVE),
	  createdAt(std::time(nullptr)),
	  closedAt(0),
	  orders(),
	  orderCount(0),
	  orderCapacity(0)
{
	if (sessionId <= 0 || tableId.empty())
	{
		throw std::invalid_argument(
			"Session ID must be positive and table ID must not be empty.");
	}
}

DiningSession::~DiningSession() = default;

bool DiningSession::ensureOrderCapacity()
{
	if (orderCount < orderCapacity)
	{
		return true;
	}

	if (orderCapacity > std::numeric_limits<std::size_t>::max() / 2)
	{
		return false;
	}

	const std::size_t newCapacity = orderCapacity == 0 ? 2 : orderCapacity * 2;
	std::unique_ptr<std::unique_ptr<Order>[]> expandedOrders(
		new (std::nothrow) std::unique_ptr<Order>[newCapacity]);
	if (!expandedOrders)
	{
		return false;
	}

	for (std::size_t index = 0; index < orderCount; ++index)
	{
		expandedOrders[index] = std::move(orders[index]);
	}

	orders = std::move(expandedOrders);
	orderCapacity = newCapacity;
	return true;
}

int DiningSession::getSessionId() const
{
	return sessionId;
}

const std::string& DiningSession::getTableId() const
{
	return tableId;
}

DiningSessionStatus DiningSession::getStatus() const
{
	return status;
}

std::time_t DiningSession::getCreatedAt() const
{
	return createdAt;
}

std::time_t DiningSession::getClosedAt() const
{
	return closedAt;
}

std::size_t DiningSession::getOrderCount() const
{
	return orderCount;
}

Order* DiningSession::getOrderAt(std::size_t index)
{
	return index < orderCount ? orders[index].get() : nullptr;
}

const Order* DiningSession::getOrderAt(std::size_t index) const
{
	return index < orderCount ? orders[index].get() : nullptr;
}

Order* DiningSession::createOrder(int orderId)
{
	if (!isActive() || orderId <= 0 || findOrder(orderId) != nullptr ||
		!ensureOrderCapacity())
	{
		return nullptr;
	}

	std::unique_ptr<Order> order(new Order(orderId, sessionId));
	orders[orderCount] = std::move(order);
	return orders[orderCount++].get();
}

Order* DiningSession::findOrder(int orderId)
{
	return const_cast<Order*>(
		static_cast<const DiningSession*>(this)->findOrder(orderId));
}

const Order* DiningSession::findOrder(int orderId) const
{
	for (std::size_t index = 0; index < orderCount; ++index)
	{
		if (orders[index]->getOrderId() == orderId)
		{
			return orders[index].get();
		}
	}
	return nullptr;
}

bool DiningSession::hasUnresolvedOrders() const
{
	for (std::size_t index = 0; index < orderCount; ++index)
	{
		const OrderStatus orderStatus = orders[index]->getStatus();
		if (orderStatus != OrderStatus::COMPLETED &&
			orderStatus != OrderStatus::REJECTED)
		{
			return true;
		}
	}
	return false;
}

bool DiningSession::isActive() const
{
	return status == DiningSessionStatus::ACTIVE;
}

bool DiningSession::isPaymentPending() const
{
	return status == DiningSessionStatus::PAYMENT_PENDING;
}

bool DiningSession::isClosed() const
{
	return status == DiningSessionStatus::CLOSED;
}

bool DiningSession::requestPayment()
{
	if (status != DiningSessionStatus::ACTIVE || hasUnresolvedOrders())
	{
		return false;
	}

	status = DiningSessionStatus::PAYMENT_PENDING;
	return true;
}

bool DiningSession::close(bool billPaid, bool noPendingOrders,
						  bool noUnfinishedTasks)
{
	if (status != DiningSessionStatus::PAYMENT_PENDING ||
		!billPaid || !noPendingOrders || hasUnresolvedOrders() ||
		!noUnfinishedTasks)
	{
		return false;
	}

	closedAt = std::time(nullptr);
	status = DiningSessionStatus::CLOSED;
	return true;
}
