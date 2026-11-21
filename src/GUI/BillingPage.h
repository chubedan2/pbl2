#pragma once

#include "../Services/BillService.h"
#include "../Services/DiningSessionService.h"
#include "../Services/PaymentService.h"
#include "../Services/TableService.h"

#include <QWidget>

#include <functional>

class QComboBox;
class QLineEdit;
class QTableWidget;

namespace restaurant {

class BillingPage : public QWidget {
public:
    BillingPage(BillService& bills,
                PaymentService& payments,
                TableService& tables,
                DiningSessionService& sessions,
                QWidget* parent = nullptr);

    void refresh();
    void setDataChangedCallback(std::function<void()> callback);

private:
    void refreshSessions();
    void refreshBills();
    void refreshPaymentBills();
    void notifyDataChanged();

    BillService& bills_;
    PaymentService& payments_;
    TableService& tables_;
    DiningSessionService& sessions_;
    QComboBox* sessionCombo_;
    QComboBox* billCombo_;
    QComboBox* paymentMethod_;
    QLineEdit* paymentAmount_;
    QTableWidget* billsTable_;
    std::function<void()> dataChangedCallback_;
};

}  // namespace restaurant
