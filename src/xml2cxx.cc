// helper tool that parse all XML files given on command line
// and create a C++ initializer structure for libAccountSettings to stdout

#include "account_settings_internal.hh"
#include "tinyxml2.hh"
#include <iostream>
#include <map>

std::vector<AccountSettings> vas;

namespace tx = tinyxml2;

const std::map<std::string, unsigned> mproto =
	{
		// protocols
		{"pop3", 0x11000},
		{"imap", 0x12000},
		{"smtp", 0x21000},
	};

const std::map<std::string, unsigned> mauth =
	{
		// authentication
		{"none"              , 0x00001},
		{"client-IP-address" , 0x00005},
		{"plain"             , 0x00010}, // is it the same as "password-cleartext"?
		{"password-cleartext", 0x00011},
		{"password-encrypted", 0x00020},
		{"OAuth2"            , 0x00028},
	};

const std::map<std::string, unsigned> msocket =
	{
		// socketType
		{"plain"   , 0x00100},
		{"STARTTLS", 0x00200},
		{"SSL"     , 0x00300},
	};

const std::map<std::string, AS_USERNAME> musername =
	{
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
		unsigned protocol = 0;
		const std::string type = e->Attribute("type");
		protocol |= mproto.at(type);
		
		server.name = assignMember(e, "hostname");
		server.port = std::stoi(assignMember(e, "port"));
		const std::string auth = assignMember(e, "authentication");
		const std::string socketType = assignMember(e, "socketType");
		protocol |= mauth.at(auth);
		protocol |= msocket.at(socketType);
		server.protocol = AS_PROTOCOL(protocol);
		
		const std::string username = assignMember(e, "username");
		server.username = musername.at(username);
		v.push_back(std::move(server));
	}
	return v;
}


bool serverPreference(const Server& a, const Server& b)
{
	// TODO: define a proper preference
	return a.protocol > b.protocol;
}


int main(int argc, char** argv)
{
	std::cerr << "Reading " << (argc-1) << "file" << ((argc-1)==1 ? "" : "s") << ":\n";
	for(int a=1; a<argc; ++a)
	{
		std::cerr << "\t #" << a << " : \"" << argv[a] << "\":  ";
		tx::XMLDocument doc;
		doc.LoadFile(argv[a]);
		const tx::XMLElement* clientConfig = doc.FirstChildElement("clientConfig");
		if(clientConfig==nullptr)
		{
			std::cerr << "ERROR: not a valid clientConfig!";
			continue;
		}
		const tx::XMLElement* emailProvider = clientConfig->FirstChildElement("emailProvider");
		if(emailProvider==nullptr)
		{
			std::cerr << "ERROR: not a valid clientConfig! <emailProvider> not found!";
			continue;
		}
		
		AccountSettings as;
		
		const char* id = emailProvider->Attribute("id");
		if(id==nullptr)
		{
			std::cerr << "PROBLEM: <emailProvider> has no attribute id. :-o";
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
		
		std::cerr << '\n' << as;
		std::cerr << ".  Done.\n";
		vas.push_back(std::move(as));
	}
}
