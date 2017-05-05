//
//  AccountSettingsProvider+Internal.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 04/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings.h"

@interface AccountSettingsProvider ()

- (instancetype _Nonnull )initWithName:(NSString * _Nonnull)name
                           description:(NSString * _Nonnull)description;

- (instancetype _Nonnull)initWithProvider:(const as_provider *_Nonnull)provider;

@end
