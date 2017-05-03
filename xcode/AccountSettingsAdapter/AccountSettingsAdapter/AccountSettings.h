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


@interface AccountSettings: NSObject

/**
 Constructor for the object that wraps account configuration functions
 accountName: name of the account that will be configured
 provider: the name of the provider, may be null
 flags: the way that the account settings object will retrive the account settings
 credentials:  might be necessary to retrieve the account settings. Depends on flags, may be NULL.
 */
+ (AccountSettings * _Nullable)accountSettingsWithAccountName:(NSString * _Nonnull)accountName
                                                     provider:(NSString * _Nullable)provider
                                                        flags:(AS_FLAGS)flags
                                                  credentials:(void * _Nullable)credentials;

/**
 wrap method for Release any memory that might be occupied by dynamically created AccountSetting objects.
 */
- (void)freeAcountSettings;

/**
 wrap method for get the status of the account_settings
 */
- (AS_STATUS)getStatus;

/**
 wrap method for get provider
 */
- (AccountSettingsProvider * _Nonnull)getProvider;

/**
 wrap method for get the server settings for "incoming" messages
 */
- (AccountSettingsServer * _Nonnull)getIncoming;

/**
 wrap method for get the server settings for "outgoing" messages
 */
- (AccountSettingsServer * _Nonnull)getOutgoing;

@end

