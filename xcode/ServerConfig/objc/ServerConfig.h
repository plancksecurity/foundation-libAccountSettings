//
//  ServerConfig.h
//  ServerConfig
//
//  Created by Dirk Zimmermann on 18/01/2017.
//  Copyright © 2017 pEp Security S.A. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "server_config.h"

@interface ConnConfig : NSObject

@property (nonatomic, nullable) NSString *name;
/** See /etc/service */
@property NSUInteger service;
@property NSUInteger port;

- (instancetype _Nonnull)initFromC:(connconfig)config;
- (void)set:(connconfig)obj;
- (connconfig)cObj;

@end

@interface ManualConfig : NSObject

@property (nonatomic, nullable) ConnConfig *incoming;
@property (nonatomic, nullable) ConnConfig *outgoing;

- (instancetype _Nonnull)initFromC:(manualconfig)manualconfig;
- (void)set:(manualconfig)obj;
- (manualconfig)cObj;

@end

@interface MailConfig : NSObject

@property preconfig pc;
@property (nonatomic, nullable) ManualConfig *mc;

- (instancetype _Nonnull)initFromC:(mailconfig)mailconfig;
- (void)set:(mailconfig)obj;
- (mailconfig)cObj;

@end

@interface Provider : NSObject

@property (nonatomic, nonnull) NSString *domain;

- (instancetype _Nonnull)initFromC:(provider)provider;
- (void)set:(provider)obj;
- (provider)cObj;

@end

@interface Account : NSObject

@property (nonatomic, nonnull) NSString *email;
@property (nonatomic, nullable) Provider *provider;
@property (nonatomic, nullable) MailConfig *mailConfig;

- (instancetype _Nonnull)initFromC:(account)account;
- (void)set:(account)obj;
- (account)cObj;

@end

@interface ServerConfig : NSObject

+ (NSInteger)probeAccount:(Account  * _Nonnull)account;
+ (NSInteger)probeAccountWithProvider:(Account  * _Nonnull)account;

@end
