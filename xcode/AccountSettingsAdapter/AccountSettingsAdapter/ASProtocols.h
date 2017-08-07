//
//  ASProtocols.h
//  AccountSettingsAdapter
//
//  Created by Dirk Zimmermann on 18.05.17.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#ifndef ASProtocols_h
#define ASProtocols_h

#import "account_settings_common.h"

typedef NS_ENUM(NSInteger, AccountSettingsServerProtocolType)
{
    AccountSettingsServerTypePOP3,
    AccountSettingsServerTypeIMAP,
    AccountSettingsServerTypeSMTP
};

typedef NS_ENUM(NSInteger, AccountSettingsServerTransport)
{
    AccountSettingsServerTransportPlain,
    AccountSettingsServerTransportStartTLS,
    AccountSettingsServerTransportTLS
};

typedef NS_ENUM(NSInteger, AccountSettingsServerAuthMethod)
{
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

@property (nonatomic, readonly) AS_STATUS status;
@property (nonatomic, readonly, nonnull) id<AccountSettingsServerProtocol> incoming;
@property (nonatomic, readonly, nonnull) id<AccountSettingsServerProtocol> outgoing;

@end

#endif /* ASProtocols_h */
