// helper tool that parse all XML files given on command line
// and create a C++ initializer structure for libAccountSettings to stdout

#include "account_settings_internal.hh"
#include "tinyxml2.hh"
#include <cstdio>

int main(int argc, char** argv)
{
	for(int a=1; a<argc; ++a)
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(argv[a]);
		
	}
}
