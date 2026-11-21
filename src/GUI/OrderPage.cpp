#include "OrderPage.h"

#include "UiHelpers.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>

#include <ctime>
#include <string>
#include <utility>

namespace restaurant {

OrderPage::OrderPage(OrderService& orders,
                     MenuService& menu,
                     TableService& tables,
                     DiningSessionService& sessions,
                     QWidget* parent)
    : QWidget(parent),
      orders_(orders),
      menu_(menu),
      tables_(tables),
      sessions_(sessions),
      sessionCombo_(nullptr),
      orderCombo_(nullptr),
      menuItemCombo_(nullptr),
      quantity_(nullptr),
      note_(nullptr),
      itemsTable_(nullptr) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* sessionRow = new QHBoxLayout;
    sessionRow->addWidget(new QLabel(QStringLiteral("Phiên đang hoạt động:")));
    sessionCombo_ = new QComboBox;
    sessionRow->addWidget(sessionCombo_, 1);
    QPushButton* createOrder = new QPushButton(QStringLiteral("Tạo Order"));
    sessionRow->addWidget(createOrder);
    layout->addLayout(sessionRow);

    QHBoxLayout* orderRow = new QHBoxLayout;
    orderRow->addWidget(new QLabel(QStringLiteral("Order:")));
    orderCombo_ = new QComboBox;
    orderRow->addWidget(orderCombo_, 1);
    QPushButton* confirmOrder = new QPushButton(QStringLiteral("Xác nhận Order"));
    QPushButton* cancelOrder = new QPushButton(QStringLiteral("Hủy Order"));
    orderRow->addWidget(confirmOrder);
    orderRow->addWidget(cancelOrder);
    layout->addLayout(orderRow);

    QHBoxLayout* addRow = new QHBoxLayout;
    menuItemCombo_ = new QComboBox;
    quantity_ = new QSpinBox;
    quantity_->setRange(1, 10000);
    note_ = new QLineEdit;
    note_->setPlaceholderText(QStringLiteral("Ghi chú món"));
    QPushButton* addItem = new QPushButton(QStringLiteral("Thêm món vào Order"));
    addRow->addWidget(menuItemCombo_, 2);
    addRow->addWidget(new QLabel(QStringLiteral("SL:")));
    addRow->addWidget(quantity_);
    addRow->addWidget(note_, 2);
    addRow->addWidget(addItem);
    layout->addLayout(addRow);

    itemsTable_ = ui::createTable({QStringLiteral("ID món gọi"),
                                   QStringLiteral("Tên món"), QStringLiteral("SL"),
                                   QStringLiteral("Đơn giá"), QStringLiteral("Ghi chú"),
                                   QStringLiteral("Trạng thái")});
    layout->addWidget(itemsTable_);

    QHBoxLayout* itemActions = new QHBoxLayout;
    QPushButton* served = new QPushButton(QStringLiteral("Đánh dấu đã phục vụ"));
    QPushButton* cancelItem = new QPushButton(QStringLiteral("Hủy món chưa phục vụ"));
    QPushButton* removeDraftItem = new QPushButton(QStringLiteral("Bỏ món khỏi Draft"));
    itemActions->addWidget(served);
    itemActions->addWidget(cancelItem);
    itemActions->addWidget(removeDraftItem);
    itemActions->addStretch();
    layout->addLayout(itemActions);

    connect(sessionCombo_, &QComboBox::currentIndexChanged, this,
            [this](int) { refreshOrders(); });
    connect(orderCombo_, &QComboBox::currentIndexChanged, this,
            [this](int) { refreshOrderItems(); });
    connect(createOrder, &QPushButton::clicked, this, [this]() {
        const std::string sessionId = sessionCombo_->currentData().toString().toStdString();
        if (sessionId.empty()) {
            ui::showResult(this, ServiceResult::failure(
                                     "Không có phiên ăn đang hoạt động."));
            return;
        }
        ui::showResult(this, orders_.createOrder(
                                 ui::newId("order"), sessionId, std::time(nullptr)));
        notifyDataChanged();
    });
    connect(addItem, &QPushButton::clicked, this, [this]() {
        const std::string orderId = orderCombo_->currentData().toString().toStdString();
        const std::string menuId = menuItemCombo_->currentData().toString().toStdString();
        if (orderId.empty() || menuId.empty()) {
            ui::showResult(this, ServiceResult::failure(
                                     "Chọn Order và món trong thực đơn trước."));
            return;
        }
        const ServiceResult result = orders_.addItem(
            orderId, ui::newId("order-item"), menuId, quantity_->value(),
            note_->text().toStdString());
        ui::showResult(this, result);
        if (result.succeeded) {
            note_->clear();
            notifyDataChanged();
        }
    });
    connect(confirmOrder, &QPushButton::clicked, this, [this]() {
        ui::showResult(this, orders_.confirmOrder(
                                 orderCombo_->currentData().toString().toStdString()));
        notifyDataChanged();
    });
    connect(cancelOrder, &QPushButton::clicked, this, [this]() {
        ui::showResult(this, orders_.cancelOrder(
                                 orderCombo_->currentData().toString().toStdString()));
        notifyDataChanged();
    });
    connect(served, &QPushButton::clicked, this, [this]() {
        ui::showResult(this, orders_.markItemServed(
                                 orderCombo_->currentData().toString().toStdString(),
                                 ui::selectedId(itemsTable_).toStdString()));
        notifyDataChanged();
    });
    connect(cancelItem, &QPushButton::clicked, this, [this]() {
        ui::showResult(this, orders_.cancelItem(
                                 orderCombo_->currentData().toString().toStdString(),
                                 ui::selectedId(itemsTable_).toStdString()));
        notifyDataChanged();
    });
    connect(removeDraftItem, &QPushButton::clicked, this, [this]() {
        ui::showResult(this, orders_.removeDraftItem(
                                 orderCombo_->currentData().toString().toStdString(),
                                 ui::selectedId(itemsTable_).toStdString()));
        notifyDataChanged();
    });
}

