//
//  AccountSettingsProvider.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 28/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings.h"
/**
 wrap object for _as_provider
 */
@interface AccountSettingsProvider : NSObject

/**
 a short and unique name of the provider
 */
- (NSString *) name;
/**
 a more verbose and human readable description of the provider
 */
- (NSString *) description;

/**
 wrap method for get list of known ISPs
 */
+(id) getKnownProviders;

@end
