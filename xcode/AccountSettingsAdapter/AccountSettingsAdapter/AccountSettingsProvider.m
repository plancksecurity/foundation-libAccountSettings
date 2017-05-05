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

- (instancetype _Nonnull)initWithProvider:(const as_provider *_Nonnull)provider
{
    NSString *name = [NSString stringWithUTF8String: provider->name];
    NSString *providerDescription = [NSString stringWithUTF8String: provider->description];

    return [self initWithName:name providerDescription:providerDescription];
}

@end
