//
//  ServerConfigTests.m
//  ServerConfigTests
//
//  Created by Dirk Zimmermann on 17/01/2017.
//  Copyright © 2017 pEp Security S.A. All rights reserved.
//

#import <XCTest/XCTest.h>

#import "ServerConfig.h"

@interface ServerConfigTests : XCTestCase

@end

@implementation ServerConfigTests

- (void)testSimpleLookup {
    SCAccount *ac = [[SCAccount alloc] init];
    ac.email = @"someguy@gmail.com";
    NSInteger res = [ac probe];
    XCTAssertEqual(res, 0);
}

@end
