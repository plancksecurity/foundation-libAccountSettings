#include "account_settings_internal.hh"
#include "isp_db.hh"
#include <cstdio>
#include <stdexcept>


std::ostream& operator<<(std::ostream& o, const AS_Server& srv)
{
	char buffer[48] = {0};
	snprintf(buffer, 47, "access=0x%x, ", srv.access);
	return o << "{ " << buffer << ", username=\"" << srv.username << "\", port=" << srv.port << ", name=\"" << srv.name << "\"}";
}


std::ostream& operator<<(std::ostream& o, const AccountSettings& as)
{
	o << "{ status=" << unsigned(as.status) << ", id=\"" << as.id << "\", name=\"" << as.displayName << "\",\n";
	
	return o << ".\n"
		"\tIncoming: " << as.incoming << "\n"
		"\tOutgoing: " << as.outgoing << "\n"
		"}\n";
}
