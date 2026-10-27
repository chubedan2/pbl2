#include "Bill.h"

#include <stdexcept>

namespace
{
bool isValidPaymentMethod(PaymentMethod method)
{
	return method == PaymentMethod::CASH || method == PaymentMethod::CARD ||
		   method == PaymentMethod::OTHER;
}
}

Bill::Bill(int billId, int sessionId, std::int64_t total,
		   PaymentMethod paymentMethod)
	: billId(billId), sessionId(sessionId), total(total),
	  paymentMethod(paymentMethod), status(BillStatus::UNPAID)
{
	if (billId <= 0 || sessionId <= 0 || total < 0 ||
		!isValidPaymentMethod(paymentMethod))
	{
		throw std::invalid_argument("Invalid bill data.");
	}
}

int Bill::getBillId() const { return billId; }
int Bill::getSessionId() const { return sessionId; }
std::int64_t Bill::getTotal() const { return total; }
PaymentMethod Bill::getPaymentMethod() const { return paymentMethod; }
BillStatus Bill::getStatus() const { return status; }

bool Bill::setPaymentMethod(PaymentMethod method)
{
	if (status != BillStatus::UNPAID || !isValidPaymentMethod(method))
	{
		return false;
	}
	paymentMethod = method;
	return true;
}

bool Bill::beginPayment()
{
	if (status != BillStatus::UNPAID)
	{
		return false;
	}
	status = BillStatus::PROCESSING;
	return true;
}

bool Bill::finishPayment(bool succeeded)
{
	if (status != BillStatus::PROCESSING)
	{
		return false;
	}
	status = succeeded ? BillStatus::PAID : BillStatus::UNPAID;
	return true;
}
