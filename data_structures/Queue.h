#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include <utility>

template <typename T>
class Queue
{
private:
	struct Node
	{
		T value;
		Node* next;

		Node(const T& item);
		Node(T&& item);
	};

	Node* frontNode;
	Node* rearNode;
	std::size_t count;

	void copyFrom(const Queue& other);

public:
	Queue();
	~Queue();

	Queue(const Queue& other);
	Queue(Queue&& other);
	Queue& operator=(const Queue& other);
	Queue& operator=(Queue&& other);

	void swap(Queue& other);

	bool isEmpty() const;
	std::size_t size() const;

	const T* front() const;
	T* front();

	void enqueue(const T& value);
	void enqueue(T&& value);
	bool dequeue(T& value);
	void clear();
};

template <typename T>
Queue<T>::Node::Node(const T& item) : value(item), next(nullptr)
{
}

template <typename T>
Queue<T>::Node::Node(T&& item) : value(std::move(item)), next(nullptr)
{
}

template <typename T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr), count(0)
{
}

template <typename T>
Queue<T>::~Queue()
{
	clear();
}

template <typename T>
Queue<T>::Queue(const Queue& other)
	: frontNode(nullptr), rearNode(nullptr), count(0)
{
	try
	{
		copyFrom(other);
	}
	catch (...)
	{
		clear();
		throw;
	}
}

template <typename T>
Queue<T>::Queue(Queue&& other)
	: frontNode(other.frontNode), rearNode(other.rearNode), count(other.count)
{
	other.frontNode = nullptr;
	other.rearNode = nullptr;
	other.count = 0;
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& other)
{
	if (this != &other)
	{
		Queue copy(other);
		swap(copy);
	}
	return *this;
}

template <typename T>
Queue<T>& Queue<T>::operator=(Queue&& other)
{
	if (this != &other)
	{
		clear();
		frontNode = other.frontNode;
		rearNode = other.rearNode;
		count = other.count;

		other.frontNode = nullptr;
		other.rearNode = nullptr;
		other.count = 0;
	}
	return *this;
}

template <typename T>
void Queue<T>::copyFrom(const Queue& other)
{
	for (const Node* current = other.frontNode; current != nullptr;
		 current = current->next)
	{
		enqueue(current->value);
	}
}

template <typename T>
void Queue<T>::swap(Queue& other)
{
	using std::swap;
	swap(frontNode, other.frontNode);
	swap(rearNode, other.rearNode);
	swap(count, other.count);
}

template <typename T>
bool Queue<T>::isEmpty() const
{
	return count == 0;
}

template <typename T>
std::size_t Queue<T>::size() const
{
	return count;
}

template <typename T>
const T* Queue<T>::front() const
{
	return frontNode == nullptr ? nullptr : &frontNode->value;
}

template <typename T>
T* Queue<T>::front()
{
	return frontNode == nullptr ? nullptr : &frontNode->value;
}

template <typename T>
void Queue<T>::enqueue(const T& value)
{
	Node* node = new Node(value);
	if (rearNode != nullptr)
	{
		rearNode->next = node;
	}
	else
	{
		frontNode = node;
	}

	rearNode = node;
	++count;
}

template <typename T>
void Queue<T>::enqueue(T&& value)
{
	Node* node = new Node(std::move(value));
	if (rearNode != nullptr)
	{
		rearNode->next = node;
	}
	else
	{
		frontNode = node;
	}

	rearNode = node;
	++count;
}

template <typename T>
bool Queue<T>::dequeue(T& value)
{
	if (frontNode == nullptr)
	{
		return false;
	}

	value = std::move(frontNode->value);
	Node* removed = frontNode;
	frontNode = frontNode->next;
	if (frontNode == nullptr)
	{
		rearNode = nullptr;
	}

	delete removed;
	--count;
	return true;
}

template <typename T>
void Queue<T>::clear()
{
	Node* current = frontNode;
	while (current != nullptr)
	{
		Node* next = current->next;
		delete current;
		current = next;
	}

	frontNode = nullptr;
	rearNode = nullptr;
	count = 0;
}

#endif
