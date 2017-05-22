//
//  AccountSettingsServer.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings.h"
#import "ASProtocols.h"

@interface AccountSettingsServer: NSObject <AccountSettingsServerProtocol>

@property (nonatomic, readonly) NSInteger port;
@property (nonatomic, readonly, nonnull) NSString *hostname;
@property (nonatomic, readonly) AS_ACCESS accesMethod;
@property (nonatomic, readonly, nonnull) NSString *username;

@end
