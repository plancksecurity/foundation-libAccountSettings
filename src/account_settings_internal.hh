// internal header of libAccountSettings.
// DO NOT USE THAT IN APPLICATION CODE!

#ifndef ACCOUNT_SETTINGS_INTERNAL_HH
#define ACCOUNT_SETTINGS_INTERNAL_HH

#include "account_settings.h"
#include <string>
#include <vector>
#include <ostream>


struct Server
{
	std::string name;
	int         port = -1;
	AS_ACCESS   access = AS_ACCESS(-1);
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

std::ostream& operator<<(std::ostream& o, const Server& srv);
std::ostream& operator<<(std::ostream& o, const AccountSettings& as);

#endif // ACCOUNT_SETTINGS_INTERNAL_HH
