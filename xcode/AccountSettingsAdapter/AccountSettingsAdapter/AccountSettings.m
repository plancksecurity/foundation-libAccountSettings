//
//  AccountSettings.m
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import "AccountSettings.h"

#import "account_settings_c.h"

#import "AccountSettingsServer+Internal.h"

@interface AccountSettings ()

@property (nonatomic, nullable) const struct AccountSettings *accountSettings;
@property (nonatomic, nullable) void *credentials;
@property (nonatomic, nonnull) dispatch_queue_t backgroundQueue;

@end

@implementation AccountSettings

- (instancetype _Nonnull )initWithAccountName:(NSString * _Nonnull)accountName
                                     provider:(NSString * _Nullable)provider
                                        flags:(AS_FLAGS)flags
                                  credentials:(void * _Nullable)credentials
{
    if (self = [super init]) {
        _accountName = accountName;
        _provider = provider;
        _flags = flags;
        _credentials = credentials;
        _backgroundQueue = dispatch_queue_create("AccountSettings.backgroundQueue",
                                                 DISPATCH_QUEUE_CONCURRENT);
    }
    return self;
}

- (void)lookup
{
    const struct AccountSettings *as =
    get_account_settings([[self.accountName
                           precomposedStringWithCanonicalMapping] UTF8String],
                         [[self.provider precomposedStringWithCanonicalMapping] UTF8String],
                         self.flags, self.credentials);
    self.accountSettings = as;
}

- (void)lookupCompletion:(AccountSettingsCompletionBlock _Nullable)completionBlock
{
    dispatch_async(self.backgroundQueue, ^ {
        __weak typeof(self) weakSelf = self;
        [weakSelf lookup];
        if (weakSelf) {
            completionBlock(weakSelf);
        }
    });
}

- (void)dealloc
{
    free_account_settings(self.accountSettings);
}

+ (AccountSettings * _Nonnull)accountSettingsWithAccountName:(NSString * _Nonnull)accountName
                                                    provider:(NSString * _Nullable)provider
                                                       flags:(AS_FLAGS)flags
                                                 credentials:(void * _Nullable)credentials
{
    return [[AccountSettings alloc]
            initWithAccountName:accountName provider:provider
            flags:flags credentials:credentials];
}

- (AS_STATUS)status
{
    if (self.accountSettings) {
        return AS_get_status(self.accountSettings);
    } else {
        return AS_ILLEGAL_VALUE;
    }
}

- (AccountSettingsServer *)incoming
{
    const struct AS_Server *asIncoming = AS_get_incoming(self.accountSettings);
    return [[AccountSettingsServer alloc] initWithAccountServer:asIncoming];

}

- (AccountSettingsServer *)outgoing
{
    const struct AS_Server *asOutgoing = AS_get_outgoing(self.accountSettings);
    return [[AccountSettingsServer alloc] initWithAccountServer:asOutgoing];
}

- (BOOL)supportsOAuth2
{
    AccountSettingsServer *theIncoming = self.incoming;
    AccountSettingsServer *theOutgoing = self.outgoing;
    if (theIncoming && theOutgoing &&
        theIncoming.authMethod == AccountSettingsServerAuthMethodOAUTH2 &&
        theOutgoing.authMethod == AccountSettingsServerAuthMethodOAUTH2) {
        return YES;
    }
    return NO;
}

@end
