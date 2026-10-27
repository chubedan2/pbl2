#include "../data_structures/Queue.h"

#include <cassert>
#include <string>
#include <utility>

static void testFifoOperations()
{
	Queue<int> values;
	int removed = 0;

	assert(values.isEmpty());
	assert(values.front() == nullptr);
	assert(!values.dequeue(removed));

	values.enqueue(10);
	values.enqueue(20);
	values.enqueue(30);
	assert(values.size() == 3);
	assert(*values.front() == 10);

	assert(values.dequeue(removed));
	assert(removed == 10);
	assert(*values.front() == 20);
	assert(values.dequeue(removed));
	assert(removed == 20);
	assert(values.dequeue(removed));
	assert(removed == 30);
	assert(values.isEmpty());
	assert(values.front() == nullptr);
	assert(!values.dequeue(removed));
}

static void testCopyMoveAndClear()
{
	Queue<std::string> original;
	original.enqueue("first");
	original.enqueue("second");

	Queue<std::string> copied(original);
	assert(copied.size() == 2);
	std::string removed;
	assert(copied.dequeue(removed));
	assert(removed == "first");
	assert(original.size() == 2);

	Queue<std::string> assigned;
	assigned.enqueue("old");
	assigned = original;
	assert(assigned.size() == 2);
	assert(*assigned.front() == "first");

	Queue<std::string> moved(std::move(assigned));
	assert(moved.size() == 2);
	assert(assigned.isEmpty());

	moved.clear();
	assert(moved.isEmpty());
	assert(moved.front() == nullptr);
}

int main()
{
	testFifoOperations();
	testCopyMoveAndClear();
	return 0;
}
