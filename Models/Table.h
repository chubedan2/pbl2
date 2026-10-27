#ifndef TABLE_H
#define TABLE_H

#include <string>

enum class TableStatus
{
	AVAILABLE,
	OCCUPIED
};

class Table
{
private:
	std::string tableId;
	int capacity;
	int sessionId;
	TableStatus status;

public:
	explicit Table(const std::string& tableId, int capacity = 4);

	const std::string& getTableId() const;
	int getCapacity() const;
	int getSessionId() const;
	TableStatus getStatus() const;

	bool isAvailable() const;
	bool isOccupied() const;

	bool openSession(int sessionId);
	bool closeSession(int sessionId, bool billPaid,
					  bool noPendingOrders, bool noUnfinishedTasks);
};

#endif
