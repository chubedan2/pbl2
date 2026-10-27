#ifndef BILL_H
#define BILL_H

#include <cstdint>

enum class PaymentMethod
{
	CASH,
	CARD,
	OTHER
};

enum class BillStatus
{
	UNPAID,
	PROCESSING,
	PAID
};

class Bill
{
private:
	int billId;
	int sessionId;
	std::int64_t total;
	PaymentMethod paymentMethod;
	BillStatus status;

public:
	Bill(int billId, int sessionId, std::int64_t total,
		 PaymentMethod paymentMethod = PaymentMethod::CASH);

	int getBillId() const;
	int getSessionId() const;
	std::int64_t getTotal() const;
	PaymentMethod getPaymentMethod() const;
	BillStatus getStatus() const;

	bool setPaymentMethod(PaymentMethod method);
	bool beginPayment();
	bool finishPayment(bool succeeded);
};

#endif
