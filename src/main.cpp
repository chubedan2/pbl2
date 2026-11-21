#include "GUI/MainWindow.h"
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

#include <QApplication>
#include <QMessageBox>

#include <exception>

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);
    application.setApplicationName(QStringLiteral("Restaurant Management"));
    application.setOrganizationName(QStringLiteral("PBL2"));

    try {
        restaurant::TableRepository tables("data/tables.txt");
        restaurant::MenuItemRepository menuItems("data/menu_items.txt");
        restaurant::DiningSessionRepository sessions("data/dining_sessions.txt");
        restaurant::OrderRepository orders("data/orders.txt");
        restaurant::BillRepository bills("data/bills.txt");
        restaurant::PaymentRepository payments("data/payments.txt");

        restaurant::TableService tableService(tables);
        restaurant::MenuService menuService(menuItems);
        restaurant::DiningSessionService sessionService(tables, sessions, orders);
        restaurant::OrderService orderService(orders, menuItems, sessions);
        restaurant::BillService billService(bills, orders, sessions);
        restaurant::PaymentService paymentService(
            payments, bills, sessions, tables);

        restaurant::MainWindow window(tableService, menuService, sessionService,
                                      orderService, billService, paymentService);
        window.show();
        return application.exec();
    } catch (const std::exception& error) {
        QMessageBox::critical(
            nullptr, QStringLiteral("Không thể khởi động"),
            QStringLiteral("Lỗi khởi động ứng dụng:\n%1")
                .arg(QString::fromUtf8(error.what())));
        return 1;
    }
}
