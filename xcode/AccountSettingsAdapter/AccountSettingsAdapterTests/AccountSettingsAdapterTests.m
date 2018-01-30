//
//  AccountSettingsAdapterTests.m
//  AccountSettingsAdapterTests
//
//  Created by Xavier Algarra on 08/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "AccountSettings.h"

@interface AccountSettingsAdapterTests : XCTestCase

@end

@implementation AccountSettingsAdapterTests

- (void)testQueryPeptest
{
    NSString *hostName = @"peptest.ch";
    [self testServerWithAddress:@"someone@peptest.ch" incomingHostName:hostName
                   incomingPort:993 incomingProtocol:AccountSettingsServerTypeIMAP
              incomingTransport:AccountSettingsServerTransportTLS
             incomingAuthMethod:AccountSettingsServerAuthMethodPasswordClearText
               outgoingHostName:hostName outgoingPort:587
               outgoingProtocol:AccountSettingsServerTypeSMTP
              outgoingTransport:AccountSettingsServerTransportStartTLS
             outgoingAuthMethod:AccountSettingsServerAuthMethodPasswordClearText];
}

- (void)testQueryPlaceiwannabe
{
    NSString *hostName = @"ssl0.ovh.net";
    [self testServerWithAddress:@"someone@someplaceiwanna.be" incomingHostName:hostName
                   incomingPort:993 incomingProtocol:AccountSettingsServerTypeIMAP
              incomingTransport:AccountSettingsServerTransportTLS
             incomingAuthMethod:AccountSettingsServerAuthMethodPasswordClearText
               outgoingHostName:hostName outgoingPort:465
               outgoingProtocol:AccountSettingsServerTypeSMTP
              outgoingTransport:AccountSettingsServerTransportTLS
             outgoingAuthMethod:AccountSettingsServerAuthMethodPasswordClearText];
}

- (void)testQueryYahoo
{
    [self testServerWithAddress:@"blahblah@yahoo.com" incomingHostName:@"imap.mail.yahoo.com"
                   incomingPort:993 incomingProtocol:AccountSettingsServerTypeIMAP
              incomingTransport:AccountSettingsServerTransportTLS
             incomingAuthMethod:AccountSettingsServerAuthMethodOAUTH2
               outgoingHostName:@"smtp.mail.yahoo.com" outgoingPort:465
               outgoingProtocol:AccountSettingsServerTypeSMTP
              outgoingTransport:AccountSettingsServerTransportTLS
             outgoingAuthMethod:AccountSettingsServerAuthMethodOAUTH2];
}

- (void)testNewpEpTestWrongFormatMailAccount
{

    id<AccountSettingsProtocol> as = [[AccountSettings alloc]
                                      initWithAccountName:@" someone@peptest.ch"
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];

    [as lookup];

    XCTAssertEqual(as.status, AS_OK);
}

- (void)testNewpEpTestUnexistentMailAccount
{
    id<AccountSettingsProtocol> as = [[AccountSettings alloc]
                                      initWithAccountName:@"someone@example.com"
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];
    [as lookup];

    XCTAssertEqual(as.status, AS_NOT_FOUND);
}

// MARK: - Helpers

