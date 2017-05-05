//
//  AccountSettingsAdapter.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import "ASAccountSettings.h"
#import "AccountSettingsProvider+Internal.h"

AccountSettingsProvider * convertASProviderToAccountSettingsProvider(const struct _as_provider * provider);

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
    const struct _as_provider *asp = AS_get_provider(self.accountSettings);

    return convertASProviderToAccountSettingsProvider(asp);
}

/*
- (AccountSettingsServer *) incoming
{

}

- (AccountSettingsServer *) outgoing
{
    return NULL
}
 */

@end

AccountSettingsProvider * convertASProviderToAccountSettingsProvider(const struct _as_provider * provider)
{
    NSString *name = [NSString stringWithUTF8String: provider->name];
    NSString *description = [NSString stringWithUTF8String: provider->description];

    return [[AccountSettingsProvider alloc] initWithName:name description:description];
}
/*
AccountSettingsServer * convertASServerToAccountSettingsServer(const struct AS_Server * server)
{

    return [[AccountSettingsProvider alloc] initWithName:name description:description];
}
*/
