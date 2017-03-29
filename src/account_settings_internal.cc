#include "account_settings_internal.hh"
#include <cstdio>


std::ostream& operator<<(std::ostream& o, const Server& srv)
{
	char buffer[48] = {0};
	snprintf(buffer, 47, "proto=0x%x, user=0x%x", srv.protocol, srv.username);
	return o << "{ " << buffer << ", port=" << srv.port << ", name=\"" << srv.name << "\"}";
}


std::ostream& operator<<(std::ostream& o, const AccountSettings& as)
{
	o << "{ id=\"" << as.id << "\", name=\"" << as.displayName << "\",\n"
		"\t" << as.domains.size() << " domain" << (as.domains.size()==1 ? "" : "s");
	if( as.domains.size()>1 && as.domains.size()<10)
	{
		o << ": " << as.domains[0];
		for(unsigned u=1; u<as.domains.size(); ++u)
		{
			o << ", " << as.domains[u];
		}
	}
	
	return o << ".\n"
		"\tIncoming: " << as.incoming << "\n"
		"\tOutgoing: " << as.outgoing << "\n"
		"}\n";
}

