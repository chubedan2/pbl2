#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <cstdint>
#include <string>

enum class MenuItemStatus
{
	AVAILABLE,
	SOLD_OUT,
	DISCONTINUED
};

enum class PreparationStation
{
	KITCHEN,
	BAR
};

class MenuItem
{
private:
	int id;
	std::string name;
	std::int64_t price;
	std::string category;
	MenuItemStatus status;
	PreparationStation station;

public:
	MenuItem();
	MenuItem(int id, const std::string& name, std::int64_t price,
			 const std::string& category,
			 MenuItemStatus status = MenuItemStatus::AVAILABLE,
			 PreparationStation station = PreparationStation::KITCHEN);

	int getId() const;
	const std::string& getName() const;
	std::int64_t getPrice() const;
	const std::string& getCategory() const;
	MenuItemStatus getStatus() const;
	PreparationStation getStation() const;
	bool isValid() const;
	bool isAvailable() const;

	bool setName(const std::string& newName);
	bool setPrice(std::int64_t newPrice);
	bool setCategory(const std::string& newCategory);
	bool setStatus(MenuItemStatus newStatus);
	bool setStation(PreparationStation newStation);

	void display() const;
};

#endif
