#include "User.h"

#include <stdexcept>

namespace
{
bool isValidRole(UserRole role)
{
	return role == UserRole::CUSTOMER || role == UserRole::STAFF ||
		   role == UserRole::WAITER || role == UserRole::KITCHEN ||
		   role == UserRole::BAR || role == UserRole::MANAGER;
}
}

User::User(int userId, const std::string& name, UserRole role)
	: userId(userId), name(name), role(role)
{
	if (userId <= 0 || name.empty() || !isValidRole(role))
	{
		throw std::invalid_argument("Invalid user data.");
	}
}

int User::getUserId() const { return userId; }
const std::string& User::getName() const { return name; }
UserRole User::getRole() const { return role; }
