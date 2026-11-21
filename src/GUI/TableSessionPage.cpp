#include "TableSessionPage.h"

#include "UiHelpers.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include <ctime>
#include <string>

namespace restaurant {

TableSessionPage::TableSessionPage(TableService& tables,
                                   DiningSessionService& sessions,
                                   QWidget* parent)
    : QWidget(parent),
      tables_(tables),
      sessions_(sessions),
      tablesTable_(nullptr),
      sessionsTable_(nullptr) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* tablesHeading = new QLabel(QStringLiteral("Danh sách bàn"));
    tablesHeading->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: 600;"));
    layout->addWidget(tablesHeading);
    tablesTable_ = ui::createTable({QStringLiteral("ID"), QStringLiteral("Mã bàn"),
                                    QStringLiteral("Trạng thái"),
                                    QStringLiteral("Đang sử dụng")});
    layout->addWidget(tablesTable_);

    QHBoxLayout* tableActions = new QHBoxLayout;
    QPushButton* addTable = new QPushButton(QStringLiteral("Thêm bàn"));
    QPushButton* maintenance = new QPushButton(QStringLiteral("Chuyển bảo trì"));
    QPushButton* makeAvailable = new QPushButton(QStringLiteral("Đánh dấu trống"));
    QPushButton* editTable = new QPushButton(QStringLiteral("Sửa mã bàn"));
    QPushButton* deactivate = new QPushButton(QStringLiteral("Ngừng sử dụng"));
    tableActions->addWidget(addTable);
    tableActions->addWidget(maintenance);
    tableActions->addWidget(makeAvailable);
    tableActions->addWidget(editTable);
    tableActions->addWidget(deactivate);
    tableActions->addStretch();
    layout->addLayout(tableActions);

    QLabel* sessionsHeading = new QLabel(QStringLiteral("Phiên ăn"));
    sessionsHeading->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: 600;"));
    layout->addWidget(sessionsHeading);
    sessionsTable_ = ui::createTable({QStringLiteral("ID phiên"), QStringLiteral("Bàn"),
                                      QStringLiteral("Số khách"),
                                      QStringLiteral("Trạng thái"),
                                      QStringLiteral("Bắt đầu")});
    layout->addWidget(sessionsTable_);

    QHBoxLayout* sessionActions = new QHBoxLayout;
    QPushButton* openSession =
        new QPushButton(QStringLiteral("Mở phiên cho bàn đã chọn"));
    QPushButton* cancelSession = new QPushButton(QStringLiteral("Hủy phiên đang chọn"));
    sessionActions->addWidget(openSession);
    sessionActions->addWidget(cancelSession);
    sessionActions->addStretch();
    layout->addLayout(sessionActions);

