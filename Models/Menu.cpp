#include "Menu.h"

#include <cctype>
#include <limits>
#include <memory>
#include <new>

Menu::Menu() : items(nullptr), itemCount(0), capacity(0)
{
}

Menu::~Menu()
{
	delete[] items;
}

int Menu::size() const
{
	return itemCount;
}

bool Menu::isEmpty() const
{
	return itemCount == 0;
}

const MenuItem* Menu::getItemAt(int index) const
{
	return index >= 0 && index < itemCount ? &items[index] : nullptr;
}

int Menu::findPosition(int id) const
{
	int left = 0;
	int right = itemCount - 1;

	while (left <= right)
	{
		const int middle = left + (right - left) / 2;
		if (items[middle].getId() == id)
		{
			return middle;
		}

		if (items[middle].getId() < id)
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}

	return left;
}

bool Menu::ensureCapacity()
{
	if (itemCount < capacity)
	{
		return true;
	}

	if (capacity > std::numeric_limits<int>::max() / 2)
	{
		return false;
	}

	const int newCapacity = capacity == 0 ? 4 : capacity * 2;
	std::unique_ptr<MenuItem[]> expandedItems(
		new (std::nothrow) MenuItem[newCapacity]);
	if (!expandedItems)
	{
		return false;
	}
	for (int index = 0; index < itemCount; ++index)
	{
		expandedItems[index] = items[index];
	}

	delete[] items;
	items = expandedItems.release();
	capacity = newCapacity;
	return true;
}

std::string Menu::normalize(const std::string& text)
{
	std::string normalized = text;
	for (std::string::size_type index = 0; index < normalized.size(); ++index)
	{
		normalized[index] = static_cast<char>(
			std::tolower(static_cast<unsigned char>(normalized[index])));
	}
	return normalized;
}

bool Menu::addItem(const MenuItem& item)
{
	if (!item.isValid())
	{
		return false;
	}

	const int insertAt = findPosition(item.getId());
	if (insertAt < itemCount &&
		items[insertAt].getId() == item.getId())
	{
		return false;
	}

	if (!ensureCapacity())
	{
		return false;
	}

	for (int index = itemCount; index > insertAt; --index)
	{
		items[index] = items[index - 1];
	}
	items[insertAt] = item;
	++itemCount;
	return true;
}

bool Menu::removeItem(int id)
{
	const int position = findPosition(id);
	if (position >= itemCount || items[position].getId() != id)
	{
		return false;
	}

	for (int index = position; index < itemCount - 1; ++index)
	{
		items[index] = items[index + 1];
	}
	--itemCount;
	return true;
}

bool Menu::updateItem(int id, const std::string& newName,
					  const std::string& newCategory)
{
	MenuItem* item = findById(id);
	if (item == nullptr || newName.empty() || newCategory.empty())
	{
		return false;
	}

	return item->setName(newName) && item->setCategory(newCategory);
}

bool Menu::updatePrice(int id, std::int64_t newPrice)
{
	MenuItem* item = findById(id);
	return item != nullptr && item->setPrice(newPrice);
}

bool Menu::setItemStatus(int id, MenuItemStatus newStatus)
{
	MenuItem* item = findById(id);
	if (item == nullptr)
	{
		return false;
	}

	return item->setStatus(newStatus);
}

bool Menu::setItemStation(int id, PreparationStation newStation)
{
	MenuItem* item = findById(id);
	return item != nullptr && item->setStation(newStation);
}

MenuItem* Menu::findById(int id)
{
	return const_cast<MenuItem*>(
		static_cast<const Menu*>(this)->findById(id));
}

const MenuItem* Menu::findById(int id) const
{
	const int position = findPosition(id);
	if (position >= itemCount || items[position].getId() != id)
	{
		return nullptr;
	}
	return &items[position];
}

MenuItem* Menu::searchByName(const std::string& name)
{
	return const_cast<MenuItem*>(
		static_cast<const Menu*>(this)->searchByName(name));
}

const MenuItem* Menu::searchByName(const std::string& name) const
{
	if (name.empty())
	{
		return nullptr;
	}

	const std::string target = normalize(name);
	for (int index = 0; index < itemCount; ++index)
	{
		if (normalize(items[index].getName()).find(target) != std::string::npos)
		{
			return &items[index];
		}
	}
	return nullptr;
}

void Menu::displayAll() const
{
	for (int index = 0; index < itemCount; ++index)
	{
		items[index].display();
	}
}

void Menu::displayAvailable() const
{
	for (int index = 0; index < itemCount; ++index)
	{
		if (items[index].isAvailable())
		{
			items[index].display();
		}
	}
}
