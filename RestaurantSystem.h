#ifndef RESTAURANT_SYSTEM_H
#define RESTAURANT_SYSTEM_H

#include "Models/AuditAction.h"
#include "Models/Bill.h"
#include "Models/DiningSession.h"
#include "Models/KitchenTask.h"
#include "Models/Menu.h"
#include "Models/Notification.h"
#include "Models/Table.h"
#include "Models/User.h"
#include "Policies/CategoryRecommendationStrategy.h"
#include "Policies/InventoryPolicy.h"
#include "Policies/KitchenRoutingPolicy.h"
#include "Policies/OrderingPolicy.h"
#include "Policies/PaymentPolicy.h"
#include "Policies/PolicyResult.h"
#include "Policies/RecommendationPolicy.h"
#include "data_structures/Queue.h"

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

class RestaurantSystem
{
private:
	enum
	{
		MAX_TABLES = 64,
		MAX_SESSIONS = 128,
		MAX_USERS = 128,
		MAX_TASKS = 512,
		MAX_NOTIFICATIONS = 512
	};

	struct PriorityEntry
	{
		int taskId;
		int priority;
		unsigned long long sequence;

		PriorityEntry(int taskId, int priority,
					  unsigned long long sequence);
	};

	struct PriorityEntryCompare
	{
		bool operator()(const PriorityEntry& left,
						const PriorityEntry& right) const;
	};

	Menu menu;
	std::unique_ptr<Table> tables[MAX_TABLES];
	std::unique_ptr<DiningSession> sessions[MAX_SESSIONS];
	std::unique_ptr<User> users[MAX_USERS];
	std::unique_ptr<KitchenTask> tasks[MAX_TASKS];
	std::unique_ptr<Bill> bills[MAX_SESSIONS];
	std::size_t tableCount;
	std::size_t sessionCount;
	std::size_t userCount;
	std::size_t taskCount;
	std::size_t notificationCount;
	int nextSessionId;
	int nextOrderId;
	int nextTaskId;
	int nextBillId;
	int nextNotificationId;
	int nextAuditId;
	unsigned long long nextQueueSequence;
	unsigned int kitchenPriorityStreak;
	unsigned int barPriorityStreak;

	Queue<int> normalKitchen;
	Queue<int> normalBar;
	std::priority_queue<PriorityEntry, std::vector<PriorityEntry>,
						PriorityEntryCompare> priorityKitchen;
	std::priority_queue<PriorityEntry, std::vector<PriorityEntry>,
						PriorityEntryCompare> priorityBar;
	std::queue<std::unique_ptr<Notification> > notifications;
	std::stack<AuditAction> auditActions;

	OrderingPolicy orderingPolicy;
	InventoryPolicy inventoryPolicy;
	KitchenRoutingPolicy kitchenRoutingPolicy;
	PaymentPolicy paymentPolicy;
	CategoryRecommendationStrategy defaultRecommendationStrategy;
	RecommendationPolicy recommendationPolicy;

	Table* findTable(const std::string& tableId);
	const Table* findTable(const std::string& tableId) const;
	DiningSession* findSession(int sessionId);
	const DiningSession* findSession(int sessionId) const;
	KitchenTask* findTask(int taskId);
	const KitchenTask* findTask(int taskId) const;
	int findSessionIndex(int sessionId) const;
	int findFreeTaskSlot() const;
	KitchenTask* takeNextTask(PreparationStation station);
	bool enqueueTask(const KitchenTask& task);
	bool addNotification(NotificationType type, const std::string& recipient,
						 const std::string& message);
	void recordAudit(const std::string& actor, const std::string& actionType,
					 const std::string& objectId,
					 const std::string& description);
	bool completeOrderIfServed(int orderId);

public:
	RestaurantSystem();

	Menu& getMenu();
	const Menu& getMenu() const;
	std::size_t getTableCount() const;
	std::size_t getSessionCount() const;
	std::size_t getUserCount() const;
	std::size_t getTaskCount() const;

	PolicyResult addTable(const std::string& tableId, int capacity);
	PolicyResult addUser(int userId, const std::string& name, UserRole role);
	PolicyResult openTable(const std::string& tableId, int& sessionId);
	PolicyResult closeSession(int sessionId);

	DiningSession* getSession(int sessionId);
	const DiningSession* getSession(int sessionId) const;
	KitchenTask* getTask(int taskId);
	const KitchenTask* getTask(int taskId) const;
	const Bill* getBill(int sessionId) const;
	const User* getUser(int userId) const;

	Order* createOrder(int sessionId);
	PolicyResult addItem(int sessionId, int orderId, int menuId,
						 int quantity);
	PolicyResult submitOrder(int sessionId, int orderId);
	PolicyResult approveOrder(int sessionId, int orderId, int priority = 0);
	PolicyResult rejectOrder(int sessionId, int orderId,
							 const std::string& denyCode);

	KitchenTask* getNextKitchenTask();
	KitchenTask* getNextBarTask();
	PolicyResult markTaskReady(int taskId);
	PolicyResult serveTask(int taskId);
	PolicyResult cancelTask(int taskId, const std::string& actor);
	PolicyResult callStaff(int sessionId);

	std::vector<int> recommend(const std::string& category,
							   std::size_t limit) const;
	void setRecommendationStrategy(const IRecommendationStrategy& strategy);

	PolicyResult requestPayment(int sessionId, PaymentMethod method);
	PolicyResult confirmPayment(int sessionId, bool succeeded);

	bool takeNotification(std::unique_ptr<Notification>& notification);
	std::vector<AuditAction> getAuditHistory() const;
};

#endif