void OrderPage::refresh() {
    refreshSessions();
    refreshOrders();
    refreshMenuItems();
}

void OrderPage::setDataChangedCallback(std::function<void()> callback) {
    dataChangedCallback_ = std::move(callback);
}

void OrderPage::notifyDataChanged() {
    if (dataChangedCallback_) {
        dataChangedCallback_();
    } else {
        refresh();
    }
}

void OrderPage::refreshSessions() {
    const QString selected = sessionCombo_->currentData().toString();
    sessionCombo_->clear();
    const DynamicArray<DiningSession>& all = sessions_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].getStatus() != DiningSessionStatus::Active) continue;
        const Table* table = tables_.findById(all[i].getTableId());
        const QString label = (table ? ui::toQString(table->getCode())
                                     : ui::toQString(all[i].getTableId())) +
                              QStringLiteral(" — ") + ui::toQString(all[i].getId());
        sessionCombo_->addItem(label, ui::toQString(all[i].getId()));
    }
    const int index = sessionCombo_->findData(selected);
    if (index >= 0) sessionCombo_->setCurrentIndex(index);
}

void OrderPage::refreshOrders() {
    const QString selected = orderCombo_->currentData().toString();
    orderCombo_->clear();
    const std::string sessionId = sessionCombo_->currentData().toString().toStdString();
    if (!sessionId.empty()) {
        const DynamicArray<Order> all = orders_.getOrdersForSession(sessionId);
        for (std::size_t i = 0; i < all.size(); ++i) {
            orderCombo_->addItem(
                ui::toQString(all[i].getId()) + QStringLiteral(" — ") +
                    ui::orderStatusText(all[i].getStatus()),
                ui::toQString(all[i].getId()));
        }
    }
    const int index = orderCombo_->findData(selected);
    if (index >= 0) orderCombo_->setCurrentIndex(index);
    refreshOrderItems();
}

void OrderPage::refreshOrderItems() {
    itemsTable_->setRowCount(0);
    const Order* order = orders_.findById(
        orderCombo_->currentData().toString().toStdString());
    if (order == nullptr) return;
    for (std::size_t i = 0; i < order->getItemCount(); ++i) {
        const OrderItem& item = order->getItemAt(i);
        const int row = itemsTable_->rowCount();
        itemsTable_->insertRow(row);
        QTableWidgetItem* id = ui::cell(ui::toQString(item.getId()));
        id->setData(Qt::UserRole, ui::toQString(item.getId()));
        itemsTable_->setItem(row, 0, id);
        itemsTable_->setItem(row, 1, ui::cell(ui::toQString(item.getItemNameSnapshot())));
        itemsTable_->setItem(row, 2, ui::cell(QString::number(item.getQuantity())));
        itemsTable_->setItem(
            row, 3, ui::cell(ui::formatMoney(item.getUnitPriceAtOrder())));
        itemsTable_->setItem(row, 4, ui::cell(ui::toQString(item.getNote())));
        itemsTable_->setItem(row, 5, ui::cell(ui::itemStatusText(item.getStatus())));
    }
}

void OrderPage::refreshMenuItems() {
    const QString selected = menuItemCombo_->currentData().toString();
    menuItemCombo_->clear();
    const DynamicArray<MenuItem>& all = menu_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        menuItemCombo_->addItem(
            ui::toQString(all[i].getName()) + QStringLiteral(" — ") +
                ui::formatMoney(all[i].getPrice()),
            ui::toQString(all[i].getId()));
    }
    const int index = menuItemCombo_->findData(selected);
    if (index >= 0) menuItemCombo_->setCurrentIndex(index);
}

}  // namespace restaurant
