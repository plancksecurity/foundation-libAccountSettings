//
//  AccountSettingsAdapter.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import "AccountSettings.h"
#import "account_settings.h"

@interface ASAccountSettings ()

@property (nonatomic, nonnull) struct AccountSettings *as;

- (instancetype) initWithAccountSettings:(struct AccountSettings * _Nonnull)internalAccountSettings;

@end

@implementation ASAccountSettings

+ (ASAccountSettings * _Nullable)aSAccountSettingsWithAccountName:(NSString * _Nonnull)accountName
                                                         provider:(NSString * _Nullable)provider
                                                            flags:(AS_FLAGS)flags
                                                      credentials:(void * _Nullable)credentials
{
    struct AccountSettings *as = get_account_settings([[accountName precomposedStringWithCanonicalMapping] UTF8String], [[provider precomposedStringWithCanonicalMapping] UTF8String], flags, credentials);

    ASAccountSettings *acountsettings = [[ASAccountSettings alloc] initWithAccountSettings:(as)];

    return acountsettings;
}



@end
