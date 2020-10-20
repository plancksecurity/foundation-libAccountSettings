// helper tool that parse all XML files given on command line
// and create a C++ initializer structure for libAccountSettings to stdout

#include "account_settings_internal.hh"
#include "tinyxml2.hh"
#include "stringpool.hh"
#include <iostream>
#include <map>
#include <vector>
#include <tuple> // for std::tie().  :-)
#include <algorithm>
#include "isp_db.hh"


// using std::string instead of const char*
struct ServerS
{
	std::string name;
	int         port = -1;
	AS_ACCESS   access = AS_ACCESS(-1);
	AS_USERNAME username = AS_USERNAME(-1);
};

struct AccountSettingsS
{
	std::string id;
	std::string displayName;
	ServerS incoming;
	ServerS outgoing;
};

std::ostream& operator<<(std::ostream& o, const ServerS& srv)
{
	char buffer[48] = {0};
	snprintf(buffer, 47, "access=0x%x, user=0x%x", srv.access, srv.username);
	return o << "{ " << buffer << ", port=" << srv.port << ", name=\"" << srv.name << "\"}";
}

std::ostream& operator<<(std::ostream& o, const AccountSettingsS& as)
{
	o << "{ id=\"" << as.id << "\", name=\"" << as.displayName << "\",\n";
	
	return o << ".\n"
		"\tIncoming: " << as.incoming << "\n"
		"\tOutgoing: " << as.outgoing << "\n"
		"}\n";
}



bool operator<(const ServerS& a, const ServerS& b)
{
	return std::tie(a.name, a.port, a.access, a.username, a.port)
	     < std::tie(b.name, b.port, b.access, b.username, b.port);
}

bool operator==(const ServerS& a, const ServerS& b)
{
	return std::tie(a.name, a.port, a.access, a.username, a.port)
	    == std::tie(b.name, b.port, b.access, b.username, b.port);
}

bool operator<(const AccountSettingsS& a, const AccountSettingsS& b)
{
	return std::tie(a.id, a.displayName, a.incoming, a.outgoing)
	     < std::tie(a.id, a.displayName, b.incoming, b.outgoing);
}

bool operator!=(const AccountSettingsS& a, const AccountSettingsS& b)
{
	return std::tie(a.id, a.displayName, a.incoming, a.outgoing)
	    != std::tie(a.id, a.displayName, b.incoming, b.outgoing);
}

std::string Hex(unsigned u)
{
	char buf[32];
	snprintf(buf,31, "0x%x", u);
	return buf;
}

typedef std::set<AccountSettingsS> AccountSettingsSet;
AccountSettingsSet ass;

std::set<std::string> problem_domains;

std::map<std::string, AccountSettingsSet::const_iterator> isp_db;


// adjacent merge
template<class InputIter, class OutputIter, class Compare, class Merge>
void adjacent_merge(InputIter begin, InputIter end, OutputIter out, Compare comp, Merge merge)
{
	while(begin!=end)
	{
		auto m = *begin;
		++begin;
		while( begin!=end && comp(m,*begin) )
		{
			merge(m, *begin);
			++begin;
		}
		
		*out = std::move(m);
		++out;
	}
}


StringPool SP;

void pool_add(const std::string& s)
{
	SP.add(s);
}

void pool_add(const std::set<std::string>& v)
{
	for(const auto& s:v) { SP.add(s); }
}

void pool_add(const ServerS& srv)
{
	SP.add(srv.name);
}

void pool_add(const AccountSettingsS& a)
{
	pool_add(a.id);
	pool_add(a.displayName);
	pool_add(a.incoming);
	pool_add(a.outgoing);
}

namespace tx = tinyxml2;

const std::map<std::string, unsigned> mproto =
	{
		// protocols
		{"pop3", AS_PROTO_POP3},
		{"imap", AS_PROTO_IMAP},
		{"smtp", AS_PROTO_SMTP},
		{"exchange", AS_PROTO_EXCHANGE},
	};

const std::map<std::string, unsigned> msocket =
	{
		// socketType
		{"plain"   , AS_SOCK_PLAIN},
		{"STARTTLS", AS_SOCK_STARTTLS},
		{"SSL"     , AS_SOCK_SSL},
	};

