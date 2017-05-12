// internal header of libAccountSettings.
// DO NOT USE THAT IN APPLICATION CODE!

#ifndef ACCOUNT_SETTINGS_INTERNAL_HH
#define ACCOUNT_SETTINGS_INTERNAL_HH

#include "account_settings.h"
#include <string>
#include <set>
#include <ostream>

struct AS_Server
{
	const char* name;
	int         port;
	AS_ACCESS   access;
	const char* username;
};

struct AccountSettings
{
	AS_STATUS status;
	const char* id;
	const char* displayName;
	AS_Server incoming;
	AS_Server outgoing;
};

std::ostream& operator<<(std::ostream& o, const AS_Server& srv);
std::ostream& operator<<(std::ostream& o, const AccountSettings& as);


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

#endif // ACCOUNT_SETTINGS_INTERNAL_HH
