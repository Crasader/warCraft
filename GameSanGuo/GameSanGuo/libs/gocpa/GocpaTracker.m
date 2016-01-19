//
//  GocpaSDKTracker.m
//  gocpaSDKSample
//
//  Created by seanwong on 13-8-18.
//  Copyright (c) 2013年 gocpa. All rights reserved.
//

#import "GocpaTracker.h"
#import "GocpaConfig.h"
#import "GocpaUtilities.h"
@implementation GocpaTracker

-(GocpaTracker*)initWithAppId:(NSString *)appId advertiserId:(NSString*)advertiserId referral:(bool)referral{
    if (self = [super init]) {
        _appId = [appId copy];
        _advertiserId = [advertiserId copy];
        _referral = referral;
        _deviceIDFA=@"";
        disableAppleAdSupportTracking=false;
    }
    return self;

}

-(void)reportDevice{
    
    NSString*advertisingId =@"";
    if (!disableAppleAdSupportTracking)
    {
        advertisingId = [GocpaUtilities getAdvertisingId];
    }
    //NSString*advertisingId = [GocpaUtilities getAdvertisingId];
    NSString*macAddress = [GocpaUtilities getMacAddress];
    
    NSString*model = [GocpaUtilities getDeviceModel];
    NSString*version = [GocpaUtilities getSystemVersion];
    NSString*deviceBrand=@"apple";
    NSString*operator =[GocpaUtilities getOperator];
    
    NSString*deviceCountry =[GocpaUtilities getDeviceCountry];
    bool iADIndicator = [GocpaUtilities getiAd];

    _appId = [GocpaUtilities urlencode:_appId];
    _advertiserId = [GocpaUtilities urlencode:_advertiserId];
    //advertisingId = [GocpaUtilities urlencode:advertisingId];
    if(![_deviceIDFA isEqualToString: @""])
    {
       advertisingId = [GocpaUtilities urlencode:_deviceIDFA];
    }else
    {
       advertisingId = [GocpaUtilities urlencode:advertisingId];
    }
    
    model = [GocpaUtilities urlencode:model];
    version = [GocpaUtilities urlencode:version];
    operator = [GocpaUtilities urlencode:operator];
    
    NSString*deviceId=  [GocpaUtilities urlencode:[NSString stringWithFormat:@"advertisingId=%@&wifimac=%@&deviceBrand=%@&deviceModel=%@&OSVersion=%@&Operator=%@&deviceCountry=%@",advertisingId,macAddress,deviceBrand,model,version,operator,deviceCountry]];
    //NSLog(@"%@",deviceId);
    
    NSString* urlstring = [NSString stringWithFormat:@"%@?appId=%@&advertiserId=%@&referral=%@&deviceId=%@&iAd=%@", PixelHost,_appId, _advertiserId,_referral?@"true":@"false",deviceId,iADIndicator?@"true":@"false"];
    
    //NSURL * url = [NSURL URLWithString:[urlstring stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    NSURL * url = [NSURL URLWithString:urlstring];
    NSLog(@"%@",url);
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:10.0];
        
    [request setHTTPMethod:@"GET"];

    NSArray*cookies = [[NSHTTPCookieStorage sharedHTTPCookieStorage] cookiesForURL:url];
    //NSArray*cookies = [[NSHTTPCookieStorage sharedHTTPCookieStorage] cookies];
    
    for (NSHTTPCookie *cookie in cookies) {
        //NSLog(@"%@",[NSString stringWithFormat:@"%@=%@",cookie.name,cookie.value]);
        [request setValue:[NSString stringWithFormat:@"%@=%@",cookie.name,cookie.value] forHTTPHeaderField:@"Cookie"];
        
    }
    
    
    [NSURLConnection
     sendAsynchronousRequest:request
     queue:[[NSOperationQueue alloc] init]
     completionHandler:^(NSURLResponse *response,
                         NSData *data,
                         NSError *error)
     {
         
         NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
         if ([data length] >0 && error == nil && [httpResponse statusCode] == 200)
         {
             
             NSString* responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
             NSLog(@"%@",responseString);
             NSArray  *cookieArray = [NSHTTPCookie cookiesWithResponseHeaderFields:[httpResponse allHeaderFields] forURL:url];
             [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookies:cookieArray forURL:url mainDocumentURL:nil];

             
         }
         else if ([data length] == 0 && error == nil)
         {
             NSLog(@"Nothing");
         }
         else if (error != nil){
             NSLog(@"Error = %@", error);
         }
         
     }];
    

}
-(void)ReportEvent:(NSString*)event amount:(float)amount currency:(NSString*)currency{
    
    NSString*advertisingId =@"";
    if (!disableAppleAdSupportTracking)
    {
        advertisingId = [GocpaUtilities getAdvertisingId];
    }
    //NSString*advertisingId = [GocpaUtilities getAdvertisingId];
    NSString*macAddress = [GocpaUtilities getMacAddress];
    
    NSString*model = [GocpaUtilities getDeviceModel];
    NSString*version = [GocpaUtilities getSystemVersion];
    NSString*deviceBrand=@"apple";
    NSString*operator =[GocpaUtilities getOperator];
    
    NSString*deviceCountry =[GocpaUtilities getDeviceCountry];
    bool iADIndicator = [GocpaUtilities getiAd];
    
    event = [GocpaUtilities urlencode:event];
    _appId = [GocpaUtilities urlencode:_appId];
    _advertiserId = [GocpaUtilities urlencode:_advertiserId];
    //advertisingId = [GocpaUtilities urlencode:advertisingId];
    if(![_deviceIDFA isEqualToString: @""])
    {
        advertisingId = [GocpaUtilities urlencode:_deviceIDFA];
    }else
    {
        advertisingId = [GocpaUtilities urlencode:advertisingId];
    }
    model = [GocpaUtilities urlencode:model];
    version = [GocpaUtilities urlencode:version];
    operator = [GocpaUtilities urlencode:operator];
    
    
    NSString*deviceId=  [GocpaUtilities urlencode:[NSString stringWithFormat:@"advertisingId=%@&wifimac=%@&deviceBrand=%@&deviceModel=%@&OSVersion=%@&Operator=%@&deviceCountry=%@",advertisingId,macAddress,deviceBrand,model,version,operator,deviceCountry]];
    
    
    NSString* urlstring = [NSString stringWithFormat:@"%@?appId=%@&advertiserId=%@&referral=%@&deviceId=%@&event=%@&amount=%f&currency=%@&iAd=%@", PixelHost,_appId, _advertiserId,_referral?@"true":@"false",deviceId,event,amount,currency,iADIndicator?@"true":@"false"];
    
    
    //NSURL * url = [NSURL URLWithString:[urlstring stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    NSURL* url = [NSURL URLWithString:urlstring];
    NSLog(@"%@",url);
    
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:10.0];
    
    [request setHTTPMethod:@"GET"];
    
    NSArray*cookies = [[NSHTTPCookieStorage sharedHTTPCookieStorage] cookiesForURL:url];
    //NSArray*cookies = [[NSHTTPCookieStorage sharedHTTPCookieStorage] cookies];
    
    for (NSHTTPCookie *cookie in cookies) {
        //NSLog(@"%@",[NSString stringWithFormat:@"%@=%@",cookie.name,cookie.value]);
        [request setValue:[NSString stringWithFormat:@"%@=%@",cookie.name,cookie.value] forHTTPHeaderField:@"Cookie"];
        
    }
    
    
    [NSURLConnection
     sendAsynchronousRequest:request
     queue:[[NSOperationQueue alloc] init]
     completionHandler:^(NSURLResponse *response,
                         NSData *data,
                         NSError *error)
     {
         
         NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
         if ([data length] >0 && error == nil && [httpResponse statusCode] == 200)
         {
             
             //NSString* responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
             //NSLog(@"%@",responseString);
             
             NSArray  *cookieArray = [NSHTTPCookie cookiesWithResponseHeaderFields:[httpResponse allHeaderFields] forURL:url];
             [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookies:cookieArray forURL:url mainDocumentURL:nil];

             
         }
         else if ([data length] == 0 && error == nil)
         {
             NSLog(@"Nothing");
         }
         else if (error != nil){
             NSLog(@"Error = %@", error);
         }
         
     }];
    

}

-(void)ReportEvent:(NSString*)event{
    [self ReportEvent:event amount:.0f currency:@""];
}
-(void)SetIDFA:(NSString *)IDFA{
    _deviceIDFA = [IDFA copy];
}
-(void)disableAppleAdSupportTracking{
    disableAppleAdSupportTracking=true;
}
-(void)enableAppleAdSupportTracking{
    disableAppleAdSupportTracking=false;
}
@end
