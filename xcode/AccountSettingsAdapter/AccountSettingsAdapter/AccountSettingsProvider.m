//
//  AccountSettingsProvider.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 04/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "AccountSettingsProvider.h"

@implementation AccountSettingsProvider

- (instancetype)initWithName:(NSString * _Nonnull)name
         providerDescription:(NSString * _Nonnull)providerDescription
{
    if (self = [super init]) {
        _name = name;
        _provierDescription = providerDescription;
    }
    return self;
}

@end