    connect(addTable, &QPushButton::clicked, this, [this]() {
        bool accepted = false;
        const QString code = QInputDialog::getText(
            this, QStringLiteral("Thêm bàn"), QStringLiteral("Mã/tên bàn:"),
            QLineEdit::Normal, QString(), &accepted);
        if (!accepted) return;
        ui::showResult(this, tables_.addTable(ui::newId("table"), code.toStdString()));
        notifyDataChanged();
    });
    connect(maintenance, &QPushButton::clicked, this, [this]() {
        const std::string id = ui::selectedId(tablesTable_).toStdString();
        if (id.empty()) {
            ui::showResult(this, ServiceResult::failure("Vui lòng chọn bàn."));
            return;
        }
        ui::showResult(this, tables_.changeStatus(id, TableStatus::Maintenance));
        notifyDataChanged();
    });
    connect(makeAvailable, &QPushButton::clicked, this, [this]() {
        const std::string id = ui::selectedId(tablesTable_).toStdString();
        if (id.empty()) {
            ui::showResult(this, ServiceResult::failure("Vui lòng chọn bàn."));
            return;
        }
        ui::showResult(this, tables_.changeStatus(id, TableStatus::Available));
        notifyDataChanged();
    });
    connect(editTable, &QPushButton::clicked, this, [this]() {
        const std::string id = ui::selectedId(tablesTable_).toStdString();
        const Table* table = id.empty() ? nullptr : tables_.findById(id);
        if (table == nullptr) {
            ui::showResult(this, ServiceResult::failure("Vui lòng chọn bàn cần sửa."));
            return;
        }
        bool accepted = false;
        const QString code = QInputDialog::getText(
            this, QStringLiteral("Sửa bàn"), QStringLiteral("Mã/tên bàn:"),
            QLineEdit::Normal, ui::toQString(table->getCode()), &accepted);
        if (!accepted) return;
        ui::showResult(this, tables_.changeCode(id, code.toStdString()));
        notifyDataChanged();
    });
    connect(deactivate, &QPushButton::clicked, this, [this]() {
        const std::string id = ui::selectedId(tablesTable_).toStdString();
        if (id.empty()) {
            ui::showResult(this, ServiceResult::failure("Vui lòng chọn bàn."));
            return;
        }
        ui::showResult(this, tables_.deactivate(id));
        notifyDataChanged();
    });
    connect(openSession, &QPushButton::clicked, this, [this]() {
        const std::string tableId = ui::selectedId(tablesTable_).toStdString();
        if (tableId.empty()) {
            ui::showResult(this, ServiceResult::failure("Vui lòng chọn bàn."));
            return;
        }
        bool accepted = false;
        const int guestCount = QInputDialog::getInt(
            this, QStringLiteral("Mở phiên ăn"), QStringLiteral("Số lượng khách:"),
            1, 1, 1000, 1, &accepted);
        if (!accepted) return;
        ui::showResult(this, sessions_.openSession(
                                 ui::newId("session"), tableId, guestCount,
                                 std::time(nullptr)));
        notifyDataChanged();
    });
    connect(cancelSession, &QPushButton::clicked, this, [this]() {
        const std::string id = ui::selectedId(sessionsTable_).toStdString();
        if (id.empty()) {
            ui::showResult(this, ServiceResult::failure("Vui lòng chọn phiên ăn."));
            return;
        }
        ui::showResult(this, sessions_.cancelSession(id, std::time(nullptr)));
        notifyDataChanged();
    });
}

void TableSessionPage::refresh() {
    refreshTables();
    refreshSessions();
}

void TableSessionPage::setDataChangedCallback(std::function<void()> callback) {
    dataChangedCallback_ = std::move(callback);
}

void TableSessionPage::notifyDataChanged() {
    if (dataChangedCallback_) {
        dataChangedCallback_();
    } else {
        refresh();
    }
}

void TableSessionPage::refreshTables() {
    tablesTable_->setRowCount(0);
    const DynamicArray<Table>& all = tables_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        const int row = tablesTable_->rowCount();
        tablesTable_->insertRow(row);
        QTableWidgetItem* id = ui::cell(ui::toQString(all[i].getId()));
        id->setData(Qt::UserRole, ui::toQString(all[i].getId()));
        tablesTable_->setItem(row, 0, id);
        tablesTable_->setItem(row, 1, ui::cell(ui::toQString(all[i].getCode())));
        tablesTable_->setItem(row, 2, ui::cell(ui::tableStatusText(all[i].getStatus())));
        tablesTable_->setItem(
            row, 3,
            ui::cell(all[i].isActive() ? QStringLiteral("Có")
                                       : QStringLiteral("Không")));
    }
}

void TableSessionPage::refreshSessions() {
    sessionsTable_->setRowCount(0);
    const DynamicArray<DiningSession>& all = sessions_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        const int row = sessionsTable_->rowCount();
        sessionsTable_->insertRow(row);
        QTableWidgetItem* id = ui::cell(ui::toQString(all[i].getId()));
        id->setData(Qt::UserRole, ui::toQString(all[i].getId()));
        sessionsTable_->setItem(row, 0, id);
        const Table* table = tables_.findById(all[i].getTableId());
        sessionsTable_->setItem(
            row, 1,
            ui::cell(table ? ui::toQString(table->getCode())
                           : ui::toQString(all[i].getTableId())));
        sessionsTable_->setItem(row, 2,
                                ui::cell(QString::number(all[i].getGuestCount())));
        sessionsTable_->setItem(
            row, 3, ui::cell(ui::sessionStatusText(all[i].getStatus())));
        sessionsTable_->setItem(
            row, 4,
            ui::cell(QDateTime::fromSecsSinceEpoch(
                         static_cast<qint64>(all[i].getOpenedAt()))
                         .toString(QStringLiteral("yyyy-MM-dd HH:mm"))));
    }
}

}  // namespace restaurant
