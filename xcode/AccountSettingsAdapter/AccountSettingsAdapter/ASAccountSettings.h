//
//  AccountSettingsAdapter.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings.h"
#import "AccountSettingsServer.h"
#import "AccountSettingsProvider.h"

@interface ASAccountSettings: NSObject

/**
 Static constructor for the object that wraps account configuration functions
 accountName: name of the account that will be configured
 provider: the name of the provider, may be null
 flags: the way that the account settings object will retrive the account settings
 credentials:  might be necessary to retrieve the account settings. Depends on flags, may be NULL.
 */
+ (ASAccountSettings * _Nonnull)accountSettingsWithAccountName:(NSString * _Nonnull)accountName
                                                      provider:(NSString * _Nullable)provider
                                                         flags:(AS_FLAGS)flags
                                                   credentials:(void * _Nullable)credentials;

@property (nonatomic, readonly) AS_STATUS status;

@property (nonatomic, readonly, nonnull) AccountSettingsProvider *provider;

@property (nonatomic, readonly, nonnull) AccountSettingsServer *incoming;

@property (nonatomic, readonly, nonnull) AccountSettingsServer *outgoing;

@end