- (void)verifyAccountSettings:(id<AccountSettingsProtocol>)accountSettings
                      address:(NSString *)address
             incomingHostName:(NSString *)incomingHostName
                 incomingPort:(NSInteger)incomingPort
             incomingProtocol:(AccountSettingsServerProtocolType)incomingProtocol
            incomingTransport:(AccountSettingsServerTransport)incomingTransport
           incomingAuthMethod:(AccountSettingsServerAuthMethod)incomingAuthMethod
             outgoingHostName:(NSString *)outgoingHostName
                 outgoingPort:(NSInteger)outgoingPort
             outgoingProtocol:(AccountSettingsServerProtocolType)outgoingProtocol
            outgoingTransport:(AccountSettingsServerTransport)outgoingTransport
           outgoingAuthMethod:(AccountSettingsServerAuthMethod)outgoingAuthMethod
{
    XCTAssertEqual(accountSettings.status, AS_OK);

    XCTAssertEqualObjects(accountSettings.incoming.username, address);
    XCTAssertEqualObjects(accountSettings.incoming.hostname, incomingHostName);
    XCTAssertEqual(accountSettings.incoming.port, incomingPort);
    XCTAssertEqual(accountSettings.incoming.transport, incomingTransport);
    XCTAssertEqual(accountSettings.incoming.authMethod, incomingAuthMethod);
    XCTAssertEqual(accountSettings.incoming.protocol, incomingProtocol);

    XCTAssertEqualObjects(accountSettings.outgoing.username, address);
    XCTAssertEqualObjects(accountSettings.outgoing.hostname, outgoingHostName);
    XCTAssertEqual(accountSettings.outgoing.port, outgoingPort);
    XCTAssertEqual(accountSettings.outgoing.transport, outgoingTransport);
    XCTAssertEqual(accountSettings.outgoing.authMethod, outgoingAuthMethod);
    XCTAssertEqual(accountSettings.outgoing.protocol, outgoingProtocol);
}

- (void)testServerWithAddress:(NSString *)address
             incomingHostName:(NSString *)incomingHostName
                 incomingPort:(NSInteger)incomingPort
             incomingProtocol:(AccountSettingsServerProtocolType)incomingProtocol
            incomingTransport:(AccountSettingsServerTransport)incomingTransport
           incomingAuthMethod:(AccountSettingsServerAuthMethod)incomingAuthMethod
             outgoingHostName:(NSString *)outgoingHostName
                 outgoingPort:(NSInteger)outgoingPort
             outgoingProtocol:(AccountSettingsServerProtocolType)outgoingProtocol
            outgoingTransport:(AccountSettingsServerTransport)outgoingTransport
           outgoingAuthMethod:(AccountSettingsServerAuthMethod)outgoingAuthMethod
{
    AS_FLAGS flagsToTest[] = { AS_FLAG_USE_ANY, AS_FLAG_USE_ANY_LOCAL };

    for (NSInteger flagIndex = 0; flagIndex < sizeof(flagsToTest)/sizeof(AS_FLAGS); ++flagIndex) {
        for (NSInteger i = 0; i < 2; ++i) {
            id<AccountSettingsProtocol> as = [[AccountSettings alloc]
                                              initWithAccountName:address
                                              provider:nil flags:flagsToTest[flagIndex]
                                              credentials:nil];

            if (i == 0) {
                [as lookup];
            } else {
                XCTestExpectation *expLookedUp = [self expectationWithDescription:@"expLookedUp"];
                [as lookupCompletion:^(id<AccountSettingsProtocol> asParam) {
                    XCTAssertEqualObjects(as, asParam);
                    [self verifyAccountSettings:asParam address:address
                               incomingHostName:incomingHostName
                                   incomingPort:incomingPort incomingProtocol:incomingProtocol
                              incomingTransport:incomingTransport incomingAuthMethod:incomingAuthMethod
                               outgoingHostName:outgoingHostName outgoingPort:outgoingPort
                               outgoingProtocol:outgoingProtocol outgoingTransport:outgoingTransport
                             outgoingAuthMethod:outgoingAuthMethod];
                    [expLookedUp fulfill];
                }];
                [self waitForExpectations:@[expLookedUp] timeout:30];
            }

            [self verifyAccountSettings:as address:address incomingHostName:incomingHostName
                           incomingPort:incomingPort incomingProtocol:incomingProtocol
                      incomingTransport:incomingTransport incomingAuthMethod:incomingAuthMethod
                       outgoingHostName:outgoingHostName outgoingPort:outgoingPort
                       outgoingProtocol:outgoingProtocol outgoingTransport:outgoingTransport
                     outgoingAuthMethod:outgoingAuthMethod];
        }
    }
}

@end
