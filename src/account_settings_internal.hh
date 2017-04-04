// internal header of libAccountSettings.
// DO NOT USE THAT IN APPLICATION CODE!

#ifndef ACCOUNT_SETTINGS_INTERNAL_HH
#define ACCOUNT_SETTINGS_INTERNAL_HH

#include "account_settings.h"
#include <string>
#include <set>
#include <ostream>

struct Server
{
	const char* name;
	int         port;
	AS_ACCESS   access;
	AS_USERNAME username;
};

struct AccountSettings
{
	enum class Type { STATIC = 232323, DYNAMIC = 424242 } type;
	const char* id;
	const char* displayName;
	Server incoming;
	Server outgoing;
};

std::ostream& operator<<(std::ostream& o, const Server& srv);
std::ostream& operator<<(std::ostream& o, const AccountSettings& as);

#endif // ACCOUNT_SETTINGS_INTERNAL_HH
