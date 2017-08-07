//
//  AccountSettingsAdapterTests.m
//  AccountSettingsAdapterTests
//
//  Created by Xavier Algarra on 08/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "ASAccountSettings.h"

@interface AccountSettingsAdapterTests : XCTestCase

@end

@implementation AccountSettingsAdapterTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testQueryPeptest
{
    NSString *hostName = @"mail.syhosting.ch";
    [self testServerWithAddress:@"someone@peptest.ch" incomingHostName:hostName
                   incomingPort:993 incomingProtocol:AccountSettingsServerTypeIMAP
              incomingTransport:AccountSettingsServerTransportTLS
             incomingAuthMethod:AccountSettingsServerAuthMethodPasswordEncrypted
               outgoingHostName:hostName outgoingPort:587
               outgoingProtocol:AccountSettingsServerTypeSMTP
              outgoingTransport:AccountSettingsServerTransportStartTLS
             outgoingAuthMethod:AccountSettingsServerAuthMethodPasswordEncrypted];
}

- (void)testQueryPlaceiwannabe
{
    NSString *hostName = @"ssl0.ovh.net";
    [self testServerWithAddress:@"someone@someplaceiwanna.be" incomingHostName:hostName
                   incomingPort:993 incomingProtocol:AccountSettingsServerTypeIMAP
              incomingTransport:AccountSettingsServerTransportTLS
             incomingAuthMethod:AccountSettingsServerAuthMethodPasswordEncrypted
               outgoingHostName:hostName outgoingPort:465
               outgoingProtocol:AccountSettingsServerTypeSMTP
              outgoingTransport:AccountSettingsServerTransportTLS
             outgoingAuthMethod:AccountSettingsServerAuthMethodPasswordEncrypted];
}

- (void)testNewpEpTestWrongFormatMailAccount {

    id<AccountSettingsProtocol> as = [[ASAccountSettings alloc]
                                      initWithAccountName:@" someone@peptest.ch "
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];

    XCTAssertEqual(as.status, AS_ILLEGAL_VALUE);
}

- (void)testNewpEpTestUnexistentMailAccount {

    id<AccountSettingsProtocol> as = [[ASAccountSettings alloc]
                                      initWithAccountName:@"someone@example.com"
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];

    XCTAssertEqual(as.status, AS_ILLEGAL_VALUE);
}

// MARK: - Helpers

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
    id<AccountSettingsProtocol> as = [[ASAccountSettings alloc]
                                      initWithAccountName:address
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];

    XCTAssertEqual(as.status, AS_OK);

    XCTAssertEqualObjects(as.incoming.username, address);
    XCTAssertEqualObjects(as.incoming.hostname, incomingHostName);
    XCTAssertEqual(as.incoming.port, incomingPort);
    XCTAssertEqual(as.incoming.transport, incomingTransport);
    XCTAssertEqual(as.incoming.authMethod, incomingAuthMethod);
    XCTAssertEqual(as.incoming.protocol, incomingProtocol);

    XCTAssertEqualObjects(as.outgoing.username, address);
    XCTAssertEqualObjects(as.outgoing.hostname, outgoingHostName);
    XCTAssertEqual(as.outgoing.port, outgoingPort);
    XCTAssertEqual(as.outgoing.transport, outgoingTransport);
    XCTAssertEqual(as.outgoing.authMethod, outgoingAuthMethod);
    XCTAssertEqual(as.outgoing.protocol, outgoingProtocol);
}

@end
