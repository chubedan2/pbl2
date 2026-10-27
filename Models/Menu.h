#ifndef MENU_H
#define MENU_H

#include <cstdint>
#include <string>

#include "MenuItem.h"

class Menu
{
private:
	MenuItem* items;
	int itemCount;
	int capacity;

	int findPosition(int id) const;
	bool ensureCapacity();
	static std::string normalize(const std::string& text);

public:
	Menu();
	~Menu();

	Menu(const Menu&) = delete;
	Menu& operator=(const Menu&) = delete;

	int size() const;
	bool isEmpty() const;
	const MenuItem* getItemAt(int index) const;

	bool addItem(const MenuItem& item);
	bool removeItem(int id);
	bool updateItem(int id, const std::string& newName,
					const std::string& newCategory);
	bool updatePrice(int id, std::int64_t newPrice);
	bool setItemStatus(int id, MenuItemStatus newStatus);
	bool setItemStation(int id, PreparationStation newStation);

	MenuItem* findById(int id);
	const MenuItem* findById(int id) const;
	MenuItem* searchByName(const std::string& name);
	const MenuItem* searchByName(const std::string& name) const;

	void displayAll() const;
	void displayAvailable() const;
};

#endif
