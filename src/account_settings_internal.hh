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
	AS_USERNAME username;
};


enum class AS_Type { STATIC = 232323, DYNAMIC = 424242 };

struct AccountSettings
{
	AS_Type type;
	const char* id;
	const char* displayName;
	AS_Server incoming;
	AS_Server outgoing;
};

std::ostream& operator<<(std::ostream& o, const AS_Server& srv);
std::ostream& operator<<(std::ostream& o, const AccountSettings& as);

#endif // ACCOUNT_SETTINGS_INTERNAL_HH
