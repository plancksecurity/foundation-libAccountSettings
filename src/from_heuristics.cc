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
	namespace bio = boost::asio;
	using bio::ip::tcp;

	// TODO: implement!
	class Protocol
	{
	public:
		Protocol() {}
		virtual ~Protocol() {}
		
		/// (re)initialize the protocol and thet the AS_Server to initial state
		virtual void init(AS_Server& ass) = 0;
		
		/// say something to the server. if nothing to say in this state, it returns empty string
		virtual std::string put() = 0;
		
		/// return true if guessing was successful
		virtual bool get(const std::string& s) = 0;
	};


	class Imap : public Protocol
	{
	public:
		Imap() {}
		virtual ~Imap() {}
		virtual void init(AS_Server& ass) override { /* TODO: implement! */ }
		virtual std::string put() override { return ""; }
		virtual bool get(const std::string& s) override { return s.empty(); }
	};


	AS_Server pingpong(const std::string& server, int port, Protocol& p)
	{
		AS_Server ass{ "", -1, AS_ACCESS_UNKNOWN, "" };
		try{
			std::vector<char> buffer(2048);
			bio::io_service ios;
			tcp::resolver res(ios);
			tcp::resolver::query query( server, std::to_string(port) );
			for(auto iter = res.resolve(query); iter != tcp::resolver::iterator(); ++iter)
			{
				p.init(ass);
				tcp::socket s(ios);
				bio::connect(s, iter);
				boost::system::error_code err;
				
				while(ass.access == AS_ACCESS_UNKNOWN)
				{
					const std::string output = p.put();
					if(!output.empty())
					{
						bio::write(s, bio::buffer(output), err);
					}
					
					const size_t length = s.read_some( boost::asio::buffer(buffer), err);
					if(!err || err == bio::error::eof)
					{
						if( p.get( std::string( buffer.data(), buffer.data()+length) ) )
						{
							return ass;
						}
					}
				}
			}
		}catch(std::runtime_error&) { /* just return what we have so far */ }
		
		return ass;
	}


	AS_Server probe_server(const std::string& domain, std::initializer_list<std::string> names, std::initializer_list<int> ports, Protocol& p )
	{
		for(auto port : ports)
		for(auto name : names)
		{
			const std::string server = domain + "." + name;
			const AS_Server ass = pingpong(server, port, p);
			if(ass.access != AS_ACCESS_UNKNOWN)
				return ass;
		}
		
		return AS_Server{ "", -1, AS_ACCESS_UNKNOWN, "" };
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
		Imap imap_protocol;
		as->incoming = probe_server( domain, {"imap", "mail", "imaps", "imapmail", "imap-mail", "mx", "", "mbox"}, {143, 993}, imap_protocol );
	}catch( const protocol_error& )
	{
		// ignore protocol errors
	}

//	fprintf(stderr, "== SMTP ==\n");
	try{
		Imap smtp_protocol;
		as->outgoing = probe_server( domain, {"smtp", "mail", "smtps", "smtpmail", "smtp-mail", "mx", "", "smtpauth",}, {25, 587, 465}, smtp_protocol );
	}catch( const protocol_error& )
	{
		// ignore protocol errors
	}
	
	return as;
}
