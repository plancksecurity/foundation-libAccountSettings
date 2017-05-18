//
//  ASAccountSettings+Internal.h
//  AccountSettingsAdapter
//
//  Created by Dirk Zimmermann on 18.05.17.
//  Copyright © 2017 pEp Security. All rights reserved.
//

#ifndef ASAccountSettings_Internal_h
#define ASAccountSettings_Internal_h

@interface ASAccountSettings ()

@property (nonatomic, nonnull) const struct AccountSettings *accountSettings;

- (instancetype _Nonnull )initWithAccountSettings:(const struct AccountSettings * _Nonnull)accountSettings;

@end

#endif /* ASAccountSettings_Internal_h */
