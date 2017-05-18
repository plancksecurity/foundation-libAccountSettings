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

#endif /* ASProtocols_h */
