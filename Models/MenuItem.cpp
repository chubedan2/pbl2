#include "MenuItem.h"

#include <iostream>
#include <stdexcept>

namespace
{
bool isValidStatus(MenuItemStatus status)
{
	return status == MenuItemStatus::AVAILABLE ||
		   status == MenuItemStatus::SOLD_OUT ||
		   status == MenuItemStatus::DISCONTINUED;
}

bool isValidStation(PreparationStation station)
{
	return station == PreparationStation::KITCHEN ||
		   station == PreparationStation::BAR;
}
}

MenuItem::MenuItem()
	: id(0), name(), price(0), category(),
	  status(MenuItemStatus::DISCONTINUED),
	  station(PreparationStation::KITCHEN)
{
}

MenuItem::MenuItem(int id, const std::string& name,
				   std::int64_t price,
				   const std::string& category, MenuItemStatus status,
				   PreparationStation station)
	: id(id), name(name), price(price), category(category),
	  status(status), station(station)
{
	if (id <= 0 || name.empty() || price < 0 || category.empty() ||
		!isValidStatus(status) || !isValidStation(station))
	{
		throw std::invalid_argument("Invalid menu item data.");
	}
}

int MenuItem::getId() const
{
	return id;
}

const std::string& MenuItem::getName() const
{
	return name;
}

std::int64_t MenuItem::getPrice() const
{
	return price;
}

const std::string& MenuItem::getCategory() const
{
	return category;
}

MenuItemStatus MenuItem::getStatus() const
{
	return status;
}

PreparationStation MenuItem::getStation() const
{
	return station;
}

bool MenuItem::isValid() const
{
	return id > 0 && !name.empty() && price >= 0 &&
		   !category.empty() && isValidStatus(status) &&
		   isValidStation(station);
}

bool MenuItem::isAvailable() const
{
	return isValid() && status == MenuItemStatus::AVAILABLE;
}

bool MenuItem::setName(const std::string& newName)
{
	if (newName.empty())
	{
		return false;
	}

	name = newName;
	return true;
}

bool MenuItem::setPrice(std::int64_t newPrice)
{
	if (newPrice < 0)
	{
		return false;
	}

	price = newPrice;
	return true;
}

bool MenuItem::setCategory(const std::string& newCategory)
{
	if (newCategory.empty())
	{
		return false;
	}

	category = newCategory;
	return true;
}

bool MenuItem::setStatus(MenuItemStatus newStatus)
{
	if (!isValidStatus(newStatus))
	{
		return false;
	}

	status = newStatus;
	return true;
}

bool MenuItem::setStation(PreparationStation newStation)
{
	if (!isValidStation(newStation))
	{
		return false;
	}

	station = newStation;
	return true;
}

void MenuItem::display() const
{
	const char* statusText = "Unknown";
	switch (status)
	{
	case MenuItemStatus::AVAILABLE:
		statusText = "Available";
		break;
	case MenuItemStatus::SOLD_OUT:
		statusText = "Sold out";
		break;
	case MenuItemStatus::DISCONTINUED:
		statusText = "Discontinued";
		break;
	}

	const char* stationText =
		station == PreparationStation::BAR ? "Bar" : "Kitchen";

	std::cout << id << " | " << name << " | " << price << " | "
			  << category << " | " << statusText << " | "
			  << stationText << '\n';
}
