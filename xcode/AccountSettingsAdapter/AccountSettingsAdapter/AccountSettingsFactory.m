//
//  AccountSettingsFactory.m
//  AccountSettingsAdapter
//
//  Created by Dirk Zimmermann on 18.05.17.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import "AccountSettingsFactory.h"

#import "ASAccountSettings.h"
#import "ASAccountSettings+Internal.h"

@implementation AccountSettingsFactory

- (id<AccountSettingsProtocol>_Nonnull)accountSettingsWithAccountName:(NSString * _Nonnull)accountName
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

@end
