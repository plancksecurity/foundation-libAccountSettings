//
//  AccountSettingsServer.h
//  AccountSettingsAdapter
//
//  Created by Xavier Algarra on 27/04/2017.
//  Copyright © 2017 pEp Security. All rights reserved.
//


#import <Foundation/Foundation.h>

#import "account_settings.h"

@interface AccountSettingsServer : NSObject

-(int) getPort;
-(char*) getHostname;
-(AS_ACCESS) getAccesMethod;
-(AS_USERNAME) getUsername;

@end
