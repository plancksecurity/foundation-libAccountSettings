#include "from_heuristics.hh"
#include "account_settings_internal.hh"
#include <ldns/ldns.h>
#include <stdexcept>
#include <memory>

#include "boost_asio.hh"


class protocol_error : public std::runtime_error
{
public:
	protocol_error() : std::runtime_error("Protocol error!")
	{}
};

namespace
{
	// TODO: implement!
	class Protocol
	{
	public:
		Protocol() = default;
	};


	AS_Server probe_server(const std::string& domain, std::initializer_list<std::string> names, Protocol p=Protocol() )
	{
		AS_Server ass { "", -1, AS_ACCESS(-1), "" };
		
		throw protocol_error{};
	}

} //end of anonymous namespace


AccountSettings* get_settings_from_heuristics(AccountSettings* as, const std::string& accountName, const std::string& domain, const std::string& provider)
{
	if(as==nullptr)
	{
		throw std::runtime_error("get_settings_from_heuristics shall not be called with NULL pointer!");
	}

//	fprintf(stderr, "== IMAP ==\n");
	try{
		as->incoming = probe_server( domain, {"imap", "mail", "imaps", "imapmail", "imap-mail", "mx", "", "mbox",} );
	}catch( const protocol_error& )
	{
		// ignore protocol errors
	}

//	fprintf(stderr, "== SMTP ==\n");
	try{
		as->outgoing = probe_server( domain, {"smtp", "mail", "smtps", "smtpmail", "smtp-mail", "mx", "", "smtpauth",} );
	}catch( const protocol_error& )
	{
		// ignore protocol errors
	}
	
	return as;
}
