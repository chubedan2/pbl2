#include "../Models/DiningSession.h"
#include "../Models/Menu.h"
#include "../Models/MenuItem.h"
#include "../Models/Order.h"
#include "../Models/Table.h"

#include <cassert>
#include <stdexcept>

static void testMenuItemAndMenu()
{
	MenuItem unavailable;
	assert(!unavailable.isValid());
	assert(!unavailable.isAvailable());

	bool invalidItemRejected = false;
	try
	{
		MenuItem invalid(0, "Invalid", 100, "Food");
	}
	catch (const std::invalid_argument&)
	{
		invalidItemRejected = true;
	}
	assert(invalidItemRejected);

	MenuItem noodles(20, "Noodles", 45000, "Food",
					 MenuItemStatus::AVAILABLE,
					 PreparationStation::KITCHEN);
	MenuItem tea(10, "Tea", 15000, "Drink",
				 MenuItemStatus::AVAILABLE, PreparationStation::BAR);
	Menu menu;
	assert(menu.addItem(noodles));
	assert(menu.addItem(tea));
	assert(!menu.addItem(tea));
	assert(menu.size() == 2);
	for (int code = 30; code < 36; ++code)
	{
		assert(menu.addItem(MenuItem(code, "Extra", 1000, "Other")));
	}
	assert(menu.size() == 8);
	assert(menu.findById(35) != nullptr);
	assert(menu.findById(10)->getName() == "Tea");
	assert(menu.searchByName("NOOD")->getId() == 20);
	assert(menu.findById(10)->getStation() == PreparationStation::BAR);
	assert(!menu.setItemStatus(
		10, static_cast<MenuItemStatus>(-1)));
	assert(!menu.setItemStation(
		10, static_cast<PreparationStation>(-1)));
	assert(menu.updateItem(10, "Iced Tea", "Drink"));
	assert(menu.updatePrice(10, 18000));
	assert(menu.setItemStation(10, PreparationStation::KITCHEN));
	assert(menu.findById(10)->getStation() ==
		   PreparationStation::KITCHEN);
	assert(menu.removeItem(10));
	assert(menu.findById(10) == nullptr);
}

static void testTable()
{
	bool invalidCapacityRejected = false;
	try
	{
		Table invalid("T1", 0);
	}
	catch (const std::invalid_argument&)
	{
		invalidCapacityRejected = true;
	}
	assert(invalidCapacityRejected);

	Table table("T1", 6);
	assert(table.getCapacity() == 6);
	assert(table.isAvailable());
	assert(!table.openSession(0));
	assert(table.openSession(1));
	assert(!table.openSession(2));
	assert(!table.closeSession(2, true, true, true));
	assert(!table.closeSession(1, false, true, true));
	assert(table.closeSession(1, true, true, true));
	assert(table.isAvailable());
}

static void testDiningSessionAndOrders()
{
	DiningSession session(1, "T1");
	assert(session.isActive());
	assert(session.getOrderCount() == 0);

	Order* first = session.createOrder(100);
	Order* second = session.createOrder(101);
	Order* third = session.createOrder(102);
	assert(first != nullptr);
	assert(second != nullptr);
	assert(third != nullptr);
	assert(session.getOrderCount() == 3);
	assert(session.findOrder(100) == first);
	assert(session.createOrder(100) == nullptr);
	assert(session.createOrder(102) == nullptr);
	assert(session.hasUnresolvedOrders());

	assert(first->addItem(10, "Tea", 15000, 1));
	assert(first->submit());
	assert(first->approve());
	assert(first->startProcessing());
	assert(!session.requestPayment());
	assert(session.createOrder(103) != nullptr);

	assert(!second->submit());
	assert(second->addItem(20, "Noodles", 45000, 1));
	assert(second->submit());
	assert(second->reject("OUT_OF_STOCK"));
	assert(third->addItem(30, "Extra", 1000, 1));
	assert(third->submit());
	assert(third->reject("CANCELLED"));
	assert(!session.hasUnresolvedOrders());
	assert(session.close(true, true, true));
	assert(session.isClosed());
	assert(session.getClosedAt() != 0);
}

static void testOrderValidation()
{
	bool invalidOrderRejected = false;
	try
	{
		Order invalid(0, 1);
	}
	catch (const std::invalid_argument&)
	{
		invalidOrderRejected = true;
	}
	assert(invalidOrderRejected);

	Order order(200, 1);
	assert(!order.approve());
	assert(!order.submit());
	assert(order.addItem(1, "Soup", 20000, 1));
	assert(order.submit());
	assert(!order.addItem(2, "Tea", 10000, 1));
	assert(order.approve());
	assert(order.startProcessing());
}

int main()
{
	testMenuItemAndMenu();
	testTable();
	testDiningSessionAndOrders();
	testOrderValidation();
	return 0;
}
