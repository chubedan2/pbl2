#ifndef DINING_SESSION_H
#define DINING_SESSION_H

#include <ctime>
#include <cstddef>
#include <memory>
#include <string>

#include "Order.h"

enum class DiningSessionStatus
{
	ACTIVE,
	PAYMENT_PENDING,
	CLOSED
};

class DiningSession
{
private:
	int sessionId;
	std::string tableId;
	DiningSessionStatus status;
	std::time_t createdAt;
	std::time_t closedAt;
	std::unique_ptr<std::unique_ptr<Order>[]> orders;
	std::size_t orderCount;
	std::size_t orderCapacity;

	bool ensureOrderCapacity();

public:
	DiningSession(int sessionId, const std::string& tableId);
	~DiningSession();

	DiningSession(const DiningSession&) = delete;
	DiningSession& operator=(const DiningSession&) = delete;

	int getSessionId() const;
	const std::string& getTableId() const;
	DiningSessionStatus getStatus() const;
	std::time_t getCreatedAt() const;
	std::time_t getClosedAt() const;
	std::size_t getOrderCount() const;
	Order* getOrderAt(std::size_t index);
	const Order* getOrderAt(std::size_t index) const;
	Order* createOrder(int orderId);
	Order* findOrder(int orderId);
	const Order* findOrder(int orderId) const;
	bool hasUnresolvedOrders() const;

	bool isActive() const;
	bool isPaymentPending() const;
	bool isClosed() const;

	bool requestPayment();
	bool close(bool billPaid, bool noPendingOrders,
			   bool noUnfinishedTasks);
};

#endif
