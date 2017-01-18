//
//  ServerConfig.m
//  ServerConfig
//
//  Created by Dirk Zimmermann on 18/01/2017.
//  Copyright © 2017 pEp Security S.A. All rights reserved.
//

#import "ServerConfig.h"

@implementation SCConnConfig

- (instancetype _Nonnull)initFromC:(connconfig)config
{
    if (self = [super init]) {
        [self set:config];
    }
    return self;
}

- (void)set:(connconfig)obj
{
    if (obj.name) {
        self.name = [[NSString alloc] initWithUTF8String:obj.name];
    } else {
        self.name = nil;
    }
    self.service = obj.service;
    self.port = obj.port;
}

- (connconfig)cObj
{
    connconfig cfg = {
        self.name.UTF8String, (unsigned int) self.service, (unsigned int) self.port
    };
    return cfg;
}

@end

@implementation SCManualConfig

- (instancetype _Nonnull)initFromC:(manualconfig)manualconfig
{
    if (self = [super init]) {
        [self set:manualconfig];
    }
    return self;
}

- (void)set:(manualconfig)obj
{
    self.incoming = [[SCConnConfig alloc] initFromC:obj.incoming];
    self.outgoing = [[SCConnConfig alloc] initFromC:obj.outgoing];
}

- (manualconfig)cObj
{
    manualconfig cfg = { self.incoming.cObj, self.outgoing.cObj };
    return cfg;
}

@end

@implementation SCMailConfig

- (instancetype _Nonnull)initFromC:(mailconfig)mailconfig
{
    if (self = [super init]) {
        [self set:mailconfig];
    }
    return self;
}

- (void)set:(mailconfig)obj
{
    self.pc = obj.pc;
    self.mc = [[SCManualConfig alloc] initFromC:obj.mc];
}

- (mailconfig)cObj
{
    mailconfig cfg = { self.pc, self.mc.cObj };
    return cfg;
}

@end

@implementation SCProvider

- (instancetype _Nonnull)initFromC:(provider)provider
{
    if (self = [super init]) {
        [self set:provider];
    }
    return self;
}

- (void)set:(provider)obj
{
    if (obj.domain) {
        self.domain = [[NSString alloc] initWithUTF8String:obj.domain];
    } else {
        self.domain = nil;
    }
}

- (provider)cObj
{
    provider p = { self.domain.UTF8String };
    return p;
}

@end

@implementation SCAccount

- (instancetype _Nonnull)initFromC:(account)account
{
    if (self = [super init]) {
        [self set:account];
    }
    return self;
}

- (void)set:(account)obj
{
    self.email = [[NSString alloc] initWithUTF8String:obj.email];
    self.provider = [[SCProvider alloc] initFromC:obj.prov];
    self.mailConfig = [[SCMailConfig alloc] initFromC:obj.conf];
}

- (account)cObj
{
    account a = { self.email.UTF8String, self.provider.cObj, self.mailConfig.cObj };
    return a;
}

- (NSInteger)probe
{
    account a = self.cObj;
    int res = probe_for_config(&a);
    [self set:a];
    return res;
}

- (NSInteger)probeWithProvider
{
    account a = self.cObj;
    int res = probe_for_config_with_provider(&a);
    [self set:a];
    return res;
}

@end
