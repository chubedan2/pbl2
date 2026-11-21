#include "MenuPage.h"

#include "UiHelpers.h"

#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include <string>
#include <utility>

namespace restaurant {

MenuPage::MenuPage(MenuService& menu, QWidget* parent)
    : QWidget(parent), menu_(menu), menuTable_(nullptr) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* heading = new QLabel(QStringLiteral("Thực đơn"));
    heading->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: 600;"));
    layout->addWidget(heading);
    menuTable_ = ui::createTable({QStringLiteral("ID"), QStringLiteral("Tên món"),
                                  QStringLiteral("Giá (VND)"), QStringLiteral("Mô tả")});
    layout->addWidget(menuTable_);

    QHBoxLayout* actions = new QHBoxLayout;
    QPushButton* add = new QPushButton(QStringLiteral("Thêm món"));
    QPushButton* edit = new QPushButton(QStringLiteral("Sửa món đã chọn"));
    actions->addWidget(add);
    actions->addWidget(edit);
    actions->addStretch();
    layout->addLayout(actions);

    connect(add, &QPushButton::clicked, this, [this]() {
        bool accepted = false;
        const QString name = QInputDialog::getText(
            this, QStringLiteral("Thêm món"), QStringLiteral("Tên món:"),
            QLineEdit::Normal, QString(), &accepted);
        if (!accepted) return;
        long long price = 0;
        if (!ui::promptPrice(this, QStringLiteral("Thêm món"), 0, &price)) return;
        const QString description = QInputDialog::getText(
            this, QStringLiteral("Thêm món"),
            QStringLiteral("Mô tả (không bắt buộc):"),
            QLineEdit::Normal, QString(), &accepted);
        if (!accepted) return;
        ui::showResult(this, menu_.addItem(ui::newId("menu"), name.toStdString(),
                                           price, description.toStdString()));
        notifyDataChanged();
    });
    connect(edit, &QPushButton::clicked, this, [this]() {
        const std::string id = ui::selectedId(menuTable_).toStdString();
        const MenuItem* item = id.empty() ? nullptr : menu_.findById(id);
        if (item == nullptr) {
            ui::showResult(this, ServiceResult::failure("Vui lòng chọn món cần sửa."));
            return;
        }
        bool accepted = false;
        const QString name = QInputDialog::getText(
            this, QStringLiteral("Sửa món"), QStringLiteral("Tên món:"),
            QLineEdit::Normal, ui::toQString(item->getName()), &accepted);
        if (!accepted) return;
        long long price = 0;
        if (!ui::promptPrice(this, QStringLiteral("Sửa món"), item->getPrice(), &price)) {
            return;
        }
        const QString description = QInputDialog::getText(
            this, QStringLiteral("Sửa món"), QStringLiteral("Mô tả:"),
            QLineEdit::Normal, ui::toQString(item->getDescription()), &accepted);
        if (!accepted) return;
        ui::showResult(this, menu_.updateItem(id, name.toStdString(), price,
                                              description.toStdString()));
        notifyDataChanged();
    });
}

void MenuPage::refresh() {
    menuTable_->setRowCount(0);
    const DynamicArray<MenuItem>& all = menu_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        const int row = menuTable_->rowCount();
        menuTable_->insertRow(row);
        QTableWidgetItem* id = ui::cell(ui::toQString(all[i].getId()));
        id->setData(Qt::UserRole, ui::toQString(all[i].getId()));
        menuTable_->setItem(row, 0, id);
        menuTable_->setItem(row, 1, ui::cell(ui::toQString(all[i].getName())));
        menuTable_->setItem(row, 2, ui::cell(ui::formatMoney(all[i].getPrice())));
        menuTable_->setItem(row, 3, ui::cell(ui::toQString(all[i].getDescription())));
    }
}

void MenuPage::setDataChangedCallback(std::function<void()> callback) {
    dataChangedCallback_ = std::move(callback);
}

void MenuPage::notifyDataChanged() {
    if (dataChangedCallback_) {
        dataChangedCallback_();
    } else {
        refresh();
    }
}

}  // namespace restaurant
