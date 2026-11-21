#include "Models/Bill.h"
#include "Models/MenuItem.h"
#include "Models/Order.h"
#include "Models/Table.h"
#include "Repositories/BillRepository.h"
#include "Repositories/DiningSessionRepository.h"
#include "Repositories/MenuItemRepository.h"
#include "Repositories/OrderRepository.h"
#include "Repositories/PaymentRepository.h"
#include "Repositories/TableRepository.h"
#include "Services/BillService.h"
#include "Services/DiningSessionService.h"
#include "Services/MenuService.h"
#include "Services/OrderService.h"
#include "Services/PaymentService.h"
#include "Services/TableService.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

int checks = 0;

void check(bool condition, const char* expression, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(
            std::string("Check failed at line ") + std::to_string(line) +
            ": " + expression);
    }
}

#define CHECK(expression) check((expression), #expression, __LINE__)

template <typename Exception, typename Function>
void checkThrows(Function function, const char* expression, int line) {
    ++checks;
    try {
        function();
    } catch (const Exception&) {
        return;
    }
    throw std::runtime_error(
        std::string("Expected exception at line ") + std::to_string(line) +
        ": " + expression);
}

#define CHECK_THROWS(exceptionType, ...) \
    checkThrows<exceptionType>([&]() { (void)(__VA_ARGS__); }, #__VA_ARGS__, __LINE__)

class TemporaryDirectory {
public:
    TemporaryDirectory() {
        const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() /
                ("pbl2-tests-" + std::to_string(now));
        std::filesystem::create_directories(path_);
    }

    ~TemporaryDirectory() {
        std::error_code error;
        std::filesystem::remove_all(path_, error);
    }

    std::string file(const std::string& name) const {
        return (path_ / name).string();
    }

    std::string path() const {
        return path_.string();
    }

private:
    std::filesystem::path path_;
};

void testDynamicArray() {
    restaurant::DynamicArray<std::string> values;
    CHECK(values.empty());
    for (int i = 0; i < 128; ++i) {
        values.pushBack(std::to_string(i));
    }
    CHECK(values.size() == 128);
    CHECK(values.capacity() >= values.size());
    CHECK(values.at(127) == "127");
    CHECK(values.findIndex([](const std::string& value) {
              return value == "42";
          }) == 42);
    CHECK(values.findIndex([](const std::string& value) {
              return value == "missing";
          }) == restaurant::DynamicArray<std::string>::npos);
    CHECK(values.removeAt(42));
    CHECK(values.at(42) == "43");
    CHECK(!values.removeAt(values.size()));

    restaurant::DynamicArray<std::string> copied(values);
    CHECK(copied.size() == values.size());
    CHECK(copied.at(42) == "43");
    restaurant::DynamicArray<std::string> moved(std::move(copied));
    CHECK(moved.size() == values.size());
    CHECK(copied.empty());
    moved.clear();
    CHECK(moved.empty());
}

void testModelInvariants() {
    restaurant::Table table("table-1", "B01");
    CHECK(table.setStatus(restaurant::TableStatus::Occupied));
    CHECK(!table.deactivate());
    CHECK(!table.setStatus(restaurant::TableStatus::Maintenance));
    CHECK(table.setStatus(restaurant::TableStatus::Available));
    CHECK(table.setStatus(restaurant::TableStatus::Maintenance));
    CHECK(table.setStatus(restaurant::TableStatus::Available));
    CHECK(table.deactivate());
    CHECK(!table.setStatus(restaurant::TableStatus::Occupied));
    CHECK(!table.setCode("   "));

    restaurant::MenuItem menuItem("menu-1", "Pho", 50000);
    CHECK(menuItem.setPrice(60000));
    CHECK(!menuItem.setPrice(-1));
    CHECK(!menuItem.setName("\t "));
    CHECK(menuItem.getPrice() == 60000);

    restaurant::DynamicArray<restaurant::BillItem> emptyItems;
    CHECK_THROWS(std::invalid_argument,
                 restaurant::Bill("empty", "session", 1, std::move(emptyItems)));
}

void testEndToEndServicesAndPersistence() {
    TemporaryDirectory directory;
    const std::string tablesPath = directory.file("tables.txt");
    const std::string menuPath = directory.file("menu.txt");
    const std::string sessionsPath = directory.file("sessions.txt");
    const std::string ordersPath = directory.file("orders.txt");
    const std::string billsPath = directory.file("bills.txt");
    const std::string paymentsPath = directory.file("payments.txt");

    {
        restaurant::TableRepository tableRepository(tablesPath);
        restaurant::MenuItemRepository menuRepository(menuPath);
        restaurant::DiningSessionRepository sessionRepository(sessionsPath);
        restaurant::OrderRepository orderRepository(ordersPath);
        restaurant::BillRepository billRepository(billsPath);
        restaurant::PaymentRepository paymentRepository(paymentsPath);

        restaurant::TableService tableService(tableRepository);
        restaurant::MenuService menuService(menuRepository);
        restaurant::DiningSessionService sessionService(
            tableRepository, sessionRepository, orderRepository);
        restaurant::OrderService orderService(
            orderRepository, menuRepository, sessionRepository);
        restaurant::BillService billService(
            billRepository, orderRepository, sessionRepository);
        restaurant::PaymentService paymentService(
            paymentRepository, billRepository, sessionRepository, tableRepository);

        CHECK(tableService.addTable("table-1", "B01").succeeded);
        CHECK(!tableService.addTable("table-1", "B02").succeeded);
        CHECK(tableService.getAvailableTables().size() == 1);
        CHECK(menuService.addItem("menu-1", "Pho", 50000, "Beef soup").succeeded);
        CHECK(menuService.addItem("menu-2", "Tea", 10000).succeeded);
        CHECK(!menuService.addItem("menu-3", "  ", 10000).succeeded);

        CHECK(!sessionService.openSession("invalid", "table-1", 0, 100).succeeded);
        CHECK(sessionService.openSession("session-1", "table-1", 2, 100).succeeded);
        CHECK(tableRepository.findById("table-1")->getStatus() ==
              restaurant::TableStatus::Occupied);
        CHECK(!sessionService.openSession("session-2", "table-1", 2, 101).succeeded);

        CHECK(!orderService.createOrder("order-bad", "missing", 110).succeeded);
        CHECK(orderService.createOrder("order-1", "session-1", 110).succeeded);
        CHECK(!orderService.addItem("order-1", "item-bad", "missing", 1).succeeded);
        CHECK(!orderService.addItem("order-1", "item-bad", "menu-1", 0).succeeded);
        CHECK(orderService.addItem("order-1", "item-1", "menu-1", 2, "less spicy").succeeded);
        CHECK(orderService.addItem("order-1", "item-2", "menu-2", 1).succeeded);
        CHECK(orderService.getUndeliveredItems("session-1").size() == 2);
        CHECK(!orderService.confirmOrder("missing").succeeded);
        CHECK(orderService.confirmOrder("order-1").succeeded);
        CHECK(!orderService.addItem("order-1", "item-3", "menu-2", 1).succeeded);

        CHECK(!billService.createBill("bill-too-early", "session-1", 120).succeeded);
        CHECK(orderService.markItemServed("order-1", "item-1").succeeded);
        CHECK(!orderService.cancelItem("order-1", "item-1").succeeded);
        CHECK(orderService.cancelItem("order-1", "item-2").succeeded);
        CHECK(orderService.getUndeliveredItems("session-1").empty());

        CHECK(menuService.updateItem("menu-1", "Pho special", 75000, "Updated").succeeded);
        CHECK(billService.createBill("bill-1", "session-1", 130).succeeded);
        CHECK(!billService.createBill("bill-2", "session-1", 131).succeeded);
        const restaurant::Bill* bill = billService.findById("bill-1");
        CHECK(bill != nullptr);
        CHECK(bill->getItemCount() == 1);
        CHECK(bill->getItemAt(0).getItemNameSnapshot() == "Pho");
        CHECK(bill->getItemAt(0).getUnitPriceSnapshot() == 50000);
        CHECK(bill->getTotalAmount() == 100000);
        CHECK(sessionRepository.findById("session-1")->getStatus() ==
              restaurant::DiningSessionStatus::WaitingForPayment);

        CHECK(!paymentService.payBill("payment-bad", "bill-1", 99999,
                                      "Cash", 140).succeeded);
        CHECK(!paymentService.payBill("payment-zero", "bill-1", 0,
                                      "Cash", 140).succeeded);
        CHECK(paymentRepository.getAll().empty());
        CHECK(paymentService.payBill("payment-1", "bill-1", 100000,
                                     "Cash", 140, "ref-1", "paid").succeeded);
        CHECK(!paymentService.payBill("payment-2", "bill-1", 100000,
                                      "Cash", 141).succeeded);
        CHECK(!paymentService.payBill("payment-1", "bill-1", 100000,
                                      "Cash", 141).succeeded);
        CHECK(billRepository.findById("bill-1")->getPaymentStatus() ==
              restaurant::BillPaymentStatus::Paid);
        CHECK(sessionRepository.findById("session-1")->getStatus() ==
              restaurant::DiningSessionStatus::Closed);
        CHECK(tableRepository.findById("table-1")->getStatus() ==
              restaurant::TableStatus::Available);

        CHECK(tableService.addTable("table-2", "B02").succeeded);
        CHECK(sessionService.openSession("session-2", "table-2", 1, 200).succeeded);
        CHECK(orderService.createOrder("order-2", "session-2", 210).succeeded);
        CHECK(orderService.addItem("order-2", "item-3", "menu-1", 1).succeeded);
        CHECK(orderService.removeDraftItem("order-2", "item-3").succeeded);
        CHECK(sessionService.cancelSession("session-2", 220).succeeded);
        CHECK(sessionRepository.findById("session-2")->getStatus() ==
              restaurant::DiningSessionStatus::Cancelled);
        CHECK(tableRepository.findById("table-2")->getStatus() ==
              restaurant::TableStatus::Available);
        CHECK(orderRepository.findById("order-2")->getStatus() ==
              restaurant::OrderStatus::Cancelled);
    }

    {
        restaurant::TableRepository tableRepository(tablesPath);
        restaurant::MenuItemRepository menuRepository(menuPath);
        restaurant::DiningSessionRepository sessionRepository(sessionsPath);
        restaurant::OrderRepository orderRepository(ordersPath);
        restaurant::BillRepository billRepository(billsPath);
        restaurant::PaymentRepository paymentRepository(paymentsPath);

        CHECK(tableRepository.getAll().size() == 2);
        CHECK(menuRepository.findById("menu-1")->getPrice() == 75000);
        CHECK(sessionRepository.findById("session-1")->getStatus() ==
              restaurant::DiningSessionStatus::Closed);
        CHECK(orderRepository.findById("order-1")->getItemAt(1).getStatus() ==
              restaurant::OrderItemStatus::Cancelled);
        CHECK(billRepository.findByDiningSessionId("session-1")->getTotalAmount() ==
              100000);
        CHECK(paymentRepository.findByBillId("bill-1")->getAmount() == 100000);
    }
}

void testRepositoryRejectsMalformedData() {
    TemporaryDirectory directory;
    const std::string path = directory.file("tables.txt");
    {
        std::ofstream output(path.c_str());
        output << "not-a-record-count\n";
    }
    CHECK_THROWS(std::runtime_error, restaurant::TableRepository(path));
}

}  // namespace

int main() {
    try {
        testDynamicArray();
        testModelInvariants();
        testEndToEndServicesAndPersistence();
        testRepositoryRejectsMalformedData();
        std::cout << "All " << checks << " checks passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
