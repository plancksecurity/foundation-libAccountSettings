//
//  AccountSettingsAdapterTests.m
//  AccountSettingsAdapterTests
//
//  Created by Xavier Algarra on 08/05/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "ASAccountSettings.h"

const unsigned int AMINCOMINGPEPTEST = 0x81012;
const unsigned int AMOUTGOINGPEPTEST = 0x41021;

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

- (void)testNewpEpTestAccountSettingsSetup {

    id<AccountSettingsProtocol> as = [[ASAccountSettings alloc]
                                      initWithAccountName:@"someone@peptest.ch"
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];

    XCTAssertEqual(as.status, AS_OK);
    XCTAssertEqualObjects(@"someone@peptest.ch", as.incoming.username);
    XCTAssertEqualObjects(@"mail.syhosting.ch", as.incoming.hostname);
    XCTAssertEqual(993, as.incoming.port);
    XCTAssertEqual(AMINCOMINGPEPTEST, as.incoming.accesMethod);
    XCTAssertEqualObjects(@"mail.syhosting.ch", as.outgoing.hostname);
    XCTAssertEqual(587, as.outgoing.port);
    XCTAssertEqual(AMOUTGOINGPEPTEST, as.outgoing.accesMethod);
}

- (void)testNewpEpTestWrongFormatMailAccount {

    id<AccountSettingsProtocol> as = [[ASAccountSettings alloc]
                                      initWithAccountName:@" someone@peptest.ch "
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];

    XCTAssertEqual(as.status, AS_ILLEGAL_VALUE);
}

//FIXME the current status vale us AS_ILLEGAL_VALUE in the future needs to be AS_NOT_FOUND
- (void)testNewpEpTestUnexistentMailAccount {

    id<AccountSettingsProtocol> as = [[ASAccountSettings alloc]
                                      initWithAccountName:@"someone@example.com"
                                      provider:nil flags:AS_FLAG_USE_ANY
                                      credentials:nil];

    XCTAssertEqual(as.status, AS_ILLEGAL_VALUE);
}

@end
