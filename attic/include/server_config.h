//
//  server_config.h
//  ServerConfig
//
//  Created by Dirk Zimmermann on 17/01/2017.
//  Copyright © 2017 pEp Security S.A. All rights reserved.
//

#ifndef server_config_h
#define server_config_h

#include <stdio.h>

#define CLEAR(A, T) memset((A), 0, sizeof(T));

typedef enum _tlsconfig {
    tlsconfig_NONE = 0,
    tlsconfig_TLS,
    tlsconfig_STARTTLS
} tlsconfig;

typedef struct _connconfig {
    const char *name;
    unsigned int service; // see /etc/services
    unsigned int port; // real port to use
    tlsconfig tls;
} connconfig;

typedef union _cleartext_password {
    char *login;
    char *passwd;
} cleartext_password;

typedef enum _hashtype {
    hashtype_NONE = 0,
    hashtype_RIPEMD160,
    hashtype_SHA512,
    hashtype_SHA384,
    hashtype_SHA256,

    // deprecated
    hashtype_SHA1,
    hashtype_MD5
} hashtype;

typedef union _hashed_password {
    char *login;
    char *hash;
    hashtype type;
    size_t salt_len;
    char *pepper;
} hashed_password;

typedef union _credentials {
    cleartext_password cp;
    hashed_password hp;
} credentials;

typedef int preconfig; // number of pre-defined config, 0 for none

typedef struct _manualconfig {
    connconfig incoming;
    connconfig outgoing;
} manualconfig;

typedef struct _mailconfig {
    preconfig pc;
    manualconfig mc;
} mailconfig;

typedef struct _provider {
    const char *domain;
} provider;

typedef struct _account {
    /**
     This will be used primarily for detecting servers, needed
     for `probe_for_config`.
     */
    const char *email;

    /**
     This is only needed when using `probe_for_config_with_provider`.
     */
    provider prov;

    mailconfig conf;
    credentials login;
} account;

/**
 Contact servers and probe for config.
 - Returns: 0 on success, other values signify errors.
 */
int probe_for_config(account *a);

/**
 Contact servers and probe for config, take into account the given
 provider information.
 - Returns: 0 on success, other values signify errors.
 */
int probe_for_config_with_provider(account *a);

#endif /* server_config_h */
