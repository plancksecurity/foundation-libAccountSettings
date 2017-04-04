#ifndef ISP_DB_YIJZYUNKJJXCQWAETZUDJ_HH
#define ISP_DB_YIJZYUNKJJXCQWAETZUDJ_HH

#include "account_settings_internal.hh"

namespace account_settings
{
	extern const char* const StringPool;
	extern const char* const StringPoolEnd;
	extern const unsigned    StringPoolSize;

	extern const AccountSettings AccountList[];
	extern const unsigned        AccountListSize;

	struct Domain2AS
	{
		const char* const domain;
		const AccountSettings* as;
	};

	extern const Domain2AS IspDB[];
	extern const unsigned  IspDBSize;

} // end of account_settings

#endif // ISP_DB_YIJZYUNKJJXCQWAETZUDJ_HH
