/**
 * Account Settings - a small library to retrieve & guess settings for e-mail and other accounts
 * 
 * (C) 2017 pEp Security S.A.  All rights reserved.
 *
 * License: See LICENSE.TXT
 *
 * This header contains the common part of the C, C++ and ObjC API for libAccountSettings.
 *
 * The C API is in account_settings_c.h
 *
 */

#ifndef PEP_ACCOUNT_SETTINGS_COMMONH_
#define PEP_ACCOUNT_SETTINGS_COMMONH_

#ifdef __cplusplus
	extern "C" {
#endif

typedef enum _AS_STATUS
{
	AS_OK            = 100,  ///< account settings were found (or guessed). Please go on and query them. :-)
	AS_TIMEOUT       = 900,  ///< the query to a remote service to get the account settings timed out. You might try it again later.
	AS_NOT_FOUND     = 950,  ///< for the given parameters & flags no account settings could be found
	AS_ILLEGAL_VALUE = 999,  ///< one or more parameters of get_account_settings() was/were invalid
} AS_STATUS;


//! The flags control the methods how get_account_settings() shall retrieve the account settings
typedef enum _AS_FLAGS
{
	AS_FLAG_USE_LOCAL_ISP_DB   = 0x0010, ///<  use local ISP database
// FIXME: local-only heuristics? Is that possible/sensible?
//	AS_FLAG_USE_HEURISTICS     = 0x0020, ///<  use heuristics from account's domain name
	AS_FLAG_USE_ANY_LOCAL      = 0x000F, ///<  use all local (without communication to 3rd parties) methods in ascending order
	
	AS_FLAG_USE_MOZ_AUTOCONFIG  = 0x0100, ///<  fetch account config via Mozilla's "autoconfig" method
	AS_FLAG_USE_MS_AUTODISCOVER = 0x0200, ///<  fetch account config via Microsoft's "autodiscover" method
	AS_FLAG_USE_DNS_SRV         = 0x0400, ///<  use DNS SRV records for the mail domain
	AS_FLAG_USE_ONLINE_ISP_DB   = 0x0800, ///<  ask https://live.mozillamessaging.com/autoconfig/v1.1/$DOMAINNAME
	AS_FLAG_USE_ANY_ONLINE      = 0x0F00, ///<  try all (almost) reliable online configuration methods in ascending order
	
	AS_FLAG_USE_GUESSING        = 0x1000, ///<  guess server names and try to detect its login methods. MAY BE DANGEROUS!
	
	AS_FLAG_USE_ANY             = 0xFFFF  ///<  try first local, than network methods
} AS_FLAGS;


//! Combination of protocol, socket type & authentication type
typedef enum _AS_ACCESS
{
	/// protocols
	AS_PROTO_EXCHANGE     = 0x00008,  ///< both (?) - selected with very low precedence
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


#ifdef __cplusplus
	} // end of extern "C"
#endif

#endif // PEP_ACCOUNT_SETTINGS_COMMON_H_
