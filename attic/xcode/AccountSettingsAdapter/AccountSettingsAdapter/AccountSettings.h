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

@interface AccountSettings : NSObject

+ (id _Nonnull )accountSettingsWithAccountName:(nonnull char*)accountName
                            provider:(char*_Nullable)provider
                               flags:(AS_FLAGS)flags credentials:(void*_Nullable)credentials;

- (void)freeAcountSettings;
- (AS_STATUS)getStatus;
- (AccountSettingsProvider *_Nonnull)getProvider;
- (AccountSettingsServer *_Nonnull)getIncoming;
- (AccountSettingsServer *_Nonnull)getOutgoing;

@end

