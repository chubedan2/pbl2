#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <cstddef>
#include <utility>

template <typename T>
class DoublyLinkedList
{
private:
	struct Node
	{
		T value;
		Node* previous;
		Node* next;

		Node(const T& item);
		Node(T&& item);
	};

	Node* head;
	Node* tail;
	std::size_t count;

	void copyFrom(const DoublyLinkedList& other);

public:
	DoublyLinkedList();
	~DoublyLinkedList();

	DoublyLinkedList(const DoublyLinkedList& other);
	DoublyLinkedList(DoublyLinkedList&& other);
	DoublyLinkedList& operator=(const DoublyLinkedList& other);
	DoublyLinkedList& operator=(DoublyLinkedList&& other);

	void swap(DoublyLinkedList& other);

	bool isEmpty() const;
	std::size_t size() const;

	const T* front() const;
	T* front();
	const T* back() const;
	T* back();

	bool popFront(T& value);
	bool popBack(T& value);
	void pushFront(const T& value);
	void pushFront(T&& value);
	void pushBack(const T& value);
	void pushBack(T&& value);

	template <typename Predicate>
	T* findIf(Predicate predicate);

	template <typename Predicate>
	const T* findIf(Predicate predicate) const;

	template <typename Predicate>
	bool removeIf(Predicate predicate);

	template <typename Function>
	void forEach(Function function) const;

	void clear();
};

template <typename T>
DoublyLinkedList<T>::Node::Node(const T& item)
	: value(item), previous(nullptr), next(nullptr)
{
}

template <typename T>
DoublyLinkedList<T>::Node::Node(T&& item)
	: value(std::move(item)), previous(nullptr), next(nullptr)
{
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
	: head(nullptr), tail(nullptr), count(0)
{
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	clear();
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other)
	: head(nullptr), tail(nullptr), count(0)
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
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&& other)
	: head(other.head), tail(other.tail), count(other.count)
{
	other.head = nullptr;
	other.tail = nullptr;
	other.count = 0;
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(
	const DoublyLinkedList& other)
{
	if (this != &other)
	{
		DoublyLinkedList copy(other);
		swap(copy);
	}
	return *this;
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(
	DoublyLinkedList&& other)
{
	if (this != &other)
	{
		clear();
		head = other.head;
		tail = other.tail;
		count = other.count;

		other.head = nullptr;
		other.tail = nullptr;
		other.count = 0;
	}
	return *this;
}

template <typename T>
void DoublyLinkedList<T>::copyFrom(const DoublyLinkedList& other)
{
	for (const Node* current = other.head; current != nullptr;
		 current = current->next)
	{
		pushBack(current->value);
	}
}

template <typename T>
void DoublyLinkedList<T>::swap(DoublyLinkedList& other)
{
	using std::swap;
	swap(head, other.head);
	swap(tail, other.tail);
	swap(count, other.count);
}

template <typename T>
bool DoublyLinkedList<T>::isEmpty() const
{
	return count == 0;
}

template <typename T>
std::size_t DoublyLinkedList<T>::size() const
{
	return count;
}

template <typename T>
const T* DoublyLinkedList<T>::front() const
{
	return head == nullptr ? nullptr : &head->value;
}

template <typename T>
T* DoublyLinkedList<T>::front()
{
	return head == nullptr ? nullptr : &head->value;
}

template <typename T>
const T* DoublyLinkedList<T>::back() const
{
	return tail == nullptr ? nullptr : &tail->value;
}

template <typename T>
T* DoublyLinkedList<T>::back()
{
	return tail == nullptr ? nullptr : &tail->value;
}

template <typename T>
bool DoublyLinkedList<T>::popFront(T& value)
{
	if (head == nullptr)
	{
		return false;
	}

	value = head->value;
	Node* removed = head;
	head = head->next;
	if (head != nullptr)
	{
		head->previous = nullptr;
	}
	else
	{
		tail = nullptr;
	}

	delete removed;
	--count;
	return true;
}

template <typename T>
bool DoublyLinkedList<T>::popBack(T& value)
{
	if (tail == nullptr)
	{
		return false;
	}

	value = tail->value;
	Node* removed = tail;
	tail = tail->previous;
	if (tail != nullptr)
	{
		tail->next = nullptr;
	}
	else
	{
		head = nullptr;
	}

	delete removed;
	--count;
	return true;
}

template <typename T>
void DoublyLinkedList<T>::pushFront(const T& value)
{
	Node* node = new Node(value);
	node->next = head;

	if (head != nullptr)
	{
		head->previous = node;
	}
	else
	{
		tail = node;
	}

	head = node;
	++count;
}

template <typename T>
void DoublyLinkedList<T>::pushFront(T&& value)
{
	Node* node = new Node(std::move(value));
	node->next = head;

	if (head != nullptr)
	{
		head->previous = node;
	}
	else
	{
		tail = node;
	}

	head = node;
	++count;
}

template <typename T>
void DoublyLinkedList<T>::pushBack(const T& value)
{
	Node* node = new Node(value);
	node->previous = tail;

	if (tail != nullptr)
	{
		tail->next = node;
	}
	else
	{
		head = node;
	}

	tail = node;
	++count;
}

template <typename T>
void DoublyLinkedList<T>::pushBack(T&& value)
{
	Node* node = new Node(std::move(value));
	node->previous = tail;

	if (tail != nullptr)
	{
		tail->next = node;
	}
	else
	{
		head = node;
	}

	tail = node;
	++count;
}

template <typename T>
template <typename Predicate>
T* DoublyLinkedList<T>::findIf(Predicate predicate)
{
	for (Node* current = head; current != nullptr; current = current->next)
	{
		if (predicate(current->value))
		{
			return &current->value;
		}
	}
	return nullptr;
}

template <typename T>
template <typename Predicate>
const T* DoublyLinkedList<T>::findIf(Predicate predicate) const
{
	for (const Node* current = head; current != nullptr;
		 current = current->next)
	{
		if (predicate(current->value))
		{
			return &current->value;
		}
	}
	return nullptr;
}

template <typename T>
template <typename Predicate>
bool DoublyLinkedList<T>::removeIf(Predicate predicate)
{
	for (Node* current = head; current != nullptr; current = current->next)
	{
		if (predicate(current->value))
		{
			if (current->previous != nullptr)
			{
				current->previous->next = current->next;
			}
			else
			{
				head = current->next;
			}

			if (current->next != nullptr)
			{
				current->next->previous = current->previous;
			}
			else
			{
				tail = current->previous;
			}

			delete current;
			--count;
			return true;
		}
	}
	return false;
}

template <typename T>
template <typename Function>
void DoublyLinkedList<T>::forEach(Function function) const
{
	for (const Node* current = head; current != nullptr; current = current->next)
	{
		function(current->value);
	}
}

template <typename T>
void DoublyLinkedList<T>::clear()
{
	Node* current = head;
	while (current != nullptr)
	{
		Node* next = current->next;
		delete current;
		current = next;
	}

	head = nullptr;
	tail = nullptr;
	count = 0;
}

#endif
