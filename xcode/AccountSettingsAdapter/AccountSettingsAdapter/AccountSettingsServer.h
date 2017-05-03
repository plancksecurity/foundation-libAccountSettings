//
//  AccountSettingsServer.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//


#import <Foundation/Foundation.h>

#import "account_settings.h"

@interface AccountSettingsServer: NSObject

/**
 wrap method for get the port number where the server provides its service
 */
@property (nonatomic, readonly) NSInteger port;

/**
 wrap method for get the hostname of the server
 */
@property (nonatomic, readonly, nonnull) NSString *hostname;

/**
 wrap method for get the access method that is provided by the server
 */
@property (nonatomic, readonly) AS_ACCESS accesMethod;

/**
 wrap method for get the type of username is expected for authentication at the server
 */
@property (nonatomic, readonly) AS_USERNAME username;

@end
