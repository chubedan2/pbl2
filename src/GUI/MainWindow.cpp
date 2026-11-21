#include "MainWindow.h"

#include <QTabWidget>

namespace restaurant {

MainWindow::MainWindow(TableService& tables,
                       MenuService& menu,
                       DiningSessionService& sessions,
                       OrderService& orders,
                       BillService& bills,
                       PaymentService& payments,
                       QWidget* parent)
    : QMainWindow(parent),
      tabs_(new QTabWidget(this)),
      tableSessionPage_(new TableSessionPage(tables, sessions, tabs_)),
      menuPage_(new MenuPage(menu, tabs_)),
      orderPage_(new OrderPage(orders, menu, tables, sessions, tabs_)),
      billingPage_(new BillingPage(bills, payments, tables, sessions, tabs_)) {
    setWindowTitle(QStringLiteral("Quản lý nhà hàng"));
    resize(1100, 720);
    setCentralWidget(tabs_);

    tabs_->addTab(tableSessionPage_, QStringLiteral("Bàn & phiên ăn"));
    tabs_->addTab(menuPage_, QStringLiteral("Thực đơn"));
    tabs_->addTab(orderPage_, QStringLiteral("Gọi món"));
    tabs_->addTab(billingPage_, QStringLiteral("Hóa đơn & thanh toán"));

    tableSessionPage_->setDataChangedCallback([this]() { refreshAll(); });
    menuPage_->setDataChangedCallback([this]() { refreshAll(); });
    orderPage_->setDataChangedCallback([this]() { refreshAll(); });
    billingPage_->setDataChangedCallback([this]() { refreshAll(); });
    connect(tabs_, &QTabWidget::currentChanged, this,
            [this](int) { refreshAll(); });
    refreshAll();
}

void MainWindow::refreshAll() {
    tableSessionPage_->refresh();
    menuPage_->refresh();
    orderPage_->refresh();
    billingPage_->refresh();
}

}  // namespace restaurant
