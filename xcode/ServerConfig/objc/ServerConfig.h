//
//  ServerConfig.h
//  ServerConfig
//
//  Created by Dirk Zimmermann on 18/01/2017.
//  Copyright © 2017 pEp Security S.A. All rights reserved.
//

#import <Foundation/Foundation.h>

//! Project version number for ServerConfig.
FOUNDATION_EXPORT double ServerConfigVersionNumber;

//! Project version string for ServerConfig.
FOUNDATION_EXPORT const unsigned char ServerConfigVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <ServerConfig/PublicHeader.h>

#import "server_config.h"

@interface SCConnConfig : NSObject

@property (nonatomic, nullable) NSString *name;
/** See /etc/service */
@property NSUInteger service;
@property NSUInteger port;

- (instancetype _Nonnull)initFromC:(connconfig)config;
- (void)set:(connconfig)obj;
- (connconfig)cObj;

@end

@interface SCManualConfig : NSObject

@property (nonatomic, nullable) SCConnConfig *incoming;
@property (nonatomic, nullable) SCConnConfig *outgoing;

- (instancetype _Nonnull)initFromC:(manualconfig)manualconfig;
- (void)set:(manualconfig)obj;
- (manualconfig)cObj;

@end

@interface SCMailConfig : NSObject

@property preconfig pc;
@property (nonatomic, nullable) SCManualConfig *mc;

- (instancetype _Nonnull)initFromC:(mailconfig)mailconfig;
- (void)set:(mailconfig)obj;
- (mailconfig)cObj;

@end

@interface SCProvider : NSObject

@property (nonatomic, nonnull) NSString *domain;

- (instancetype _Nonnull)initFromC:(provider)provider;
- (void)set:(provider)obj;
- (provider)cObj;

@end

@interface SCAccount : NSObject

@property (nonatomic, nonnull) NSString *email;
@property (nonatomic, nullable) SCProvider *provider;
@property (nonatomic, nullable) SCMailConfig *mailConfig;

- (instancetype _Nonnull)initFromC:(account)account;
- (void)set:(account)obj;
- (account)cObj;

@end

@interface ServerConfig : NSObject

+ (NSInteger)probeAccount:(SCAccount  * _Nonnull)account;
+ (NSInteger)probeAccountWithProvider:(SCAccount  * _Nonnull)account;

@end
