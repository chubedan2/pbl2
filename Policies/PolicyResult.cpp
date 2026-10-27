#include "PolicyResult.h"

#include <stdexcept>

PolicyResult::PolicyResult(bool successful, const std::string& denyCode,
						   const std::string& message)
	: successful(successful), denyCode(denyCode), message(message)
{
}

PolicyResult PolicyResult::allow(const std::string& message)
{
	return PolicyResult(true, "", message);
}

PolicyResult PolicyResult::deny(const std::string& denyCode,
								const std::string& message)
{
	if (denyCode.empty() || message.empty())
	{
		throw std::invalid_argument("A denial requires a code and message.");
	}
	return PolicyResult(false, denyCode, message);
}

bool PolicyResult::isSuccess() const { return successful; }
const std::string& PolicyResult::getDenyCode() const { return denyCode; }
const std::string& PolicyResult::getMessage() const { return message; }
