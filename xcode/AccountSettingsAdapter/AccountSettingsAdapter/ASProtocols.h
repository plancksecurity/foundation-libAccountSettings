//
//  ASProtocols.h
//  AccountSettingsAdapter
//
//  Created by Dirk Zimmermann on 18.05.17.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#ifndef ASProtocols_h
#define ASProtocols_h

#import "account_settings.h"

@protocol AccountSettingsProviderProtocol

@property (nonatomic, nonnull) NSString *name;
@property (nonatomic, nonnull) NSString *provierDescription;

@end

@protocol AccountSettingsServerProtocol

@property (nonatomic, readonly) NSInteger port;
@property (nonatomic, readonly, nonnull) NSString *hostname;
@property (nonatomic, readonly) AS_ACCESS accesMethod;
@property (nonatomic, readonly) AS_USERNAME username;

@end

@protocol AccountSettingsProtocol

@property (nonatomic, readonly) AS_STATUS status;
@property (nonatomic, readonly, nonnull) id<AccountSettingsProviderProtocol> provider;
@property (nonatomic, readonly, nonnull) id<AccountSettingsServerProtocol> incoming;
@property (nonatomic, readonly, nonnull) id<AccountSettingsServerProtocol> outgoing;

@end

@protocol AccountSettingsFactoryProtocol

/**
 @parameter accountName name of the account that will be configured
 @parameter provider the name of the provider, may be null
 @parameter flags the way that the account settings object will retrive the account settings
 @parameter credentials might be necessary to retrieve the account settings. Depends on flags, may be NULL.
 @return An object that wraps account configuration functions
 */
- (id<AccountSettingsProtocol> _Nonnull)accountSettingsWithAccountName:(NSString * _Nonnull)accountName
                                                              provider:(NSString * _Nullable)provider
                                                                 flags:(AS_FLAGS)flags
                                                           credentials:(void * _Nullable)credentials;
@end

#endif /* ASProtocols_h */
