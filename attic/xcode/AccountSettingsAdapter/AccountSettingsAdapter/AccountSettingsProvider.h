//
//  AccountSettingsProvider.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 28/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings.h"

@interface AccountSettingsProvider : NSObject

+(id) getKnownProviders;

@end
