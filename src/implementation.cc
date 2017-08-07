#include "account_settings_internal.hh"
#include "isp_db.hh"
#include <cstdio>
#include <stdexcept>


namespace
{

AccountSettings* create_dynamic_account_settings()
{
	static const AccountSettings dummyAS
		{
			AS_NOT_FOUND, "", "",
				{"", -1, AS_ACCESS(-1), ""},
				{"", -1, AS_ACCESS(-1), ""}
		};
	
	return new AccountSettings{dummyAS};
}

} // end of anonymous namespace


/////////////////////////
//
//  C API functions
//
/////////////////////////

using namespace account_settings;


void free_account_settings(const AccountSettings* account_settings)
{
	delete   account_settings;
}


std::string type2username(AS_USERNAME username_type, const std::string& accountName, const std::string& provider)
{
	const auto at_sign = accountName.rfind('@');
	switch(username_type)
	{
		case AS_USERNAME_NONE:             return std::string();
		case AS_USERNAME_EMAIL_ADDRESS:    return accountName;
		case AS_USERNAME_EMAIL_LOCALPART:  return accountName.substr(0, at_sign);
		case AS_USERNAME_EMAIL_LOCALPART_DOMAIN: return accountName.substr(0, at_sign) + "@" + provider;
		case AS_USERNAME_OTHER : throw std::runtime_error("Username cannot be created algorithmically from accountName!");
	}
	throw std::logic_error("Unkown/unmplemented username type " + std::to_string(int(username_type)) );
}


const AccountSettings* create_account_settings_from_db( const AccountSettings_DB* asdb, const std::string& accountName, const std::string& provider)
{
	auto as = create_dynamic_account_settings();
	as->id = asdb->id;
	as->displayName = asdb->displayName;
	as->incoming.name = asdb->incoming.name;
	as->outgoing.name = asdb->outgoing.name;
	as->incoming.username = type2username(asdb->incoming.username_type, accountName, provider);
	as->outgoing.username = type2username(asdb->outgoing.username_type, accountName, provider);
	as->incoming.access = asdb->incoming.access;
	as->outgoing.access = asdb->outgoing.access;
	as->incoming.port   = asdb->incoming.port;
	as->outgoing.port   = asdb->outgoing.port; 
	as->status = AS_OK;
	return as;
}


const AccountSettings* get_account_settings(const char* accountName, const char* provider, AS_FLAGS flags, const void* credentials)
{
	const std::string ac = accountName;
	const auto at_sign = ac.rfind('@');
	const std::string domain = at_sign!=std::string::npos ? ac.substr(at_sign+1) : "";
	
	// FIXME: it does a linear search for domain names, yet. Might be replaced by binary search some day, but for <1000 domains: is it worth?
	for(auto isp = IspDB; isp!= IspDB+IspDBSize; ++isp)
	{
		if( domain == StringPool + isp->domain_nr )
		{
			return create_account_settings_from_db(AccountList + isp->as_nr, accountName, (provider?provider:""));
		}
	}
	
	AccountSettings* dyn_as = create_dynamic_account_settings();

	// TODO
	//  - implement heuristics
	//  - implement automx
	//  - fetch from Mozilla's DB (still necessary when we have their data already in our local DB?)
	
	return dyn_as;
}


AS_STATUS AS_get_status(const struct AccountSettings* account_settings)
{
	return account_settings ? account_settings->status : AS_ILLEGAL_VALUE;
}


const AS_Server* AS_get_incoming(const AccountSettings* as)
{
	return &as->incoming;
}

const AS_Server* AS_get_outgoing(const AccountSettings* as)
{
	return &as->outgoing;
}


const char* AS_get_hostname(const AS_Server* server)
{
	return server->name.c_str();
}


int AS_get_port(const AS_Server* server)
{
	return server->port;
}


AS_ACCESS AS_get_access_method(const AS_Server* server)
{
	return server->access;
}


const char* AS_get_username(const AS_Server* server)
{
	return server->username.c_str();
}
