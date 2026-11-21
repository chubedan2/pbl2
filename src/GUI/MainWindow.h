#pragma once

#include "BillingPage.h"
#include "MenuPage.h"
#include "OrderPage.h"
#include "TableSessionPage.h"

#include <QMainWindow>

class QTabWidget;

namespace restaurant {

class MainWindow : public QMainWindow {
public:
    MainWindow(TableService& tables,
               MenuService& menu,
               DiningSessionService& sessions,
               OrderService& orders,
               BillService& bills,
               PaymentService& payments,
               QWidget* parent = nullptr);

private:
    void refreshAll();

    QTabWidget* tabs_;
    TableSessionPage* tableSessionPage_;
    MenuPage* menuPage_;
    OrderPage* orderPage_;
    BillingPage* billingPage_;
};

}  // namespace restaurant
