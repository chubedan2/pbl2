#ifndef USER_H
#define USER_H

#include <string>

enum class UserRole
{
	CUSTOMER,
	STAFF,
	WAITER,
	KITCHEN,
	BAR,
	MANAGER
};

class User
{
private:
	int userId;
	std::string name;
	UserRole role;

public:
	User(int userId, const std::string& name, UserRole role);

	int getUserId() const;
	const std::string& getName() const;
	UserRole getRole() const;
};

#endif
