#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

enum class NotificationType
{
	ORDER_APPROVED,
	ORDER_REJECTED,
	TASK_READY,
	CALL_STAFF,
	PAYMENT_SUCCESS
};

enum class NotificationStatus
{
	PENDING,
	DELIVERED
};

class Notification
{
private:
	int notificationId;
	NotificationType type;
	NotificationStatus status;
	std::string recipient;
	std::string message;

public:
	Notification(int notificationId, NotificationType type,
				 const std::string& recipient, const std::string& message);

	int getNotificationId() const;
	NotificationType getType() const;
	NotificationStatus getStatus() const;
	const std::string& getRecipient() const;
	const std::string& getMessage() const;
	bool markDelivered();
};

#endif
