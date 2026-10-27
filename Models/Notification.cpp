#include "Notification.h"

#include <stdexcept>

namespace
{
bool isValidType(NotificationType type)
{
	return type == NotificationType::ORDER_APPROVED ||
		   type == NotificationType::ORDER_REJECTED ||
		   type == NotificationType::TASK_READY ||
		   type == NotificationType::CALL_STAFF ||
		   type == NotificationType::PAYMENT_SUCCESS;
}
}

Notification::Notification(int notificationId, NotificationType type,
						   const std::string& recipient,
						   const std::string& message)
	: notificationId(notificationId), type(type),
	  status(NotificationStatus::PENDING), recipient(recipient), message(message)
{
	if (notificationId <= 0 || !isValidType(type) || recipient.empty() ||
		message.empty())
	{
		throw std::invalid_argument("Invalid notification data.");
	}
}

int Notification::getNotificationId() const { return notificationId; }
NotificationType Notification::getType() const { return type; }
NotificationStatus Notification::getStatus() const { return status; }
const std::string& Notification::getRecipient() const { return recipient; }
const std::string& Notification::getMessage() const { return message; }

bool Notification::markDelivered()
{
	if (status != NotificationStatus::PENDING)
	{
		return false;
	}
	status = NotificationStatus::DELIVERED;
	return true;
}
