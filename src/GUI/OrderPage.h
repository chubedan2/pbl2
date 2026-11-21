#pragma once

#include "../Services/MenuService.h"
#include "../Services/OrderService.h"
#include "../Services/DiningSessionService.h"
#include "../Services/TableService.h"

#include <QWidget>

#include <functional>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QTableWidget;

namespace restaurant {

class OrderPage : public QWidget {
public:
    OrderPage(OrderService& orders,
              MenuService& menu,
              TableService& tables,
              DiningSessionService& sessions,
              QWidget* parent = nullptr);

    void refresh();
    void setDataChangedCallback(std::function<void()> callback);

private:
    void refreshSessions();
    void refreshOrders();
    void refreshOrderItems();
    void refreshMenuItems();
    void notifyDataChanged();

    OrderService& orders_;
    MenuService& menu_;
    TableService& tables_;
    DiningSessionService& sessions_;
    QComboBox* sessionCombo_;
    QComboBox* orderCombo_;
    QComboBox* menuItemCombo_;
    QSpinBox* quantity_;
    QLineEdit* note_;
    QTableWidget* itemsTable_;
    std::function<void()> dataChangedCallback_;
};

}  // namespace restaurant
