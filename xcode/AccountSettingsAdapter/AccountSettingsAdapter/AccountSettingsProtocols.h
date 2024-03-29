//
//  AccountSettingsProtocols.h
//  AccountSettingsAdapter
//
//  Created by Dirk Zimmermann on 18.05.17.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#ifndef AccountSettingsProtocols_h
#define AccountSettingsProtocols_h

#import "account_settings_common.h"

extern NSString * const _Nonnull AccountSettingsProviderIDGmail;
extern NSString * const _Nonnull AccountSettingsProviderIDYahoo;

typedef NS_ENUM(NSInteger, AccountSettingsServerProtocolType)
{
    AccountSettingsServerTypeUnknown,
    AccountSettingsServerTypePOP3,
    AccountSettingsServerTypeIMAP,
    AccountSettingsServerTypeSMTP
};

typedef NS_ENUM(NSInteger, AccountSettingsServerTransport)
{
    AccountSettingsServerTransportUnknown,
    AccountSettingsServerTransportPlain,
    AccountSettingsServerTransportStartTLS,
    AccountSettingsServerTransportTLS
};

typedef NS_ENUM(NSInteger, AccountSettingsServerAuthMethod)
{
    AccountSettingsServerAuthMethodUnknown,
    AccountSettingsServerAuthMethodNone,
    AccountSettingsServerAuthMethodClientIP,
    AccountSettingsServerAuthMethodPasswordClearText,
    AccountSettingsServerAuthMethodPasswordEncrypted,
    AccountSettingsServerAuthMethodOAUTH2
};

@protocol AccountSettingsServerProtocol

@property (nonatomic, readonly) NSInteger port;
@property (nonatomic, readonly, nonnull) NSString *hostname;
@property (nonatomic, readonly, nonnull) NSString *username;
@property (nonatomic, readonly) AccountSettingsServerProtocolType protocol;
@property (nonatomic, readonly) AccountSettingsServerTransport transport;
@property (nonatomic, readonly) AccountSettingsServerAuthMethod authMethod;

@end

@protocol AccountSettingsProtocol

/**
 Type for completion blocks into this library.
 */
typedef void (^AccountSettingsCompletionBlock)(id<AccountSettingsProtocol> _Nonnull);

@property (nonatomic, readonly, nonnull) NSString *accountName;

/** The provider name as provided by the client */
@property (nonatomic, readonly, nullable) NSString *provider;

 @property (nonatomic, readonly) AS_FLAGS flags;

/**
 Defined *after* lookup has finished.
 */
@property (nonatomic, readonly) AS_STATUS status;

/**
 Defined *after* lookup has finished, and status is AS_OK.
 */
@property (nonatomic, readonly, nullable) id<AccountSettingsServerProtocol> incoming;

/**
 Defined *after* lookup has finished, and status is AS_OK.
 */
@property (nonatomic, readonly, nullable) id<AccountSettingsServerProtocol> outgoing;

/**
 Tries to lookup account data based on the parameters given in init.
 @Note This will make blocking network access.
 */
- (void)lookup;

/**
 Will make an asynchronous lookup and call the provided block when finished.
 */
- (void)lookupCompletion:(AccountSettingsCompletionBlock _Nullable)completionBlock;

/**
 Does the given provider support OAuth2?
 */
@property (nonatomic, readonly) BOOL supportsOAuth2;

/**
 The provider ID, which might be set for local DB lookups.
 */
@property (nonatomic, readonly, nullable) NSString *providerID;

@end

#endif /* AccountSettingsProtocols_h */
