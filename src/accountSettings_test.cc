// unit test program for libAccountSettings

#include "../include/account_settings.h"
#include <string>
#include <vector>
#include <iostream>


template<class Value>
struct KeyValue
{
	std::string key;
	Value value;
};

struct TestHost
{
	std::string name;
	int port;
};

const std::vector< KeyValue< TestHost> > incomingServer =
	{
		{ "lib_as@peptest.ch", {"mail.syhosting.ch", 993} },
		{ "example@gmx.de"   , {"imap.gmx.de", 143} },
		{ "example@yandex.ua", {"imap.yandex.com", 993} },
	};

template<class TestData, class Function >
bool testServers( const TestData& testData, Function testFunc )
{
	bool okay = true;
	for(const auto& t : testData )
	{
		const AccountSettings* as = get_account_settings( t.key.c_str(), nullptr, AS_FLAG_USE_ANY, nullptr);
		if(as==nullptr)
		{
			throw std::runtime_error("Got NULL AccountSettings* for testdata \"" + t.key + "\"!");
		}
		
		const AS_Server* srv = testFunc(as);
		const char* hostname = AS_get_hostname(srv);
		if( hostname != t.value.name )
		{
			std::cerr << "Got Hostname \"" << hostname << "\", expected \"" << t.value.name << "\" for account " << t.key << "!\n";
			okay = false;
		}
		
		const int port = AS_get_port(srv);
		if( port != t.value.port )
		{
			std::cerr << "Got port " << port << ", expected " << t.value.port << " for account " << t.key << "!\n";
			okay = false;
		}
	}
	
	return okay;
}


int main()
{
	bool all_okay = testServers( incomingServer, &AS_get_incoming );
	
	return all_okay ? 0 : 2;
}