const std::map<std::string, unsigned> mauth =
	{
		// authentication
		{"none"              , AS_AUTH_NONE},
		{"client-IP-address" , AS_AUTH_CLIENT_IP},
		{"plain"             , AS_AUTH_PW_CLEARTEXT}, // older name for "password-cleartext"
		{"password-cleartext", AS_AUTH_PW_CLEARTEXT},
		{"password-encrypted", AS_AUTH_PW_ENCRYPTED},
		{"OAuth2"            , AS_AUTH_OAUTH2},
	};


const std::map<std::string, AS_USERNAME> musername =
	{
		{""              ,  AS_USERNAME_NONE},
		{"%EMAILADDRESS%",  AS_USERNAME_EMAIL_ADDRESS},
		{"%EMAILLOCALPART%", AS_USERNAME_EMAIL_LOCALPART},
		{"%EMAILLOCALPART%.%EMAILDOMAIN%", AS_USERNAME_EMAIL_LOCALPART_DOMAIN },
	};


std::string assignMember(const tx::XMLElement* elem, const char* memberName, const char* message = nullptr)
{
	const tx::XMLElement* member = elem->FirstChildElement(memberName);
	if(member)
	{
		return member->GetText();
	}else{
		if(message)
		{
			std::cerr << message;
		}else{
			std::cerr << "PROBLEM: No \"" << memberName << "\"! ";
		}
	}
	return std::string();
}


std::set<std::string> assignAllMembers(const tx::XMLElement* elem, const char* memberName)
{
	std::set<std::string> v;
	for(const tx::XMLElement* e = elem->FirstChildElement(memberName); e!=nullptr; e=e->NextSiblingElement(memberName) )
	{
		v.emplace( e->GetText() );
	}
	return v;
}

std::vector<ServerS> assignServers(const tx::XMLElement* elem, const char* memberName)
{
	std::vector<ServerS> v;
	for(const tx::XMLElement* e = elem->FirstChildElement(memberName); e!=nullptr; e=e->NextSiblingElement(memberName) )
	{
		ServerS server;
		unsigned access = 0;
		const std::string type = e->Attribute("type");
		try{
			access |= mproto.at(type);
		}catch(std::exception& e)
		{
			std::cerr << "*** UNKNOWN PROTOCOL TYPE \"" << type << "\" in memberName \"" << memberName << "\"! \n";
			throw;
		}
		
		server.name = assignMember(e, "hostname");
		server.port = std::stoi(assignMember(e, "port"));
		const std::string auth = assignMember(e, "authentication");
		const std::string socketType = assignMember(e, "socketType");
		access |= mauth.at(auth);
		access |= msocket.at(socketType);
		server.access = AS_ACCESS(access);
		
		const std::string username = assignMember(e, "username");
		server.username = musername.at(username);
		v.push_back(std::move(server));
	}
	return v;
}


bool serverPreference(const ServerS& a, const ServerS& b)
{
	// TODO: define a proper preference
	return a.access > b.access;
}


