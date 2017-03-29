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

struct AccountSettings;

AccountSettings* create_account_settings(void);

void free_account_settings(AccountSettings* account_settings);


typedef enum _AS_STATUS
{
	AS_OK = 0,
	AS_ILLEGAL_VALUE = -100,
	AS_OUT_OF_MEMORY = -110,
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
	// protocols
	AS_PROTO_POP3         = 0x11000,  // incoming
	AS_PROTO_IMAP         = 0x12000,  // incoming
	AS_PROTO_SMTP         = 0x21000,  // outgoing
	AS_PROTO_BITMASK      = 0xFF000,  // bitmask for AS_PROTO...
	
	// socket types
	AS_SOCK_PLAIN         = 0x00100,
	AS_SOCK_STARTTLS      = 0x00400,
	AS_SOCK_SSL           = 0x00800,
	AS_SOCK_BITMASK       = 0x00F00,  // bitmask for AS_SOCK...

	// authentication types
	AS_AUTH_NONE          = 0x00001,
	AS_AUTH_CLIENT_IP     = 0x00005,
	AS_AUTH_PLAIN         = 0x00010,  // TODO: is "plain" and "password-cleartext" the same?
	AS_AUTH_PW_CLEARTEXT  = 0x00011,
	AS_AUTH_PW_ENCRYPTED  = 0x00020,
	AS_AUTH_OAUTH2        = 0x00050,
	AS_AUTH_BITMASK       = 0x000FF,  // bitmask for AS_AUTH...
	
} AS_ACCESS;


//! How the username is built:
typedef enum _AS_USERNAME
{
	AS_USERNAME_NONE = 0x4000, // no username, no authentication?
	AS_USERNAME_EMAIL_ADDRESS = 0x4001,
	AS_USERNAME_EMAIL_LOCALPART = 0x4002,
	AS_USERNAME_EMAIL_LOCALPART_DOMAIN = 0x4003,
} AS_USERNAME;



/** get account settings - the main API function.
 *
 * @param accountName  the accountName, e.g. an e-mail address. MUST NOT be NULL.
 * @param provider     the name of a provider. May be NULL if unknown. @see get_known_providers()
 * @param flags        @see AS_FLAGS
 * @param credentials  additional credentials that might be necessary to retrieve the account settings. Depends on AS_FLAGS, may be NULL.
 * @param out          output parameter holding the account settings, if return value is AS_OK, unchanged if an error occurs.
 *                     all text strings in the AccountSettings are allocated with malloc() and have to be free()'s by the user
 *                     or call account_settings_free().
 */
AS_STATUS get_account_settings(const char* accountName, const char* provider, AS_FLAGS flags, const void* credentials, AccountSettings* out);


typedef struct _as_provider
{
	const char* name;        ///< a short and unique name of the provider. Can be used as provider name in get_account_settings().
	const char* description; ///< a more verbose and human readable description of the provider
} as_provider;

/** get list of known ISPs
 *  @return a pointer to a static array of known providers. The array is terminated by a {NULL,NULL} entry.
 */
const as_provider* get_known_providers();


/** get provider
 *
 * @param  accountSettings  guess what
 * @return the provider associated with the accountSettings. The pointer points to internal r/o data in accountSettings, do not delete it!
 */
const as_provider* get_provider(const AccountSettings* accountSettings);



#ifdef __cplusplus
	} // end of extern "C"
#endif

#endif // PEP_ACCOUNT_SETTINGS_H_
