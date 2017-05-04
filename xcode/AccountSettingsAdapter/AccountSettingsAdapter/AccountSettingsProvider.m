//
//  AccountSettingsProvider.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 04/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "AccountSettingsProvider.h"

@interface AccountSettingsProvider ()

- (instancetype)initWithName:(NSString * _Nonnull)name description:(NSString * _Nonnull)description;

@end

@implementation AccountSettingsProvider

- (instancetype)initWithName:(NSString * _Nonnull)name description:(NSString * _Nonnull)description
{
    if (self = [super init]) {
        _name = name;
        _provierDescription = description;
    }
    return self;
}

@end
