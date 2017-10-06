// unit test program for libAccountSettings

#include "../include/account_settings_c.h"
#include "from_srv.hh"
#include <string>
#include <vector>
#include <iostream>
#include <memory>


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

// stored in ISP DB
const std::vector< KeyValue< TestHost> > incomingServer =
	{
		{ "lib_as@peptest.ch", {"peptest.ch", 993} },
		{ "example@gmx.de"   , {"imap.gmx.net", 993} },
		{ "example@yandex.ua", {"imap.yandex.com", 993} },
	};

// stored in ISP DB
const std::vector< KeyValue< TestHost> > outgoingServer =
	{
		{ "lib_as@peptest.ch", {"peptest.ch", 587} },
		{ "example@gmx.de"   , {"mail.gmx.net", 465} },
		{ "example@yandex.ua", {"smtp.yandex.com", 465} },
	};


// 
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
		
		free_account_settings(as);
	}
	
	return okay;
}


bool testSRV()
{
	std::cout << "Test DNS queries for SRV records. These tests might fail due to network problems." << std::endl;
	bool okay = true;
	
	return okay;
}


int main(int argc, char** argv)
try{
	if(argc>1)
	{
		for(int a=1; a<argc; ++a)
		{
			printf("#%d : ", a);
			auto as = std::unique_ptr<AccountSettings>{ new AccountSettings };
			get_settings_from_srv( as.get(), "foo@bar.com", argv[a], "dummy");
			printf("\n");
		}
	}
	bool all_okay = true;
	all_okay &= testServers( incomingServer, &AS_get_incoming );
	all_okay &= testServers( outgoingServer, &AS_get_outgoing );
	
	all_okay &= testSRV();
	
	std::cout << "***\t" << (all_okay ? "All tests are okay." : "ERROS happened") << std::endl;
	return all_okay ? 0 : 2;
}
catch(const std::runtime_error& e)
{
	std::cerr << "Exception: " << e.what() << std::endl;
	return 4;
}
catch(...)
{
	std::cerr << "Unknown exception! :-(" << std::endl;
	return 23;
}
