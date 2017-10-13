#ifndef ACCOUNT_SETTINGS_FROM_HEURISTICS_HH
#define ACCOUNT_SETTINGS_FROM_HEURISTICS_HH

#include "account_settings_internal.hh"

AccountSettings* get_settings_from_heuristics(AccountSettings* as, const std::string& accountName, const std::string& domain, const std::string& provider);

#endif // ACCOUNT_SETTINGS_FROM_HEURISTICS_HH
