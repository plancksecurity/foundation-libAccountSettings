/**
 * Account Settings - a small library to retrieve & guess settings for e-mail and other accounts
 * 
 * (C) 2017 pEp Security S.A.  All rights reserved.
 *
 * License: See LICENSE.TXT
 *
 * This is the public C API for libAccountSettings.
 * There is a C++ API in account_settings.hh
 *
 */

#ifndef PEP_ACCOUNT_SETTINGS_C_H_
#define PEP_ACCOUNT_SETTINGS_C_H_

#include "account_settings_common.h"

#ifdef __cplusplus
	extern "C" {
#endif

//! from user-level code: just an opaque handle to access the account settings
struct AccountSettings;

//! from user-level code: just an opaque handle to access certain server parameters (hostname, port number, access methods etc.)
struct AS_Server;


/** Release any memory that might be occupied by dynamically created AccountSetting objects.
 * Don't access the pointee after this call!
 * @param  account_settings  a pointer to an object created by get_account_settings() or a NULL pointer (which is a safe no-op).
 */
void free_account_settings(const struct AccountSettings* account_settings);

/** get account settings - the main API function. @see AS_FLAGS
 *
 * @param accountName  the accountName, e.g. an e-mail address. MUST NOT be NULL.
 * @param provider     the name of a provider. May be NULL if unknown. @see get_known_providers()
 * @param flags        the flags @see AS_FLAGS
 * @param credentials  additional credentials that might be necessary to retrieve the account settings. Depends on AS_FLAGS, may be NULL.
 * @return             the requested settings or NULL (only in case of out-of-memory)
 *                     Don't forget to call free_account_settings() when the result is no longer needed. free_account_settings(NULL) is a safe no-op.
 */
const struct AccountSettings* get_account_settings(const char* accountName, const char* provider, AS_FLAGS flags, const void* credentials);


/** get the status of the account_settings
 *
 * @param account_settings  the account settings your want to get the status for
 * @return the status. Only if it is AS_OK the other query functions will return useful results!
 */
AS_STATUS AS_get_status(const struct AccountSettings* account_settings);


/**  get the server settings for "incoming" messages (e.g. IMAP or POP3)
 * @param  accountSettings  guess what
 * @return the server for incoming messages associated with the accountSettings.
           The pointer points to internal r/o data in accountSettings, do not delete it!
 */
const struct AS_Server* AS_get_incoming(const struct AccountSettings* accountSettings);


/**  get the server settings for "outgoing" messages (e.g. SMTP)
 * @param  accountSettings  guess what
 * @return the server for outgoing messages associated with the accountSettings.
           The pointer points to internal r/o data in accountSettings, do not delete it!
 */
const struct AS_Server* AS_get_outgoing(const struct AccountSettings* accountSettings);


/**  get the hostname of the server
 * @param  server  guess what
 * @return the hostname (DNS name or IP literal) of the server.
           The pointer points to internal r/o data in accountSettings, do not delete it!
 */
const char* AS_get_hostname(const struct AS_Server* server);


/**  get the port number where the server provides its service
 * @param  server  guess what
 * @return the numerical port number
 */
int AS_get_port(const struct AS_Server* server);


/**  get the access method that is provided by the server
 * @param  server  guess what
 * @return the combined access method (protocol, socket type, authentication type)
 */
AS_ACCESS AS_get_access_method(const struct AS_Server* server);


/**  get the username for authentication at the server
 * @param  server  guess what
 * @return the user name/login name
 */
const char* AS_get_username(const struct AS_Server* server);


#ifdef __cplusplus
	} // end of extern "C"
#endif

#endif // PEP_ACCOUNT_SETTINGS_C_H_
