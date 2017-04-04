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
			AS_Type::DYNAMIC, nullptr, nullptr,
				{nullptr, -1, AS_ACCESS(-1), AS_USERNAME(-1)}, 
				{nullptr, -1, AS_ACCESS(-1), AS_USERNAME(-1)}
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
	const unsigned type = (account_settings->type & AS_Type::MASK);
	if(type == AS_Type::STATIC)
	{
		// nothing to do
		return;
	}else if(type == AS_Type::DYNAMIC)
	{
		delete[] account_settings->id; // all strings are in that array :-)
		delete   account_settings;
		return;
	}
	throw std::runtime_error("free_account_settings: illegal account settings type!");
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
			return AccountList + isp->as_nr;
		}
	}
	
	AccountSettings* dyn_as = create_dynamic_account_settings();

	// TODO
	//  - implement heuristics
	//  - implement automx
	//  - fetch from Mozilla's DB (still necessary when we have their data already in our local DB?)
	
	return dyn_as;
}


AS_STATUS AS_get_status(AccountSettings* account_settings)
{
	return account_settings ? AS_STATUS(account_settings->type & ~AS_Type::MASK) : AS_ILLEGAL_VALUE;
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
	return server->name;
}


int AS_get_port(const AS_Server* server)
{
	return server->port;
}


AS_ACCESS AS_get_access_method(const AS_Server* server)
{
	return server->access;
}


AS_USERNAME AS_get_username(const AS_Server* server)
{
	return server->username;
}
