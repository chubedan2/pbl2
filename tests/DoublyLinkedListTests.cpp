#include "../data_structures/DoublyLinkedList.h"
#include "../Models/Order.h"

#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

struct TestValue
{
	int key;

	explicit TestValue(int value = 0) : key(value)
	{
	}
};

static bool contains(const DoublyLinkedList<TestValue>& values, int key)
{
	return values.findIf([key](const TestValue& value) {
		return value.key == key;
	}) != nullptr;
}

static void testListOperations()
{
	DoublyLinkedList<TestValue> values;
	TestValue removed;

	assert(values.isEmpty());
	assert(values.popFront(removed) == false);
	assert(values.popBack(removed) == false);

	values.pushBack(TestValue(2));
	values.pushFront(TestValue(1));
	values.pushBack(TestValue(3));
	assert(values.size() == 3);
	assert(values.front()->key == 1);
	assert(values.back()->key == 3);
	assert(contains(values, 2));

	assert(values.removeIf([](const TestValue& value) {
		return value.key == 2;
	}));
	assert(values.size() == 2);
	assert(!contains(values, 2));

	assert(values.popFront(removed));
	assert(removed.key == 1);
	assert(values.popBack(removed));
	assert(removed.key == 3);
	assert(values.isEmpty());
	assert(values.front() == nullptr);
	assert(values.back() == nullptr);
}

static void testCopyAndMove()
{
	DoublyLinkedList<TestValue> original;
	original.pushBack(TestValue(10));
	original.pushBack(TestValue(20));

	DoublyLinkedList<TestValue> copied(original);
	assert(copied.size() == 2);
	assert(copied.removeIf([](const TestValue& value) {
		return value.key == 10;
	}));
	assert(original.size() == 2);

	DoublyLinkedList<TestValue> assigned;
	assigned.pushBack(TestValue(99));
	assigned = original;
	assert(assigned.size() == 2);
	assert(original.size() == 2);

	DoublyLinkedList<TestValue> moved(std::move(assigned));
	assert(moved.size() == 2);
	assert(assigned.isEmpty());
}

static void testOrderIntegration()
{
	Order order(1, 10);
	assert(order.addItem(100, "Soup", 25000, 2));
	assert(order.addItem(100, "Soup", 25000, 1));
	assert(order.getItemCount() == 1);
	assert(order.findItem(100)->getQuantity() == 3);
	assert(order.getTotal() == 75000);
	assert(order.submit());
	assert(!order.addItem(200, "Tea", 10000, 1));

	Order overflowOrder(2, 10);
	assert(overflowOrder.addItem(
		200, "Large", std::numeric_limits<std::int64_t>::max(), 2));
	bool overflowDetected = false;
	try
	{
		overflowOrder.getTotal();
	}
	catch (const std::overflow_error&)
	{
		overflowDetected = true;
	}
	assert(overflowDetected);
}

int main()
{
	testListOperations();
	testCopyAndMove();
	testOrderIntegration();
	return 0;
}
