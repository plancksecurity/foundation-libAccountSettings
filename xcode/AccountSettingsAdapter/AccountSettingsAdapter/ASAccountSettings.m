//
//  AccountSettingsAdapter.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import "ASAccountSettings.h"

@interface AccountSettingsProvider ()

- (instancetype)initWithName:(NSString * _Nonnull)name description:(NSString * _Nonnull)description;

@end

@interface ASAccountSettings ()

@property (nonatomic, nonnull) const struct AccountSettings *accountSettings;

- (instancetype)initWithAccountSettings:(const struct AccountSettings * _Nonnull)accountSettings;

@end

@implementation ASAccountSettings

- (instancetype)initWithAccountSettings:(const struct AccountSettings * _Nonnull)accountSettings
{
    if (self = [super init]) {
        _accountSettings = accountSettings;
    }
    return self;
}

- (void)dealloc
{
    free_account_settings(self.accountSettings);
}

+ (ASAccountSettings * _Nonnull)accountSettingsWithAccountName:(NSString * _Nonnull)accountName
                                                      provider:(NSString * _Nullable)provider
                                                         flags:(AS_FLAGS)flags
                                                   credentials:(void * _Nullable)credentials
{
    const struct AccountSettings *as =
    get_account_settings([[accountName
                           precomposedStringWithCanonicalMapping] UTF8String],
                         [[provider precomposedStringWithCanonicalMapping] UTF8String],
                         flags, credentials);

    ASAccountSettings *acountsettings = [[ASAccountSettings alloc] initWithAccountSettings:(as)];

    return acountsettings;
}

- (AS_STATUS)status
{
    return AS_get_status(self.accountSettings);
}

- (AccountSettingsProvider *)provider
{
    const as_provider *asp = AS_get_provider(self.accountSettings);

    return [[AccountSettingsProvider alloc] initWithName:@"" description:@""];
}

/*
- (AccountSettingsProvider) incoming
{
    return NULL
}

- (AccountSettingsProvider) outgoing
{
    return NULL
}
 */

@end
