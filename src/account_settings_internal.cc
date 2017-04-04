#include "account_settings_internal.hh"
#include "isp_db.hh"
#include <cstdio>
#include <stdexcept>


std::ostream& operator<<(std::ostream& o, const AS_Server& srv)
{
	char buffer[48] = {0};
	snprintf(buffer, 47, "access=0x%x, user=0x%x", srv.access, srv.username);
	return o << "{ " << buffer << ", port=" << srv.port << ", name=\"" << srv.name << "\"}";
}


std::ostream& operator<<(std::ostream& o, const AccountSettings& as)
{
	o << "{ type=" << unsigned(as.type) << ", id=\"" << as.id << "\", name=\"" << as.displayName << "\",\n";
	
	return o << ".\n"
		"\tIncoming: " << as.incoming << "\n"
		"\tOutgoing: " << as.outgoing << "\n"
		"}\n";
}


/////////////////////////
//
//  C API functions
//
/////////////////////////

using namespace account_settings;


void free_account_settings(const AccountSettings* account_settings)
{
	switch(account_settings->type)
	{
		case AS_Type::STATIC : // do nothing
			return;
		case AS_Type::DYNAMIC :
			delete[] account_settings->id; // all strings are in that array :-)
			delete   account_settings;
			return;
	}
	throw std::runtime_error("free_account_settings: illegal account settings type!");
}


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


const AccountSettings* get_account_settings(const char* accountName, const char* provider, AS_FLAGS flags, const void* credentials)
{
	const std::string ac = accountName;
	const auto at_sign = ac.rfind('@');
	const std::string domain = at_sign!=std::string::npos ? ac.substr(at_sign+1) : "";
	
	// linear search for domain names. might be replaced by binary search some day, but for <1000 domains: is it worth, yet?
	for(auto isp = IspDB; isp!= IspDB+IspDBSize; ++isp)
	{
		if( domain == StringPool + isp->domain_nr )
		{
			return AccountList + isp->as_nr;
		}
	}
	
	AccountSettings* dyn_as = create_dynamic_account_settings();
	
	return dyn_as;
}
