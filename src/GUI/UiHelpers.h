#pragma once

#include "../Models/Bill.h"
#include "../Models/DiningSession.h"
#include "../Models/Order.h"
#include "../Models/OrderItem.h"
#include "../Models/Table.h"
#include "../Services/ServiceResult.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QUuid>

#include <string>

namespace restaurant {
namespace ui {

inline QString toQString(const std::string& value) {
    return QString::fromStdString(value);
}

inline std::string newId(const char* prefix) {
    return std::string(prefix) + "-" +
           QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
}

inline QString tableStatusText(TableStatus status) {
    switch (status) {
        case TableStatus::Available: return QStringLiteral("Trống");
        case TableStatus::Occupied: return QStringLiteral("Đang phục vụ");
        case TableStatus::Maintenance: return QStringLiteral("Bảo trì");
    }
    return QStringLiteral("Không xác định");
}

inline QString sessionStatusText(DiningSessionStatus status) {
    switch (status) {
        case DiningSessionStatus::Active: return QStringLiteral("Đang hoạt động");
        case DiningSessionStatus::WaitingForPayment: return QStringLiteral("Chờ thanh toán");
        case DiningSessionStatus::Closed: return QStringLiteral("Đã đóng");
        case DiningSessionStatus::Cancelled: return QStringLiteral("Đã hủy");
    }
    return QStringLiteral("Không xác định");
}

inline QString orderStatusText(OrderStatus status) {
    switch (status) {
        case OrderStatus::Draft: return QStringLiteral("Bản nháp");
        case OrderStatus::Pending: return QStringLiteral("Đang chờ");
        case OrderStatus::InProgress: return QStringLiteral("Đang phục vụ");
        case OrderStatus::Completed: return QStringLiteral("Hoàn tất");
        case OrderStatus::Cancelled: return QStringLiteral("Đã hủy");
    }
    return QStringLiteral("Không xác định");
}

inline QString itemStatusText(OrderItemStatus status) {
    switch (status) {
        case OrderItemStatus::Pending: return QStringLiteral("Chưa phục vụ");
        case OrderItemStatus::Served: return QStringLiteral("Đã phục vụ");
        case OrderItemStatus::Cancelled: return QStringLiteral("Đã hủy");
    }
    return QStringLiteral("Không xác định");
}

inline QString formatMoney(long long amount) {
    return QString::number(static_cast<qlonglong>(amount)) + QStringLiteral(" VND");
}

inline bool promptPrice(QWidget* parent,
                        const QString& title,
                        long long initialValue,
                        long long* price) {
    bool accepted = false;
    const QString text = QInputDialog::getText(
        parent, title, QStringLiteral("Giá (VND, số nguyên không âm):"),
        QLineEdit::Normal,
        QString::number(static_cast<qlonglong>(initialValue)), &accepted);
    if (!accepted) {
        return false;
    }

    bool parsed = false;
    const qlonglong value = text.trimmed().toLongLong(&parsed);
    if (!parsed || value < 0) {
        QMessageBox::warning(parent, QStringLiteral("Giá không hợp lệ"),
                             QStringLiteral("Giá phải là số nguyên VND không âm."));
        return false;
    }
    *price = static_cast<long long>(value);
    return true;
}

inline QTableWidget* createTable(const QStringList& headers) {
    QTableWidget* table = new QTableWidget(0, headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    return table;
}

inline QTableWidgetItem* cell(const QString& text) {
    return new QTableWidgetItem(text);
}

inline QString selectedId(const QTableWidget* table) {
    if (table->currentRow() < 0 || table->item(table->currentRow(), 0) == nullptr) {
        return QString();
    }
    return table->item(table->currentRow(), 0)->data(Qt::UserRole).toString();
}

inline void showResult(QWidget* parent, const ServiceResult& result) {
    if (result.succeeded) {
        QMessageBox::information(parent, QStringLiteral("Thành công"),
                                 toQString(result.message));
    } else {
        QMessageBox::warning(parent, QStringLiteral("Không thể thực hiện"),
                             toQString(result.message));
    }
}

}  // namespace ui
}  // namespace restaurant
