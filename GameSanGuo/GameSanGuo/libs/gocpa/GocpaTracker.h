//
//  GocpaSDKTracker.h
//  gocpaSDKSample
//
//  Created by seanwong on 13-8-18.
//  Copyright (c) 2013年 gocpa. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GocpaTracker : NSObject  {
@private
    NSString *_appId;
    NSString *_advertiserId;
    bool _referral;
    NSString *_deviceIDFA;
    
    bool disableAppleAdSupportTracking;
    
}

-(GocpaTracker*)initWithAppId:(NSString *)appId advertiserId:(NSString*)advertiserId referral:(bool)referral;
-(void)reportDevice;
-(void)ReportEvent:(NSString*)event;
-(void)ReportEvent:(NSString*)event amount:(float)amount currency:(NSString*)currency;
-(void)SetIDFA:(NSString*)IDFA;
-(void)disableAppleAdSupportTracking;
-(void)enableAppleAdSupportTracking;
@end
