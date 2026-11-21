#include "BillingPage.h"

#include "UiHelpers.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTableWidget>
#include <QVBoxLayout>

#include <ctime>
#include <string>
#include <utility>

namespace restaurant {

BillingPage::BillingPage(BillService& bills,
                         PaymentService& payments,
                         TableService& tables,
                         DiningSessionService& sessions,
                         QWidget* parent)
    : QWidget(parent),
      bills_(bills),
      payments_(payments),
      tables_(tables),
      sessions_(sessions),
      sessionCombo_(nullptr),
      billCombo_(nullptr),
      paymentMethod_(nullptr),
      paymentAmount_(nullptr),
      billsTable_(nullptr) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* createRow = new QHBoxLayout;
    createRow->addWidget(new QLabel(QStringLiteral("Phiên cần lập hóa đơn:")));
    sessionCombo_ = new QComboBox;
    createRow->addWidget(sessionCombo_, 1);
    QPushButton* createBill = new QPushButton(QStringLiteral("Lập hóa đơn"));
    createRow->addWidget(createBill);
    layout->addLayout(createRow);

    billsTable_ = ui::createTable({QStringLiteral("ID hóa đơn"),
                                   QStringLiteral("ID phiên"), QStringLiteral("Số dòng"),
                                   QStringLiteral("Tổng tiền"),
                                   QStringLiteral("Thanh toán")});
    layout->addWidget(billsTable_);

    QHBoxLayout* paymentRow = new QHBoxLayout;
    billCombo_ = new QComboBox;
    paymentAmount_ = new QLineEdit;
    paymentAmount_->setPlaceholderText(QStringLiteral("Số tiền VND"));
    paymentAmount_->setValidator(new QRegularExpressionValidator(
        QRegularExpression(QStringLiteral("[0-9]*")), paymentAmount_));
    paymentMethod_ = new QComboBox;
    paymentMethod_->addItems({QStringLiteral("Cash"), QStringLiteral("Card"),
                              QStringLiteral("Bank transfer")});
    QPushButton* pay = new QPushButton(QStringLiteral("Thanh toán"));
    paymentRow->addWidget(new QLabel(QStringLiteral("Bill chưa thanh toán:")));
    paymentRow->addWidget(billCombo_, 2);
    paymentRow->addWidget(paymentAmount_, 1);
    paymentRow->addWidget(paymentMethod_);
    paymentRow->addWidget(pay);
    layout->addLayout(paymentRow);

    connect(createBill, &QPushButton::clicked, this, [this]() {
        const std::string sessionId = sessionCombo_->currentData().toString().toStdString();
        if (sessionId.empty()) {
            ui::showResult(this, ServiceResult::failure(
                                     "Chọn phiên ăn đang hoạt động."));
            return;
        }
        ui::showResult(this, bills_.createBill(
                                 ui::newId("bill"), sessionId, std::time(nullptr)));
        notifyDataChanged();
    });
    connect(billCombo_, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index >= 0) {
            paymentAmount_->setText(
                billCombo_->itemData(index, Qt::UserRole + 1).toString());
        }
    });
    connect(pay, &QPushButton::clicked, this, [this]() {
        const std::string billId = billCombo_->currentData().toString().toStdString();
        bool parsed = false;
        const long long amount = paymentAmount_->text().trimmed().toLongLong(&parsed);
        if (billId.empty() || !parsed) {
            ui::showResult(this, ServiceResult::failure(
                                     "Chọn hóa đơn và nhập số tiền hợp lệ."));
            return;
        }
        ui::showResult(this, payments_.payBill(
                                 ui::newId("payment"), billId, amount,
                                 paymentMethod_->currentText().toStdString(),
                                 std::time(nullptr)));
        notifyDataChanged();
    });
}

void BillingPage::refresh() {
    refreshSessions();
    refreshBills();
    refreshPaymentBills();
}

void BillingPage::setDataChangedCallback(std::function<void()> callback) {
    dataChangedCallback_ = std::move(callback);
}

void BillingPage::notifyDataChanged() {
    if (dataChangedCallback_) {
        dataChangedCallback_();
    } else {
        refresh();
    }
}

void BillingPage::refreshSessions() {
    const QString selected = sessionCombo_->currentData().toString();
    sessionCombo_->clear();
    const DynamicArray<DiningSession>& all = sessions_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].getStatus() != DiningSessionStatus::Active) continue;
        if (bills_.findByDiningSessionId(all[i].getId()) != nullptr) continue;
        const Table* table = tables_.findById(all[i].getTableId());
        sessionCombo_->addItem(
            (table ? ui::toQString(table->getCode())
                   : ui::toQString(all[i].getTableId())) +
                QStringLiteral(" — ") + ui::toQString(all[i].getId()),
            ui::toQString(all[i].getId()));
    }
    const int index = sessionCombo_->findData(selected);
    if (index >= 0) sessionCombo_->setCurrentIndex(index);
}

void BillingPage::refreshBills() {
    billsTable_->setRowCount(0);
    const DynamicArray<Bill>& all = bills_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        const int row = billsTable_->rowCount();
        billsTable_->insertRow(row);
        QTableWidgetItem* id = ui::cell(ui::toQString(all[i].getId()));
        id->setData(Qt::UserRole, ui::toQString(all[i].getId()));
        billsTable_->setItem(row, 0, id);
        billsTable_->setItem(row, 1, ui::cell(ui::toQString(all[i].getDiningSessionId())));
        billsTable_->setItem(row, 2, ui::cell(QString::number(all[i].getItemCount())));
        billsTable_->setItem(row, 3,
                             ui::cell(ui::formatMoney(all[i].getTotalAmount())));
        billsTable_->setItem(
            row, 4,
            ui::cell(all[i].getPaymentStatus() == BillPaymentStatus::Paid
                         ? QStringLiteral("Đã thanh toán")
                         : QStringLiteral("Chưa thanh toán")));
    }
}

void BillingPage::refreshPaymentBills() {
    const QString selected = billCombo_->currentData().toString();
    billCombo_->clear();
    const DynamicArray<Bill>& all = bills_.getAll();
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].getPaymentStatus() != BillPaymentStatus::Unpaid ||
            payments_.findByBillId(all[i].getId()) != nullptr) {
            continue;
        }
        billCombo_->addItem(
            ui::toQString(all[i].getId()) + QStringLiteral(" — ") +
                ui::formatMoney(all[i].getTotalAmount()),
            ui::toQString(all[i].getId()));
        billCombo_->setItemData(
            billCombo_->count() - 1,
            QString::number(static_cast<qlonglong>(all[i].getTotalAmount())),
            Qt::UserRole + 1);
    }
    const int index = billCombo_->findData(selected);
    if (index >= 0) {
        billCombo_->setCurrentIndex(index);
    } else if (billCombo_->count() > 0) {
        billCombo_->setCurrentIndex(0);
    }
    if (billCombo_->currentIndex() >= 0) {
        paymentAmount_->setText(
            billCombo_->currentData(Qt::UserRole + 1).toString());
    }
}

}  // namespace restaurant
