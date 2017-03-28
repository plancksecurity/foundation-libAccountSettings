// internal header of libAccountSettings.
// DO NOT USE THAT IN APPLICATION CODE!

#ifndef ACCOUNT_SETTINGS_INTERNAL_HH
#define ACCOUNT_SETTINGS_INTERNAL_HH

#include "account_settings.h"
#include <string>
#include <vector>

struct Server
{
	std::string name;
	int         port = -1;
	AS_PROTOCOL protocol = AS_PROTOCOL(-1);
	AS_USERNAME username = AS_USERNAME(-1);
};

struct AccountSettings
{
	std::string id;
	std::string displayName;
	std::vector<std::string> domains;
	Server incoming;
	Server outgoing;
};

#endif // ACCOUNT_SETTINGS_INTERNAL_HH
