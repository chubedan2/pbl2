#include "AuditAction.h"

#include <stdexcept>

AuditAction::AuditAction(int actionId, const std::string& actor,
						 const std::string& actionType,
						 const std::string& objectId,
						 const std::string& description)
	: actionId(actionId), actor(actor), actionType(actionType),
	  objectId(objectId), description(description)
{
	if (actionId <= 0 || actor.empty() || actionType.empty() ||
		objectId.empty() || description.empty())
	{
		throw std::invalid_argument("Invalid audit action data.");
	}
}

int AuditAction::getActionId() const { return actionId; }
const std::string& AuditAction::getActor() const { return actor; }
const std::string& AuditAction::getActionType() const { return actionType; }
const std::string& AuditAction::getObjectId() const { return objectId; }
const std::string& AuditAction::getDescription() const
{
	return description;
}