int main(int argc, char** argv)
try{
	std::cerr << "Reading " << (argc-1) << "file" << ((argc-1)==1 ? "" : "s") << ":\n";
	for(int a=1; a<argc; ++a)
	{
		std::cerr << "\t #" << a << " : \"" << argv[a] << "\":  ";
		tx::XMLDocument doc;
		doc.LoadFile(argv[a]);
		const tx::XMLElement* clientConfig = doc.FirstChildElement("clientConfig");
		if(clientConfig==nullptr)
		{
			std::cerr << " ERROR: not a valid clientConfig!\n";
			continue;
		}
		const tx::XMLElement* emailProvider = clientConfig->FirstChildElement("emailProvider");
		if(emailProvider==nullptr)
		{
			std::cerr << " ERROR: not a valid clientConfig! <emailProvider> not found!\n";
			continue;
		}
		
		AccountSettingsS as;
		
		const char* id = emailProvider->Attribute("id");
		if(id==nullptr)
		{
			std::cerr << " PROBLEM: <emailProvider> has no attribute id!";
		}else{
			as.id = id;
			std::cerr << "id=" << as.id;
		}
		as.displayName = assignMember(emailProvider, "displayName", "No displayName member");
		
		const std::set<std::string> domains = assignAllMembers(emailProvider, "domain");
		
		std::vector<ServerS> incomingServers = assignServers(emailProvider, "incomingServer");
		std::sort(incomingServers.begin(), incomingServers.end(), &serverPreference);
		if(incomingServers.size()>0)
		{
			as.incoming = incomingServers.front();
		}
		
		std::vector<ServerS> outgoingServers = assignServers(emailProvider, "outgoingServer");
		std::sort(outgoingServers.begin(), outgoingServers.end(), &serverPreference);
		if(outgoingServers.size()>0)
		{
			as.outgoing = outgoingServers.front();
		}
		
		pool_add(as);
		pool_add(domains);
		
		const auto r = ass.insert(as);
		std::cerr << (r.second ? "NEW" : "known") << " settings ";
		if(domains.empty())
		{
			std::cerr << " PROBLEM: empty domain list! ";
		}
		for(const auto& domain : domains)
		{
			auto iter = isp_db.find(domain);
			if(iter==isp_db.end())
			{
				isp_db.emplace( domain, r.first );
			}else{
				if(*iter->second != as)
				{
					std::cerr << " PROBLEM: domain \"" << domain << "\" has conflicting server entries!:\n"
						"\t old: " << *iter->second << ", new:" << as << "\n";
					
					problem_domains.insert(domain);
				}
			}
		}
		
		std::cerr << " pool_size=" << SP.size() << ".\n";
	}
	
	std::cerr << "There are " << ass.size() << " entries for " << isp_db.size() << " domains.\n";
	std::cerr << problem_domains.size() << " problematic domains with conflicting entries.\n";
	
	SP.makePool();
	std::cout << "// ==== Auto generated ISP DB. ===\n"
		"#include \"isp_db.hh\"\n\n"
		"namespace account_settings {\n\n";
	
	SP.printPool(std::cout, "StringPool");
	
	// write account settings list
	std::cout << "static const char* const SP = StringPool;\n\n";
	std::cout << "const AccountSettings_DB AccountList[] = {\n";
	for(const auto& as : ass)
	{
		std::cout << "\t{ SP+" << SP.getOffset(as.id) << ", SP+" << SP.getOffset(as.displayName) << ",\t// " << as.id << "\t" << as.displayName << "\n"
			"\t\t{ SP+" << SP.getOffset(as.incoming.name) << ", " << as.incoming.port << ", AS_ACCESS(" << Hex(as.incoming.access) << "), AS_USERNAME(" << Hex(as.incoming.username) << ") },\t// " << as.incoming.name << "\n"
			"\t\t{ SP+" << SP.getOffset(as.outgoing.name) << ", " << as.outgoing.port << ", AS_ACCESS(" << Hex(as.outgoing.access) << "), AS_USERNAME(" << Hex(as.outgoing.username) << ") }\t// " << as.outgoing.name << "\n"
			"\t},\n";
	}
	
	std::cout << "};\n\n"
		"const unsigned AccountListSize = " << ass.size() << ";\n";

	std::cout << "const Domain2AS IspDB[] = {\n";
	for(const auto& isp : isp_db)
	{
		const auto account_offset = std::distance(ass.begin(), isp.second);
		std::cout << "\t{ " << SP.getOffset(isp.first) << ", " << account_offset << "},\t// " << isp.first << "\n";
	}

	std::cout << "};\n\n"
		"const unsigned IspDBSize = " << isp_db.size() << ";\n\n";

	const size_t string_pool_size = SP.size();
	const size_t ass_size = ass.size()*sizeof(account_settings::AccountSettings_DB);
	const size_t isp_size = isp_db.size()*sizeof(account_settings::Domain2AS);
	
	std::cout << "\n} // end of namespace account_settings.\n\n"
		"// sizeof(Domain2AS) = " << sizeof(account_settings::Domain2AS) << "\n"
		"// sizeof(AccountSettings) = " << sizeof(account_settings::AccountSettings_DB) << "\n"
		"// Data size: " << string_pool_size << " + " << ass_size << " + " << isp_size << " = " << (string_pool_size+ass_size+isp_size) << " Bytes.\n"
		"// ===<End of generated file>===\n\n";
}
/*
catch(std::runtime_error& e)
{
	std::cerr << "ERROR: " << e.what() << "\n";
	return 7;
}
*/
catch(double * d) {throw;}
