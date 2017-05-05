//
//  AccountSettingsProvider.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 28/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "account_settings.h"

@interface AccountSettingsProvider: NSObject

@property (nonatomic, nonnull) NSString *name;
@property (nonatomic, nonnull) NSString *provierDescription;

/**
 method for get list of all known ISPs
 */
+ (NSArray<AccountSettingsProvider *> *_Nonnull)knownProviders;

@end
