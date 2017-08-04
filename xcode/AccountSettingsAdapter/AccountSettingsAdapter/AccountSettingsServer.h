//
//  AccountSettingsServer.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings_common.h"
#import "ASProtocols.h"

typedef NS_ENUM(NSInteger, AccountSettingsServerProtocol)
{
    AccountSettingsServerProtocolPOP3,
    AccountSettingsServerProtocolIMAP,
    AccountSettingsServerProtocolSMTP
};

typedef NS_ENUM(NSInteger, AccountSettingsServerSockType)
{
    AccountSettingsServerSockTypePlain,
    AccountSettingsServerSockTypeStartTLS,
    AccountSettingsServerSockTypeTLS
};

typedef NS_ENUM(NSInteger, AccountSettingsServerAuth)
{
    AccountSettingsServerAuthNone,
    AccountSettingsServerAuthClientIP,
    AccountSettingsServerAuthPasswordClearText,
    AccountSettingsServerAuthPasswordEncrypted,
    AccountSettingsServerAuthOAUTH2
};

@interface AccountSettingsServer: NSObject <AccountSettingsServerProtocol>

@property (nonatomic, readonly) NSInteger port;
@property (nonatomic, readonly, nonnull) NSString *hostname;
@property (nonatomic, readonly) AS_ACCESS accessMethod;
@property (nonatomic, readonly, nonnull) NSString *username;

@end
