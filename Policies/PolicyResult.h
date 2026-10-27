#ifndef POLICY_RESULT_H
#define POLICY_RESULT_H

#include <string>

class PolicyResult
{
private:
	bool successful;
	std::string denyCode;
	std::string message;

	PolicyResult(bool successful, const std::string& denyCode,
				 const std::string& message);

public:
	static PolicyResult allow(const std::string& message = "OK");
	static PolicyResult deny(const std::string& denyCode,
							 const std::string& message);

	bool isSuccess() const;
	const std::string& getDenyCode() const;
	const std::string& getMessage() const;
};

#endif
