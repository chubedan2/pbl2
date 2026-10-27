#include "RestaurantSystem.h"

#include <limits>
#include <stdexcept>

RestaurantSystem::PriorityEntry::PriorityEntry(
	int taskId, int priority, unsigned long long sequence)
	: taskId(taskId), priority(priority), sequence(sequence)
{
}

bool RestaurantSystem::PriorityEntryCompare::operator()(
	const PriorityEntry& left, const PriorityEntry& right) const
{
	if (left.priority != right.priority)
	{
		return left.priority < right.priority;
	}
	return left.sequence > right.sequence;
}

RestaurantSystem::RestaurantSystem()
	: menu(), tableCount(0), sessionCount(0), userCount(0), taskCount(0),
	  notificationCount(0), nextSessionId(1), nextOrderId(1), nextTaskId(1),
	  nextBillId(1), nextNotificationId(1), nextAuditId(1),
	  nextQueueSequence(0), kitchenPriorityStreak(0), barPriorityStreak(0),
	  normalKitchen(), normalBar(), priorityKitchen(), priorityBar(),
	  notifications(), auditActions(), orderingPolicy(), inventoryPolicy(),
	  kitchenRoutingPolicy(), paymentPolicy(),
	  defaultRecommendationStrategy(), recommendationPolicy(
		  defaultRecommendationStrategy)
{
}

Menu& RestaurantSystem::getMenu() { return menu; }
const Menu& RestaurantSystem::getMenu() const { return menu; }
std::size_t RestaurantSystem::getTableCount() const { return tableCount; }
std::size_t RestaurantSystem::getSessionCount() const { return sessionCount; }
std::size_t RestaurantSystem::getUserCount() const { return userCount; }
std::size_t RestaurantSystem::getTaskCount() const { return taskCount; }

Table* RestaurantSystem::findTable(const std::string& tableId)
{
	return const_cast<Table*>(
		static_cast<const RestaurantSystem*>(this)->findTable(tableId));
}

const Table* RestaurantSystem::findTable(const std::string& tableId) const
{
	for (std::size_t index = 0; index < tableCount; ++index)
	{
		if (tables[index]->getTableId() == tableId)
		{
			return tables[index].get();
		}
	}
	return nullptr;
}

DiningSession* RestaurantSystem::findSession(int sessionId)
{
	return const_cast<DiningSession*>(
		static_cast<const RestaurantSystem*>(this)->findSession(sessionId));
}

const DiningSession* RestaurantSystem::findSession(int sessionId) const
{
	for (std::size_t index = 0; index < sessionCount; ++index)
	{
		if (sessions[index]->getSessionId() == sessionId)
		{
			return sessions[index].get();
		}
	}
	return nullptr;
}

KitchenTask* RestaurantSystem::findTask(int taskId)
{
	return const_cast<KitchenTask*>(
		static_cast<const RestaurantSystem*>(this)->findTask(taskId));
}

const KitchenTask* RestaurantSystem::findTask(int taskId) const
{
	for (std::size_t index = 0; index < taskCount; ++index)
	{
		if (tasks[index]->getTaskId() == taskId)
		{
			return tasks[index].get();
		}
	}
	return nullptr;
}

int RestaurantSystem::findSessionIndex(int sessionId) const
{
	for (std::size_t index = 0; index < sessionCount; ++index)
	{
		if (sessions[index]->getSessionId() == sessionId)
		{
			return static_cast<int>(index);
		}
	}
	return -1;
}

int RestaurantSystem::findFreeTaskSlot() const
{
	for (std::size_t index = 0; index < MAX_TASKS; ++index)
	{
		if (!tasks[index])
		{
			return static_cast<int>(index);
		}
	}
	return -1;
}

