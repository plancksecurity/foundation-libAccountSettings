// internal header of libAccountSettings.
// DO NOT USE THAT IN APPLICATION CODE!

#ifndef ACCOUNT_SETTINGS_INTERNAL_HH
#define ACCOUNT_SETTINGS_INTERNAL_HH

#include "account_settings_c.h"
#include <string>
#include <set>
#include <ostream>

// public names, must be accessible from C, so no namespace here:
struct AS_Server
{
	std::string name;
	int         port;
	AS_ACCESS   access;
	std::string username;
};

struct AccountSettings
{
	AS_STATUS status;
	std::string id;
	std::string displayName;
	AS_Server incoming;
	AS_Server outgoing;
};

std::ostream& operator<<(std::ostream& o, const AS_Server& srv);
std::ostream& operator<<(std::ostream& o, const AccountSettings& as);


namespace account_settings {
// for static ISP DB:
struct AS_Server_DB
{
	const char* name;
	int         port;
	AS_ACCESS   access;
	AS_USERNAME username_type;
};

struct AccountSettings_DB
{
	const char* id;
	const char* displayName;
	AS_Server_DB incoming;
	AS_Server_DB outgoing;
};

} // end of namespace account_settings

#endif // ACCOUNT_SETTINGS_INTERNAL_HH
