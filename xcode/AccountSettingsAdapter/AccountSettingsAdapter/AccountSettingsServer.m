//
//  AccountSettingsServer.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 03/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "AccountSettingsServer.h"
#import "AccountSettingsServer+Internal.h"
#import "account_settings_c.h"

@interface AccountSettingsServer ()

@property (nonatomic, nonnull) const struct AS_Server *accountServer;

@end

@implementation AccountSettingsServer

- (instancetype _Nonnull)initWithAccountServer:(const struct AS_Server * _Nonnull)accountServer
{
    if (self = [super init]) {
        _accountServer = accountServer;
    }
    return self;
}

- (NSInteger)port
{
    int value = AS_get_port(self.accountServer);
    return (NSInteger)value;
}

- (NSString *)hostname
{
    NSString *value = [NSString stringWithUTF8String:AS_get_hostname(self.accountServer)];
    return value;
}

- (AS_ACCESS)accessMethod
{
    AS_ACCESS value = AS_get_access_method(self.accountServer);
    return value;
}

- (NSString *)username
{
    NSString *value = [NSString stringWithUTF8String:AS_get_username(self.accountServer)];
    return value;
}

- (AccountSettingsServerProtocolType)protocol
{
    AS_ACCESS flags = self.accessMethod & AS_PROTO_BITMASK;
    if (flags == AS_PROTO_POP3) {
        return AccountSettingsServerTypePOP3;
    }
    if (flags == AS_PROTO_IMAP) {
        return AccountSettingsServerTypeIMAP;
    }
    if (flags == AS_PROTO_SMTP) {
        return AccountSettingsServerTypeSMTP;
    }
    return AccountSettingsServerTypeUnknown;
}

- (AccountSettingsServerTransport)transport
{
    AS_ACCESS flags = self.accessMethod & AS_SOCK_BITMASK;
    if (flags == AS_SOCK_PLAIN) {
        return AccountSettingsServerTransportPlain;
    }
    if (flags == AS_SOCK_SSL) {
        return AccountSettingsServerTransportTLS;
    }
    if (flags == AS_SOCK_STARTTLS) {
        return AccountSettingsServerTransportStartTLS;
    }
    return AccountSettingsServerTransportUnknown;
}

- (AccountSettingsServerAuthMethod)authMethod
{
    AS_ACCESS flags = self.accessMethod & AS_AUTH_BITMASK;
    if (flags == AS_AUTH_NONE) {
        return AccountSettingsServerAuthMethodNone;
    }
    if (flags == AS_AUTH_OAUTH2) {
        return AccountSettingsServerAuthMethodOAUTH2;
    }
    if (flags == AS_AUTH_CLIENT_IP) {
        return AccountSettingsServerAuthMethodClientIP;
    }
    if (flags == AS_AUTH_PW_CLEARTEXT) {
        return AccountSettingsServerAuthMethodPasswordClearText;
    }
    if (flags == AS_AUTH_PW_ENCRYPTED) {
        return AccountSettingsServerAuthMethodPasswordEncrypted;
    }
    return AccountSettingsServerAuthMethodUnknown;
}

@end
