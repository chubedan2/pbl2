#ifndef ORDER_H
#define ORDER_H

#include <cstdint>
#include <string>

#include "../data_structures/DoublyLinkedList.h"

class OrderItem
{
private:
	int menuId;
	std::string name;
	std::int64_t unitPrice;
	int quantity;

public:
	OrderItem(int menuId, const std::string& name,
			  std::int64_t unitPrice, int quantity);

	int getMenuId() const;
	const std::string& getName() const;
	std::int64_t getUnitPrice() const;
	int getQuantity() const;
	bool setQuantity(int newQuantity);
};

enum class OrderStatus
{
	DRAFT,
	PENDING,
	APPROVED,
	REJECTED,
	PROCESSING,
	COMPLETED
};

class Order
{
	friend class RestaurantSystem;

private:
	int orderId;
	int sessionId;
	DoublyLinkedList<OrderItem> items;
	OrderStatus status;
	std::string denyCode;

	bool cancelItemBeforePreparation(int menuId);
	bool complete();

public:
	Order(int orderId, int sessionId);
	~Order();

	Order(const Order&) = delete;
	Order& operator=(const Order&) = delete;

	int getOrderId() const;
	int getSessionId() const;
	OrderStatus getStatus() const;
	const std::string& getDenyCode() const;
	const DoublyLinkedList<OrderItem>& getItems() const;

	bool isEmpty() const;
	std::size_t getItemCount() const;
	std::int64_t getTotal() const;
	const OrderItem* findItem(int menuId) const;

	bool addItem(int menuId, const std::string& name,
				 std::int64_t unitPrice,
				 int quantity);
	bool updateQuantity(int menuId, int quantity);
	bool removeItem(int menuId);

	bool submit();
	bool approve();
	bool reject(const std::string& denyCode);
	bool startProcessing();
};

#endif
