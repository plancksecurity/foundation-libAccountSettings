// helper tool that parse all XML files given on command line
// and create a C++ initializer structure for libAccountSettings to stdout

#include "account_settings_internal.hh"
#include "tinyxml2.hh"
#include <iostream>

std::vector<AccountSettings> vas;

namespace tx = tinyxml2;


std::string assignMember(const tx::XMLElement* elem, const char* memberName, const char* message)
{
	const tx::XMLElement* member = elem->FirstChildElement(memberName);
	if(member)
	{
		return member->GetText();
	}else{
		if(message)
		{
			std::cerr << "PROBLEM: " << message;
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
		// TODO!
	}
	return v;
}


bool serverPreference(const Server& a, const Server& b)
{
	// TODO: define a proper preference
	return a.protocol < b.protocol;
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
		std::cerr << " dN=\"" << as.displayName << "\", " << as.domains.size() << " domains";
		if(as.domains.size()>0 && as.domains.size()<11)
		{
			std::cerr << ":";
			for(const auto& d : as.domains)
			{
				std::cerr << " " << d;
			}
		}
		
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
		
		
		std::cerr << ".  Done.\n";
	}
}
