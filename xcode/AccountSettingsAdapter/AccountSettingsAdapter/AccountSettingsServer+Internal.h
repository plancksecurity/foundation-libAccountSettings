//
//  AccountSettingsServer+Internal.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 04/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings_common.h"
#import "account_settings_c.h"

@interface AccountSettingsServer ()

- (instancetype _Nonnull )initWithAccountServer:(const struct AS_Server * _Nonnull)accountServer;

@end
