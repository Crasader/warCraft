//
//  GocpaSDKUtilities.h
//  gocpaSDKSample
//
//  Created by seanwong on 13-8-18.
//  Copyright (c) 2013年 gocpa. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GocpaUtilities : NSObject
+ (NSString*) getIP;
+ (NSString *) getMacAddress;
+ (NSString *)urlencode:(NSString*)inputstring;
+(NSString*)getAdvertisingId;
+(NSString*)getSystemVersion;
+(NSString*)getDeviceModel;
+(NSString*)getOperator;
+(NSString*)getDeviceCountry;
+(Boolean)getiAd;
@end
