//
//  AccountSettings.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "AccountSettingsServer.h"

#import "AccountSettingsProtocols.h"

@interface AccountSettings: NSObject <AccountSettingsProtocol>

/**
 @parameter accountName name of the account that will be configured
 @parameter provider the name of the provider, may be null
 @parameter flags the way that the account settings object will retrive the account settings
 @parameter credentials might be necessary to retrieve the account settings. Depends on flags, may be NULL.
 @return An object that wraps account configuration functions
 */
- (instancetype _Nonnull )initWithAccountName:(NSString * _Nonnull)accountName
                                     provider:(NSString * _Nullable)provider
                                        flags:(AS_FLAGS)flags
                                  credentials:(void * _Nullable)credentials;

@property (nonatomic, readonly, nonnull) NSString *accountName;
@property (nonatomic, readonly, nullable) NSString *provider;
@property (nonatomic, readonly) AS_FLAGS flags;

@property (nonatomic, readonly) AS_STATUS status;
@property (nonatomic, readonly, nullable) AccountSettingsServer *incoming;
@property (nonatomic, readonly, nullable) AccountSettingsServer *outgoing;

@end
