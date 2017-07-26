#ifndef ACCOUNT_SETTINGS_FROM_SRV_HH
#define ACCOUNT_SETTINGS_FROM_SRV_HH

#include "account_settings_internal.hh"

AccountSettings* get_settings_from_srv(AccountSettings* as, const std::string& accountName, const std::string& domain, const std::string& provider);


#endif // ACCOUNT_SETTINGS_FROM_SRV_HH
