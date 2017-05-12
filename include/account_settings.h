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

#ifndef PEP_ACCOUNT_SETTINGS_H_
#define PEP_ACCOUNT_SETTINGS_H_

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


typedef enum _AS_STATUS
{
	AS_OK            = 0,    ///< account settings were found (or guessed). Please go on and query them. :-)
	AS_TIMEOUT       = 900,  ///< the query to a remote service to get the account settings timed out. You might try it again later.
	AS_NOT_FOUND     = 950,  ///< for the given parameters & flags no account settings could be found
	AS_ILLEGAL_VALUE = 999,  ///< one or more parameters of get_account_settings() was/were invalid
} AS_STATUS;


//! The flags control the methods how get_account_settings() shall retrieve the account settings
typedef enum _AS_FLAGS
{
	AS_FLAG_USE_LOCAL_ISP_DB   = 0x0010, ///<  use local ISP database
	AS_FLAG_USE_HEURISTICS     = 0x0020, ///<  use heuristics from account's domain name
	AS_FLAG_USE_ANY_LOCAL      = 0x00FF, ///<  use all local (without communication to 3rd parties) methods in ascending order
	AS_FLAG_USE_DNS_AUTOCONFIG = 0x0100, ///<  fetch account config from "autodiscover/autoconfig" servers
	AS_FLAG_USE_ONLINE_ISP_DB  = 0x0800, ///<  ask https://live.mozillamessaging.com/autoconfig/v1.1/$DOMAINNAME
	AS_FLAG_USE_ANY            = 0xFFFF  ///<  try first local, than network methods
} AS_FLAGS;


//! Combination of protocol, socket type & authentication type
typedef enum _AS_ACCESS
{
	/// protocols
	AS_PROTO_POP3         = 0x00011,  ///< incoming
	AS_PROTO_IMAP         = 0x00012,  ///< incoming
	AS_PROTO_SMTP         = 0x00021,  ///< outgoing
	AS_PROTO_BITMASK      = 0x000FF,  ///< bitmask for AS_PROTO...
	
	/// socket types
	AS_SOCK_PLAIN         = 0x10000,
	AS_SOCK_STARTTLS      = 0x40000,
	AS_SOCK_SSL           = 0x80000,
	AS_SOCK_BITMASK       = 0xF0000,  ///< bitmask for AS_SOCK...

	/// authentication types
	AS_AUTH_NONE          = 0x00100,
	AS_AUTH_CLIENT_IP     = 0x00500,
	AS_AUTH_PW_CLEARTEXT  = 0x01000,  ///< was written as "plain" in older XML files
	AS_AUTH_PW_ENCRYPTED  = 0x02000,
	AS_AUTH_OAUTH2        = 0x05000,
	AS_AUTH_BITMASK       = 0x0FF00,  ///< bitmask for AS_AUTH...
	
} AS_ACCESS;


//! How the username is built:
typedef enum _AS_USERNAME
{
	AS_USERNAME_NONE = 0x4000,            ///< no username, no authentication?
	AS_USERNAME_EMAIL_ADDRESS = 0x4001,
	AS_USERNAME_EMAIL_LOCALPART = 0x4002,
	AS_USERNAME_EMAIL_LOCALPART_DOMAIN = 0x4003,
	AS_USERNAME_OTHER = 0x4fff,  ///< the username is none of the above (i.e. might be a numerical user ID etc.) and must be given by the user itself
} AS_USERNAME;


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


typedef struct _as_provider
{
	const char* name;        ///< a short and unique name of the provider. Can be used as provider name in get_account_settings().
	const char* description; ///< a more verbose and human readable description of the provider
} as_provider;


/** get list of known ISPs
 *  @return  a pointer to a static array of known providers. The array is terminated by a {NULL,NULL} entry.
             The pointer points to internal r/o data of the library, do not delete it!
 */
const as_provider* AS_get_known_providers();


/** get provider
 *
 * @param  accountSettings  guess what
 * @return the provider associated with the accountSettings.
           The pointer points to internal r/o data in accountSettings, do not delete it!
 */
const as_provider* AS_get_provider(const struct AccountSettings* accountSettings);


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

#endif // PEP_ACCOUNT_SETTINGS_H_
