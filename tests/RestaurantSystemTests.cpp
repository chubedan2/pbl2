#include "../RestaurantSystem.h"
#include "../Policies/PopularityRecommendationStrategy.h"

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

static void testIntegratedOrderPaymentFlow()
{
	RestaurantSystem system;
	assert(system.addTable("T1", 4).isSuccess());
	assert(system.getMenu().addItem(
		MenuItem(1, "Soup", 30000, "Food", MenuItemStatus::AVAILABLE,
				 PreparationStation::KITCHEN)));
	assert(system.getMenu().addItem(
		MenuItem(2, "Tea", 12000, "Drink", MenuItemStatus::AVAILABLE,
				 PreparationStation::BAR)));

	int sessionId = 0;
	assert(system.openTable("T1", sessionId).isSuccess());
	assert(sessionId > 0);
	assert(!system.openTable("T1", sessionId).isSuccess());

	Order* order = system.createOrder(sessionId);
	assert(order != nullptr);
	assert(system.addItem(sessionId, order->getOrderId(), 1, 2).isSuccess());
	assert(system.addItem(sessionId, order->getOrderId(), 2, 1).isSuccess());
	assert(system.submitOrder(sessionId, order->getOrderId()).isSuccess());
	assert(system.approveOrder(sessionId, order->getOrderId()).isSuccess());
	assert(order->getStatus() == OrderStatus::PROCESSING);

	KitchenTask* kitchenTask = system.getNextKitchenTask();
	KitchenTask* barTask = system.getNextBarTask();
	assert(kitchenTask != nullptr);
	assert(barTask != nullptr);
	assert(kitchenTask->getStation() == PreparationStation::KITCHEN);
	assert(barTask->getStation() == PreparationStation::BAR);
	assert(system.markTaskReady(kitchenTask->getTaskId()).isSuccess());
	assert(system.markTaskReady(barTask->getTaskId()).isSuccess());
	assert(system.serveTask(kitchenTask->getTaskId()).isSuccess());
	assert(system.serveTask(barTask->getTaskId()).isSuccess());
	assert(order->getStatus() == OrderStatus::COMPLETED);

	assert(system.requestPayment(sessionId, PaymentMethod::CASH).isSuccess());
	const Bill* bill = system.getBill(sessionId);
	assert(bill != nullptr);
	assert(bill->getTotal() == 72000);
	assert(!system.confirmPayment(sessionId, false).isSuccess());
	assert(system.getBill(sessionId)->getStatus() == BillStatus::UNPAID);
	assert(system.getSession(sessionId)->isPaymentPending());
	assert(system.confirmPayment(sessionId, true).isSuccess());
	assert(system.getBill(sessionId)->getStatus() == BillStatus::PAID);
	assert(system.getSession(sessionId)->isClosed());

	int nextSessionId = 0;
	assert(system.openTable("T1", nextSessionId).isSuccess());
	assert(nextSessionId != sessionId);

	std::unique_ptr<Notification> notification;
	assert(system.takeNotification(notification));
	assert(notification != nullptr);
	assert(notification->getStatus() == NotificationStatus::DELIVERED);
	const std::size_t auditCount = system.getAuditHistory().size();
	assert(auditCount > 0);
	assert(system.getAuditHistory().size() == auditCount);
}

static void testRecommendationPolymorphism()
{
	RestaurantSystem system;
	assert(system.getMenu().addItem(MenuItem(1, "Soup", 100, "Food")));
	assert(system.getMenu().addItem(MenuItem(2, "Tea", 50, "Drink")));

	const std::vector<int> categoryResults = system.recommend("Food", 5);
	assert(categoryResults.size() == 1);
	assert(categoryResults[0] == 1);

	std::vector<std::pair<int, std::size_t> > popularity;
	popularity.push_back(std::make_pair(2, 10));
	popularity.push_back(std::make_pair(1, 5));
	PopularityRecommendationStrategy popular(popularity);
	system.setRecommendationStrategy(popular);
	const std::vector<int> popularResults = system.recommend("", 2);
	assert(popularResults.size() == 2);
	assert(popularResults[0] == 2);
	assert(popularResults[1] == 1);
}

static void testPriorityFairnessAndCancellation()
{
	RestaurantSystem system;
	assert(system.addTable("T2", 2).isSuccess());
	assert(system.getMenu().addItem(
		MenuItem(10, "Dish", 5000, "Food", MenuItemStatus::AVAILABLE,
				 PreparationStation::KITCHEN)));
	int sessionId = 0;
	assert(system.openTable("T2", sessionId).isSuccess());

	for (int index = 0; index < 4; ++index)
	{
		Order* order = system.createOrder(sessionId);
		assert(order != nullptr);
		assert(system.addItem(sessionId, order->getOrderId(), 10, 1)
				   .isSuccess());
		assert(system.submitOrder(sessionId, order->getOrderId()).isSuccess());
		assert(system.approveOrder(sessionId, order->getOrderId(), 3)
				   .isSuccess());
	}

	Order* normalOrder = system.createOrder(sessionId);
	assert(normalOrder != nullptr);
	assert(system.addItem(sessionId, normalOrder->getOrderId(), 10, 1)
			   .isSuccess());
	assert(system.submitOrder(sessionId, normalOrder->getOrderId()).isSuccess());
	assert(system.approveOrder(sessionId, normalOrder->getOrderId()).isSuccess());

	Order* cancellableOrder = system.createOrder(sessionId);
	assert(cancellableOrder != nullptr);
	assert(system.addItem(sessionId, cancellableOrder->getOrderId(), 10, 1)
			   .isSuccess());
	assert(system.submitOrder(sessionId, cancellableOrder->getOrderId())
			   .isSuccess());
	assert(system.approveOrder(sessionId, cancellableOrder->getOrderId())
			   .isSuccess());

	for (int index = 0; index < 3; ++index)
	{
		KitchenTask* task = system.getNextKitchenTask();
		assert(task != nullptr);
		assert(task->isPriority());
	}
	KitchenTask* ordinaryTask = system.getNextKitchenTask();
	assert(ordinaryTask != nullptr);
	assert(!ordinaryTask->isPriority());

	assert(system.cancelTask(6, "staff").isSuccess());
	assert(system.getTask(6)->getStatus() == KitchenTaskStatus::CANCELLED);
	KitchenTask* lastPriorityTask = system.getNextKitchenTask();
	assert(lastPriorityTask != nullptr);
	assert(lastPriorityTask->isPriority());
}

int main()
{
	testIntegratedOrderPaymentFlow();
	testRecommendationPolymorphism();
	testPriorityFairnessAndCancellation();
	return 0;
}
