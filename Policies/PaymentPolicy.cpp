#include "PaymentPolicy.h"

#include <limits>
#include <stdexcept>

PolicyResult PaymentPolicy::createBill(
	const DiningSession& session, int billId,
	const std::vector<const KitchenTask*>& tasks, PaymentMethod method,
	std::unique_ptr<Bill>& bill) const
{
	if (billId <= 0 || (!session.isActive() && !session.isPaymentPending()))
	{
		return PolicyResult::deny("SESSION_NOT_ACTIVE",
								  "Session is not active for payment.");
	}
	if (method != PaymentMethod::CASH && method != PaymentMethod::CARD &&
		method != PaymentMethod::OTHER)
	{
		return PolicyResult::deny("INVALID_PAYMENT_METHOD",
								  "Payment method is invalid.");
	}
	if (session.hasUnresolvedOrders())
	{
		return PolicyResult::deny("ORDERS_UNFINISHED",
								  "An order has not finished processing.");
	}
	for (std::size_t index = 0; index < tasks.size(); ++index)
	{
		const KitchenTask* task = tasks[index];
		if (task == nullptr)
		{
			return PolicyResult::deny("TASK_NOT_FOUND",
									  "A kitchen task record is missing.");
		}
		if (session.findOrder(task->getOrderId()) != nullptr &&
			task->getStatus() != KitchenTaskStatus::SERVED &&
			task->getStatus() != KitchenTaskStatus::CANCELLED)
		{
			return PolicyResult::deny("TASKS_UNFINISHED",
									  "A kitchen task is not yet complete.");
		}
	}

	std::int64_t total = 0;
	for (std::size_t orderIndex = 0;
		 orderIndex < session.getOrderCount(); ++orderIndex)
	{
		const Order* order = session.getOrderAt(orderIndex);
		if (order == nullptr || order->getStatus() == OrderStatus::REJECTED)
		{
			continue;
		}
		if (order->getStatus() != OrderStatus::COMPLETED)
		{
			return PolicyResult::deny("ORDERS_UNFINISHED",
									  "An approved order is not complete.");
		}

		try
		{
			const std::int64_t orderTotal = order->getTotal();
			if (total > std::numeric_limits<std::int64_t>::max() - orderTotal)
			{
				return PolicyResult::deny(
					"TOTAL_OVERFLOW", "Bill total exceeds the supported range.");
			}
			total += orderTotal;
		}
		catch (const std::overflow_error&)
		{
			return PolicyResult::deny("TOTAL_OVERFLOW",
									  "Order total exceeds the supported range.");
		}
	}

	bill.reset(new Bill(billId, session.getSessionId(), total, method));
	return PolicyResult::allow();
}
