//
//  AccountSettingsServer.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 03/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "AccountSettingsServer.h"
#import "account_settings.h"

@interface AccountSettingsServer ()

@property (nonatomic, nonnull) const struct AS_Server *accountServer;

@end

@implementation AccountSettingsServer

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

- (AS_ACCESS)accesMethod
{
    AS_ACCESS value = AS_get_access_method(self.accountServer);
    return value;
}

- (AS_USERNAME)username
{
    AS_USERNAME value = AS_get_username(self.accountServer);
    return value;
}
@end
