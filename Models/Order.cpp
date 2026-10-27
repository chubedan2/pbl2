#include "Order.h"

#include <limits>
#include <stdexcept>

OrderItem::OrderItem(int menuId, const std::string& name,
					 std::int64_t unitPrice, int quantity)
	: menuId(menuId), name(name), unitPrice(unitPrice), quantity(quantity)
{
	if (menuId <= 0 || name.empty() || unitPrice < 0 || quantity <= 0)
	{
		throw std::invalid_argument("Invalid order item");
	}
}

int OrderItem::getMenuId() const
{
	return menuId;
}

const std::string& OrderItem::getName() const
{
	return name;
}

std::int64_t OrderItem::getUnitPrice() const
{
	return unitPrice;
}

int OrderItem::getQuantity() const
{
	return quantity;
}

bool OrderItem::setQuantity(int newQuantity)
{
	if (newQuantity <= 0)
	{
		return false;
	}
	quantity = newQuantity;
	return true;
}

Order::Order(int orderId, int sessionId)
	: orderId(orderId),
	  sessionId(sessionId),
	  items(),
	  status(OrderStatus::DRAFT),
	  denyCode()
{
	if (orderId <= 0 || sessionId <= 0)
	{
		throw std::invalid_argument(
			"Order ID and dining session ID must be positive.");
	}
}

Order::~Order() = default;

int Order::getOrderId() const
{
	return orderId;
}

int Order::getSessionId() const
{
	return sessionId;
}

OrderStatus Order::getStatus() const
{
	return status;
}

const std::string& Order::getDenyCode() const
{
	return denyCode;
}

const DoublyLinkedList<OrderItem>& Order::getItems() const
{
	return items;
}

bool Order::isEmpty() const
{
	return items.isEmpty();
}

std::size_t Order::getItemCount() const
{
	return items.size();
}

std::int64_t Order::getTotal() const
{
	std::int64_t total = 0;
	items.forEach([&total](const OrderItem& item) {
		const std::int64_t price = item.getUnitPrice();
		const std::int64_t quantity = item.getQuantity();
		if (price > 0 &&
			quantity > std::numeric_limits<std::int64_t>::max() / price)
		{
			throw std::overflow_error("Order total exceeds the supported range");
		}

		const std::int64_t lineTotal = price * quantity;
		if (total > std::numeric_limits<std::int64_t>::max() - lineTotal)
		{
			throw std::overflow_error("Order total exceeds the supported range");
		}
		total += lineTotal;
	});
	return total;
}

const OrderItem* Order::findItem(int menuId) const
{
	return items.findIf([menuId](const OrderItem& item) {
		return item.getMenuId() == menuId;
	});
}

bool Order::addItem(int menuId, const std::string& name,
					std::int64_t unitPrice,
					int quantity)
{
	if (status != OrderStatus::DRAFT || menuId <= 0 || name.empty() ||
		unitPrice < 0 || quantity <= 0)
	{
		return false;
	}

	OrderItem* existingItem = items.findIf([menuId](const OrderItem& item) {
		return item.getMenuId() == menuId;
	});
	if (existingItem != nullptr)
	{
		if (quantity > std::numeric_limits<int>::max() -
						   existingItem->getQuantity())
		{
			return false;
		}

		return existingItem->setQuantity(existingItem->getQuantity() + quantity);
	}

	items.pushBack(OrderItem(menuId, name, unitPrice, quantity));
	return true;
}

bool Order::updateQuantity(int menuId, int quantity)
{
	if (status != OrderStatus::DRAFT || quantity <= 0)
	{
		return false;
	}

	OrderItem* item = items.findIf([menuId](const OrderItem& orderItem) {
		return orderItem.getMenuId() == menuId;
	});
	return item != nullptr && item->setQuantity(quantity);
}

bool Order::removeItem(int menuId)
{
	if (status != OrderStatus::DRAFT)
	{
		return false;
	}

	return items.removeIf([menuId](const OrderItem& item) {
		return item.getMenuId() == menuId;
	});
}

bool Order::cancelItemBeforePreparation(int menuId)
{
	if (status != OrderStatus::PROCESSING)
	{
		return false;
	}

	return items.removeIf([menuId](const OrderItem& item) {
		return item.getMenuId() == menuId;
	});
}

bool Order::submit()
{
	if (status != OrderStatus::DRAFT || items.isEmpty())
	{
		return false;
	}

	denyCode.clear();
	status = OrderStatus::PENDING;
	return true;
}

bool Order::approve()
{
	if (status != OrderStatus::PENDING || items.isEmpty())
	{
		return false;
	}

	status = OrderStatus::APPROVED;
	return true;
}

bool Order::reject(const std::string& denyCode)
{
	if (status != OrderStatus::PENDING || denyCode.empty())
	{
		return false;
	}

	this->denyCode = denyCode;
	status = OrderStatus::REJECTED;
	return true;
}

bool Order::startProcessing()
{
	if (status != OrderStatus::APPROVED)
	{
		return false;
	}

	status = OrderStatus::PROCESSING;
	return true;
}

bool Order::complete()
{
	if (status != OrderStatus::PROCESSING)
	{
		return false;
	}

	status = OrderStatus::COMPLETED;
	return true;
}