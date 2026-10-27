#ifndef AUDIT_ACTION_H
#define AUDIT_ACTION_H

#include <string>

class AuditAction
{
private:
	int actionId;
	std::string actor;
	std::string actionType;
	std::string objectId;
	std::string description;

public:
	AuditAction(int actionId, const std::string& actor,
				const std::string& actionType, const std::string& objectId,
				const std::string& description);

	int getActionId() const;
	const std::string& getActor() const;
	const std::string& getActionType() const;
	const std::string& getObjectId() const;
	const std::string& getDescription() const;
};

#endif