PolicyResult RestaurantSystem::addTable(const std::string& tableId,
										int capacity)
{
	if (tableCount == MAX_TABLES)
	{
		return PolicyResult::deny("TABLE_CAPACITY_REACHED",
								  "The table collection is full.");
	}
	if (tableId.empty() || capacity <= 0 || findTable(tableId) != nullptr)
	{
		return PolicyResult::deny("INVALID_TABLE",
								  "Table data is invalid or duplicated.");
	}
	tables[tableCount].reset(new Table(tableId, capacity));
	++tableCount;
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::addUser(int userId, const std::string& name,
									   UserRole role)
{
	if (userCount == MAX_USERS)
	{
		return PolicyResult::deny("USER_CAPACITY_REACHED",
								  "The user collection is full.");
	}
	const bool validRole =
		role == UserRole::CUSTOMER || role == UserRole::STAFF ||
		role == UserRole::WAITER || role == UserRole::KITCHEN ||
		role == UserRole::BAR || role == UserRole::MANAGER;
	if (userId <= 0 || name.empty() || !validRole ||
		getUser(userId) != nullptr)
	{
		return PolicyResult::deny("INVALID_USER",
								  "User data is invalid or duplicated.");
	}
	users[userCount].reset(new User(userId, name, role));
	++userCount;
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::openTable(const std::string& tableId,
										 int& sessionId)
{
	Table* table = findTable(tableId);
	if (table == nullptr)
	{
		return PolicyResult::deny("TABLE_NOT_FOUND", "Table was not found.");
	}
	if (!table->isAvailable())
	{
		return PolicyResult::deny("TABLE_UNAVAILABLE",
								  "Table is already occupied.");
	}
	if (sessionCount == MAX_SESSIONS)
	{
		return PolicyResult::deny("SESSION_CAPACITY_REACHED",
								  "The session collection is full.");
	}
	if (nextSessionId <= 0 ||
		nextSessionId == std::numeric_limits<int>::max())
	{
		return PolicyResult::deny("SESSION_ID_EXHAUSTED",
								  "No session IDs are available.");
	}

	std::unique_ptr<DiningSession> session(
		new DiningSession(nextSessionId, tableId));
	if (!table->openSession(nextSessionId))
	{
		return PolicyResult::deny("TABLE_UNAVAILABLE",
								  "Table could not be opened.");
	}

	sessionId = nextSessionId++;
	sessions[sessionCount++] = std::move(session);
	recordAudit("system", "OPEN_TABLE", tableId,
				"Opened table and created dining session.");
	return PolicyResult::allow();
}

DiningSession* RestaurantSystem::getSession(int sessionId)
{
	return findSession(sessionId);
}

const DiningSession* RestaurantSystem::getSession(int sessionId) const
{
	return findSession(sessionId);
}

KitchenTask* RestaurantSystem::getTask(int taskId) { return findTask(taskId); }

const KitchenTask* RestaurantSystem::getTask(int taskId) const
{
	return findTask(taskId);
}

const Bill* RestaurantSystem::getBill(int sessionId) const
{
	const int index = findSessionIndex(sessionId);
	return index < 0 ? nullptr : bills[index].get();
}

const User* RestaurantSystem::getUser(int userId) const
{
	for (std::size_t index = 0; index < userCount; ++index)
	{
		if (users[index]->getUserId() == userId)
		{
			return users[index].get();
		}
	}
	return nullptr;
}

Order* RestaurantSystem::createOrder(int sessionId)
{
	DiningSession* session = findSession(sessionId);
	if (session == nullptr || nextOrderId <= 0 ||
		nextOrderId == std::numeric_limits<int>::max())
	{
		return nullptr;
	}
	return session->createOrder(nextOrderId++);
}

PolicyResult RestaurantSystem::addItem(int sessionId, int orderId,
									   int menuId, int quantity)
{
	DiningSession* session = findSession(sessionId);
	if (session == nullptr || !session->isActive())
	{
		return PolicyResult::deny("SESSION_NOT_ACTIVE",
								  "Dining session is not active.");
	}
	Order* order = session->findOrder(orderId);
	if (order == nullptr)
	{
		return PolicyResult::deny("ORDER_NOT_FOUND", "Order was not found.");
	}
	if (quantity <= 0)
	{
		return PolicyResult::deny("INVALID_QUANTITY",
								  "Quantity must be positive.");
	}
	const PolicyResult available = inventoryPolicy.checkItem(menu, menuId);
	if (!available.isSuccess())
	{
		return available;
	}
	const MenuItem* item = menu.findById(menuId);
	if (!order->addItem(item->getId(), item->getName(), item->getPrice(),
						quantity))
	{
		return PolicyResult::deny("ORDER_NOT_EDITABLE",
								  "Order cannot be changed.");
	}
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::submitOrder(int sessionId, int orderId)
{
	DiningSession* session = findSession(sessionId);
	if (session == nullptr)
	{
		return PolicyResult::deny("SESSION_NOT_FOUND",
								  "Dining session was not found.");
	}
	Order* order = session->findOrder(orderId);
	if (order == nullptr)
	{
		return PolicyResult::deny("ORDER_NOT_FOUND", "Order was not found.");
	}
	const PolicyResult validation =
		orderingPolicy.validateSubmission(*session, *order, menu);
	if (!validation.isSuccess())
	{
		return validation;
	}
	if (!order->submit())
	{
		return PolicyResult::deny("ORDER_SUBMISSION_FAILED",
								  "Order could not be submitted.");
	}
	recordAudit("customer", "SUBMIT_ORDER", std::to_string(orderId),
				"Submitted order for review.");
	return PolicyResult::allow();
}

bool RestaurantSystem::enqueueTask(const KitchenTask& task)
{
	if (nextQueueSequence == std::numeric_limits<unsigned long long>::max())
	{
		return false;
	}
	if (task.getStation() == PreparationStation::KITCHEN)
	{
		if (task.isPriority())
		{
			priorityKitchen.push(PriorityEntry(task.getTaskId(),
											   task.getPriority(),
											   nextQueueSequence++));
		}
		else
		{
			normalKitchen.enqueue(task.getTaskId());
		}
	}
	else if (task.isPriority())
	{
		priorityBar.push(PriorityEntry(task.getTaskId(), task.getPriority(),
									   nextQueueSequence++));
	}
	else
	{
		normalBar.enqueue(task.getTaskId());
	}
	return true;
}

PolicyResult RestaurantSystem::approveOrder(int sessionId, int orderId,
											int priority)
{
	DiningSession* session = findSession(sessionId);
	Order* order = session == nullptr ? nullptr : session->findOrder(orderId);
	if (session == nullptr || order == nullptr ||
		order->getStatus() != OrderStatus::PENDING || priority < 0)
	{
		return PolicyResult::deny("ORDER_NOT_PENDING",
								  "Only a pending order can be approved.");
	}
	PolicyResult availability = PolicyResult::allow();
	order->getItems().forEach([&](const OrderItem& item) {
		if (availability.isSuccess())
		{
			availability = inventoryPolicy.checkItem(menu, item.getMenuId());
		}
	});
	if (!availability.isSuccess())
	{
		return availability;
	}
	if (notificationCount == MAX_NOTIFICATIONS ||
		nextNotificationId <= 0 ||
		nextNotificationId == std::numeric_limits<int>::max())
	{
		return PolicyResult::deny("NOTIFICATION_CAPACITY_REACHED",
								  "The notification queue is full.");
	}

	std::vector<KitchenTask> newTasks;
	const PolicyResult routing = kitchenRoutingPolicy.createTasks(
		*order, menu, nextTaskId, newTasks, priority);
	if (!routing.isSuccess())
	{
		return routing;
	}
	std::size_t freeSlots = 0;
	for (std::size_t index = 0; index < MAX_TASKS; ++index)
	{
		if (!tasks[index])
		{
			++freeSlots;
		}
	}
	if (newTasks.size() > freeSlots)
	{
		return PolicyResult::deny("TASK_CAPACITY_REACHED",
								  "The kitchen task collection is full.");
	}
	if (nextTaskId <= 0 ||
		newTasks.size() > static_cast<std::size_t>(
							  std::numeric_limits<int>::max() - nextTaskId))
	{
		return PolicyResult::deny("TASK_ID_EXHAUSTED",
								  "No task IDs are available.");
	}
	if (!order->approve())
	{
		return PolicyResult::deny("ORDER_APPROVAL_FAILED",
								  "Order could not be approved.");
	}
	if (!order->startProcessing())
	{
		return PolicyResult::deny("ORDER_PROCESSING_FAILED",
								  "Order could not enter processing.");
	}

	for (std::size_t index = 0; index < newTasks.size(); ++index)
	{
		const int slot = findFreeTaskSlot();
		if (slot < 0)
		{
			return PolicyResult::deny("TASK_CAPACITY_REACHED",
									  "The kitchen task collection is full.");
		}
		tasks[slot].reset(new KitchenTask(newTasks[index]));
		++taskCount;
		if (!enqueueTask(*tasks[slot]))
		{
			return PolicyResult::deny("TASK_QUEUE_FAILED",
									  "Task could not be queued.");
		}
	}
	nextTaskId += static_cast<int>(newTasks.size());
	addNotification(NotificationType::ORDER_APPROVED, "Staff",
					"Order " + std::to_string(orderId) + " was approved.");
	recordAudit("staff", "APPROVE_ORDER", std::to_string(orderId),
				"Approved order and created kitchen tasks.");
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::rejectOrder(int sessionId, int orderId,
										   const std::string& denyCode)
{
	DiningSession* session = findSession(sessionId);
	Order* order = session == nullptr ? nullptr : session->findOrder(orderId);
	if (order == nullptr || !order->reject(denyCode))
	{
		return PolicyResult::deny("ORDER_REJECTION_FAILED",
								  "Order could not be rejected.");
	}
	addNotification(NotificationType::ORDER_REJECTED, "Customer",
					"Order " + std::to_string(orderId) + " was rejected.");
	recordAudit("staff", "REJECT_ORDER", std::to_string(orderId), denyCode);
	return PolicyResult::allow();
}

KitchenTask* RestaurantSystem::takeNextTask(PreparationStation station)
{
	Queue<int>& normal = station == PreparationStation::KITCHEN
							 ? normalKitchen
							 : normalBar;
	std::priority_queue<PriorityEntry, std::vector<PriorityEntry>,
						PriorityEntryCompare>& priority =
		station == PreparationStation::KITCHEN ? priorityKitchen : priorityBar;
	unsigned int& streak = station == PreparationStation::KITCHEN
							  ? kitchenPriorityStreak
							  : barPriorityStreak;

	int taskId = 0;
	while (!normal.isEmpty() || !priority.empty())
	{
		if (!priority.empty() && (normal.isEmpty() || streak < 3))
		{
			taskId = priority.top().taskId;
			priority.pop();
			++streak;
		}
		else if (!normal.isEmpty())
		{
			if (!normal.dequeue(taskId))
			{
				return nullptr;
			}
			streak = 0;
		}
		else
		{
			taskId = priority.top().taskId;
			priority.pop();
			++streak;
		}

		KitchenTask* task = findTask(taskId);
		if (task != nullptr && task->startPreparing())
		{
			return task;
		}
	}
	return nullptr;
}

KitchenTask* RestaurantSystem::getNextKitchenTask()
{
	return takeNextTask(PreparationStation::KITCHEN);
}

KitchenTask* RestaurantSystem::getNextBarTask()
{
	return takeNextTask(PreparationStation::BAR);
}

bool RestaurantSystem::addNotification(NotificationType type,
									   const std::string& recipient,
									   const std::string& message)
{
	if (notificationCount == MAX_NOTIFICATIONS ||
		nextNotificationId <= 0 ||
		nextNotificationId == std::numeric_limits<int>::max())
	{
		return false;
	}
	std::unique_ptr<Notification> notification(
		new Notification(nextNotificationId++, type, recipient, message));
	notifications.push(std::move(notification));
	++notificationCount;
	return true;
}

PolicyResult RestaurantSystem::markTaskReady(int taskId)
{
	KitchenTask* task = findTask(taskId);
	if (task == nullptr)
	{
		return PolicyResult::deny("TASK_NOT_FOUND", "Kitchen task was not found.");
	}
	if (notificationCount == MAX_NOTIFICATIONS ||
		nextNotificationId <= 0 ||
		nextNotificationId == std::numeric_limits<int>::max())
	{
		return PolicyResult::deny("NOTIFICATION_CAPACITY_REACHED",
								  "The notification queue is full.");
	}
	if (!task->markReady())
	{
		return PolicyResult::deny("INVALID_TASK_TRANSITION",
								  "Task must be preparing before it is ready.");
	}
	addNotification(NotificationType::TASK_READY, "Waiter",
					task->getItemName() + " is ready to serve.");
	recordAudit("kitchen", "TASK_READY", std::to_string(taskId),
				"Kitchen task is ready.");
	return PolicyResult::allow();
}

bool RestaurantSystem::completeOrderIfServed(int orderId)
{
	for (std::size_t index = 0; index < sessionCount; ++index)
	{
		Order* order = sessions[index]->findOrder(orderId);
		if (order == nullptr)
		{
			continue;
		}
		bool hasTask = false;
		bool allFinished = true;
		for (std::size_t taskIndex = 0; taskIndex < MAX_TASKS; ++taskIndex)
		{
			if (tasks[taskIndex] &&
				tasks[taskIndex]->getOrderId() == orderId)
			{
				hasTask = true;
				if (tasks[taskIndex]->getStatus() != KitchenTaskStatus::SERVED &&
					tasks[taskIndex]->getStatus() !=
						KitchenTaskStatus::CANCELLED)
				{
					allFinished = false;
				}
			}
		}
		if (hasTask && allFinished &&
			order->getStatus() == OrderStatus::PROCESSING)
		{
			return order->complete();
		}
	}
	return false;
}

PolicyResult RestaurantSystem::serveTask(int taskId)
{
	KitchenTask* task = findTask(taskId);
	if (task == nullptr)
	{
		return PolicyResult::deny("TASK_NOT_FOUND", "Kitchen task was not found.");
	}
	if (!task->markServed())
	{
		return PolicyResult::deny("INVALID_TASK_TRANSITION",
								  "Only a ready task can be served.");
	}
	completeOrderIfServed(task->getOrderId());
	recordAudit("waiter", "TASK_SERVED", std::to_string(taskId),
				"Kitchen task was delivered to the customer.");
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::cancelTask(int taskId,
										  const std::string& actor)
{
	KitchenTask* task = findTask(taskId);
	if (task == nullptr)
	{
		return PolicyResult::deny("TASK_NOT_FOUND", "Kitchen task was not found.");
	}
	Order* order = nullptr;
	for (std::size_t index = 0; index < sessionCount && order == nullptr;
		 ++index)
	{
		order = sessions[index]->findOrder(task->getOrderId());
	}
	if (order == nullptr || order->getStatus() != OrderStatus::PROCESSING ||
		order->findItem(task->getMenuId()) == nullptr)
	{
		return PolicyResult::deny("ORDER_ITEM_NOT_CANCELLABLE",
								  "The order item cannot be cancelled.");
	}
	if (!task->cancel())
	{
		return PolicyResult::deny("TASK_NOT_CANCELLABLE",
								  "Only a waiting task can be cancelled.");
	}
	if (!order->cancelItemBeforePreparation(task->getMenuId()))
	{
		return PolicyResult::deny("ORDER_ITEM_CANCELLATION_FAILED",
								  "The order item could not be cancelled.");
	}
	completeOrderIfServed(task->getOrderId());
	recordAudit(actor, "CANCEL_TASK", std::to_string(taskId),
				"Cancelled a waiting kitchen task.");
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::callStaff(int sessionId)
{
	const DiningSession* session = findSession(sessionId);
	if (session == nullptr || !session->isActive())
	{
		return PolicyResult::deny("SESSION_NOT_ACTIVE",
								  "Dining session is not active.");
	}
	if (notificationCount == MAX_NOTIFICATIONS ||
		nextNotificationId <= 0 ||
		nextNotificationId == std::numeric_limits<int>::max())
	{
		return PolicyResult::deny("NOTIFICATION_CAPACITY_REACHED",
								  "The notification queue is full.");
	}
	if (!addNotification(NotificationType::CALL_STAFF, "Waiter",
						 "Customer at table " + session->getTableId() +
							 " requested assistance."))
	{
		return PolicyResult::deny("NOTIFICATION_CAPACITY_REACHED",
								  "The notification queue is full.");
	}
	recordAudit("customer", "CALL_STAFF", std::to_string(sessionId),
				"Customer requested staff assistance.");
	return PolicyResult::allow();
}

std::vector<int> RestaurantSystem::recommend(const std::string& category,
											 std::size_t limit) const
{
	return recommendationPolicy.recommend(menu, category, limit);
}

void RestaurantSystem::setRecommendationStrategy(
	const IRecommendationStrategy& strategy)
{
	recommendationPolicy.setStrategy(strategy);
}

PolicyResult RestaurantSystem::requestPayment(int sessionId,
											  PaymentMethod method)
{
	DiningSession* session = findSession(sessionId);
	const int sessionIndex = findSessionIndex(sessionId);
	if (session == nullptr || sessionIndex < 0 || !session->isActive())
	{
		return PolicyResult::deny("SESSION_NOT_ACTIVE",
								  "Dining session is not active.");
	}
	if (bills[sessionIndex])
	{
		return PolicyResult::deny("BILL_ALREADY_EXISTS",
								  "A bill already exists for this session.");
	}
	if (method != PaymentMethod::CASH && method != PaymentMethod::CARD &&
		method != PaymentMethod::OTHER)
	{
		return PolicyResult::deny("INVALID_PAYMENT_METHOD",
								  "Payment method is invalid.");
	}
	if (session->hasUnresolvedOrders())
	{
		return PolicyResult::deny("ORDERS_UNFINISHED",
								  "An order has not finished processing.");
	}

	std::vector<const KitchenTask*> sessionTasks;
	for (std::size_t index = 0; index < MAX_TASKS; ++index)
	{
		if (tasks[index] && session->findOrder(tasks[index]->getOrderId()))
		{
			sessionTasks.push_back(tasks[index].get());
		}
	}
	for (std::size_t index = 0; index < sessionTasks.size(); ++index)
	{
		const KitchenTaskStatus status = sessionTasks[index]->getStatus();
		if (status != KitchenTaskStatus::SERVED &&
			status != KitchenTaskStatus::CANCELLED)
		{
			return PolicyResult::deny("TASKS_UNFINISHED",
									  "A kitchen task is not yet complete.");
		}
	}
	if (nextBillId <= 0 || nextBillId == std::numeric_limits<int>::max())
	{
		return PolicyResult::deny("BILL_ID_EXHAUSTED", "No bill IDs are available.");
	}
	if (!session->requestPayment())
	{
		return PolicyResult::deny("SESSION_NOT_ACTIVE",
								  "Dining session cannot request payment.");
	}

	std::unique_ptr<Bill> bill;
	const PolicyResult created = paymentPolicy.createBill(
		*session, nextBillId, sessionTasks, method, bill);
	if (!created.isSuccess())
	{
		return created;
	}
	if (!session->requestPayment())
	{
		return PolicyResult::deny("SESSION_NOT_ACTIVE",
								  "Dining session cannot request payment.");
	}
	bills[sessionIndex] = std::move(bill);
	++nextBillId;
	recordAudit("customer", "REQUEST_PAYMENT", std::to_string(sessionId),
				"Created final bill for dining session.");
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::closeSession(int sessionId)
{
	DiningSession* session = findSession(sessionId);
	const int sessionIndex = findSessionIndex(sessionId);
	if (session == nullptr || sessionIndex < 0 ||
		!bills[sessionIndex] ||
		bills[sessionIndex]->getStatus() != BillStatus::PAID)
	{
		return PolicyResult::deny("PAYMENT_REQUIRED",
								  "A paid bill is required to close the session.");
	}
	Table* table = findTable(session->getTableId());
	if (table == nullptr || table->getSessionId() != sessionId)
	{
		return PolicyResult::deny("TABLE_SESSION_MISMATCH",
								  "Table and session are inconsistent.");
	}
	if (!table->isOccupied())
	{
		return PolicyResult::deny("TABLE_SESSION_MISMATCH",
								  "Table is not occupied by this session.");
	}
	if (!session->close(true, true, true))
	{
		return PolicyResult::deny("SESSION_NOT_CLOSABLE",
								  "Session still has unfinished work.");
	}
	if (!table->closeSession(sessionId, true, true, true))
	{
		return PolicyResult::deny("TABLE_NOT_CLOSABLE",
								  "Table could not be released.");
	}
	recordAudit("staff", "CLOSE_SESSION", std::to_string(sessionId),
				"Closed paid dining session and released table.");
	return PolicyResult::allow();
}

PolicyResult RestaurantSystem::confirmPayment(int sessionId, bool succeeded)
{
	const int sessionIndex = findSessionIndex(sessionId);
	if (sessionIndex < 0 || !bills[sessionIndex])
	{
		return PolicyResult::deny("BILL_NOT_FOUND", "Bill was not found.");
	}
	Bill* bill = bills[sessionIndex].get();
	if (succeeded &&
		(notificationCount == MAX_NOTIFICATIONS ||
		 nextNotificationId <= 0 ||
		 nextNotificationId == std::numeric_limits<int>::max()))
	{
		return PolicyResult::deny("NOTIFICATION_CAPACITY_REACHED",
								  "The notification queue is full.");
	}
	if (!bill->beginPayment() || !bill->finishPayment(succeeded))
	{
		return PolicyResult::deny("PAYMENT_NOT_PROCESSABLE",
								  "Bill cannot be processed.");
	}
	if (!succeeded)
	{
		recordAudit("cashier", "PAYMENT_FAILED", std::to_string(sessionId),
					"Payment simulation failed; bill remains unpaid.");
		return PolicyResult::deny("PAYMENT_FAILED", "Payment was unsuccessful.");
	}
	addNotification(NotificationType::PAYMENT_SUCCESS, "Customer",
					"Payment completed successfully.");
	recordAudit("cashier", "PAYMENT_SUCCESS", std::to_string(sessionId),
				"Payment was confirmed successfully.");
	return closeSession(sessionId);
}

bool RestaurantSystem::takeNotification(
	std::unique_ptr<Notification>& notification)
{
	if (notifications.empty())
	{
		return false;
	}
	notification = std::move(notifications.front());
	notifications.pop();
	notification->markDelivered();
	--notificationCount;
	return true;
}

void RestaurantSystem::recordAudit(const std::string& actor,
								   const std::string& actionType,
								   const std::string& objectId,
								   const std::string& description)
{
	if (nextAuditId <= 0 ||
		nextAuditId == std::numeric_limits<int>::max())
	{
		throw std::overflow_error("Audit action ID range is exhausted.");
	}
	auditActions.push(
		AuditAction(nextAuditId++, actor, actionType, objectId, description));
}

std::vector<AuditAction> RestaurantSystem::getAuditHistory() const
{
	std::stack<AuditAction> copy(auditActions);
	std::vector<AuditAction> history;
	history.reserve(copy.size());
	while (!copy.empty())
	{
		history.push_back(copy.top());
		copy.pop();
	}
	return history;
}
