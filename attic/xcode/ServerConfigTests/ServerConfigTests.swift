//
//  ServerConfigTests.swift
//  ServerConfig
//
//  Created by Dirk Zimmermann on 18/01/2017.
//  Copyright © 2017 pEp Security S.A. All rights reserved.
//

import XCTest

class ServerConfigTests: XCTestCase {

    func testSimpleLookup() {
        let ac = SCAccount()
        ac.email = "someone@gmail.com"
        let res = ac.probe()
        XCTAssertEqual(res, 0)
    }
}
