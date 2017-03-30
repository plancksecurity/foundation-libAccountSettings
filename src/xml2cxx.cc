// helper tool that parse all XML files given on command line
// and create a C++ initializer structure for libAccountSettings to stdout

#include "account_settings_internal.hh"
#include "tinyxml2.hh"
#include "stringpool.hh"
#include <iostream>
#include <map>

std::vector<AccountSettings> vas;

StringPool SP;


void pool_add(const std::string& s)
{
	SP.add(s);
}

void pool_add(const std::vector<std::string>& v)
{
	for(const auto& s:v) { SP.add(s); }
}

void pool_add(const Server& srv)
{
	SP.add(srv.name);
}

void pool_add(const AccountSettings& a)
{
	pool_add(a.id);
	pool_add(a.displayName);
	pool_add(a.domains);
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
		{"plain"             , AS_AUTH_PLAIN}, // is it the same as "password-cleartext"?
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


std::vector<std::string> assignAllMembers(const tx::XMLElement* elem, const char* memberName)
{
	std::vector<std::string> v;
	for(const tx::XMLElement* e = elem->FirstChildElement(memberName); e!=nullptr; e=e->NextSiblingElement(memberName) )
	{
		v.emplace_back( e->GetText() );
	}
	return v;
}

std::vector<Server> assignServers(const tx::XMLElement* elem, const char* memberName)
{
	std::vector<Server> v;
	for(const tx::XMLElement* e = elem->FirstChildElement(memberName); e!=nullptr; e=e->NextSiblingElement(memberName) )
	{
		Server server;
		unsigned access = 0;
		const std::string type = e->Attribute("type");
		access |= mproto.at(type);
		
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


bool serverPreference(const Server& a, const Server& b)
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
		
		AccountSettings as;
		
		const char* id = emailProvider->Attribute("id");
		if(id==nullptr)
		{
			std::cerr << " PROBLEM: <emailProvider> has no attribute id!";
		}else{
			as.id = id;
			std::cerr << "id=" << as.id;
		}
		as.displayName = assignMember(emailProvider, "displayName", "No displayName member");
		as.domains = assignAllMembers(emailProvider, "domain");
		std::vector<Server> incomingServers = assignServers(emailProvider, "incomingServer");
		std::sort(incomingServers.begin(), incomingServers.end(), &serverPreference);
		if(incomingServers.size()>0)
		{
			as.incoming = incomingServers.front();
		}
		
		std::vector<Server> outgoingServers = assignServers(emailProvider, "outgoingServer");
		std::sort(outgoingServers.begin(), outgoingServers.end(), &serverPreference);
		if(outgoingServers.size()>0)
		{
			as.outgoing = outgoingServers.front();
		}
		
		pool_add(as);
		std::cout << "\n" << as << std::flush;
		std::cerr << " pool_size=" << SP.size() << ".\n";
		vas.push_back(std::move(as));
	}
	
	SP.makePool();
	std::cout << "\n ==== Pool ===\n\n";
	SP.printPool(std::cout, "stringpool");
}
catch(std::runtime_error& e)
{
	std::cerr << "ERROR: " << e.what() << "\n";
	return 7;
}
