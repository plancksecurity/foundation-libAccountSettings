//
//  ASProtocols.h
//  AccountSettingsAdapter
//
//  Created by Dirk Zimmermann on 18.05.17.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#ifndef ASProtocols_h
#define ASProtocols_h

#import "account_settings_c.h"

@protocol AccountSettingsServerProtocol

@property (nonatomic, readonly) NSInteger port;
@property (nonatomic, readonly, nonnull) NSString *hostname;
@property (nonatomic, readonly) AS_ACCESS accesMethod;
@property (nonatomic, readonly, nonnull) NSString *username;

@end

@protocol AccountSettingsProtocol

@property (nonatomic, readonly) AS_STATUS status;
@property (nonatomic, readonly, nonnull) id<AccountSettingsServerProtocol> incoming;
@property (nonatomic, readonly, nonnull) id<AccountSettingsServerProtocol> outgoing;

@end

#endif /* ASProtocols_h */
