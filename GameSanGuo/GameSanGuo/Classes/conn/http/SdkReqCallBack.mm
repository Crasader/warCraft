//
//  SdkReqCallBack.m
//  OPUserSDK
//
//  Created by pengyou on 13-8-15.
//  Copyright (c) 2013年 OURPALM. All rights reserved.
//

#import "SdkReqCallBack.h"
#import <CoreTelephony/CTCarrier.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import "ASIFormDataRequest.h"

#include "SGMainManager.h"
#include "libjson.h"
#include "Reachability.h"
#include "SGHttpClient.h"
#include "SGShowString.h"
#include "GlobalConfig.h"

#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
#import <FacebookSDK/FacebookSDK.h>
#endif

#if (GAME_HASOFFER_STATUS == GHOS_HASOFFER_ON)
#import <MobileAppTracker/MobileAppTracker.h>
#import <AdSupport/AdSupport.h>
#endif

#if (GAME_HASOFFER_STATUS == GCBS_CHARTBOOST_ON)
#import "Chartboost.h"
//#define CHARTBOOSTAPPID         @"53b617361873da4e4548b7d0"
//#define CHARTBOOSTAPPSIGNATURE  @"9046385fc3e2f340b2eecf114f7773676636063a"
#define CHARTBOOSTAPPID         @"54af95bc04b0167e26cdb520"
#define CHARTBOOSTAPPSIGNATURE  @"3b1a3119cb98dc0d214abfa2206a7396cce3baf5"
#endif

#define HASOFFERADVERTISERID    (@"20888")
#define HASOFFERCONVERSIONKEY   (@"aaecc7cee8b76c214427f22888bd5d8d")


//废弃，做appid参考，暂不删除
//#if OWNTALKDATA == 1
//#define TALKAPP_ID      @"AADCA30D169116CF80FCAEA8EB601177"      //pengyou的talkingdata
//
//#elif OWNTALKDATA == 2
//#define TALKAPP_ID      @"A6F6B5566E09245EF904A6A824D9191D"     //go2play的talkingdata
//
//#else
//#define TALKAPP_ID      @"AADCA30D169116CF80FCAEA8EB601177"    //pengyou的talkingdata
//#endif

//#define TALKCHANNEL_ID  @"CHANNEL_TAIWAN"
@interface SdkReqCallBack ()

@end

@implementation SdkReqCallBack

- (id)init
{
    if (self != [super init]) {
        return nil;
    }

#if TARGET_IPHONE_SIMULATOR
    _ChannelId = [[NSString alloc] initWithUTF8String:"460"];
#else
    CTTelephonyNetworkInfo *info = [[CTTelephonyNetworkInfo alloc] init];
    CTCarrier *carrier = info.subscriberCellularProvider;
    //获取MCC(移动国家码)
    NSString *mcc = [carrier mobileCountryCode];
    //获取名称
    //NSString *countryName = [carrier carrierName];
    //获取MNC(移动网络码)
    NSString *mnc = [carrier mobileNetworkCode];
    
    if (mcc == nil || mnc == nil) {
        _ChannelId = [[NSString alloc] initWithUTF8String:"460"];
    }
    else
    {
        NSString *nameFlag = nil;
        if ( [mcc length] > 2 &&
            [[mcc substringWithRange:NSMakeRange(0, 3)] isEqualToString:@"460"]) {
            NSInteger MNC = [[mnc substringWithRange:NSMakeRange(0, 2)] intValue];
            switch (MNC) {
                case 00:
                case 02:
                case 07:
                    nameFlag = [[NSString alloc] initWithUTF8String:"1"];
                    break;
                case 01:
                case 06:
                    nameFlag = [[NSString alloc] initWithUTF8String:"2"];
                    break;
                case 03:
                case 05:
                    nameFlag = [[NSString alloc] initWithUTF8String:"3"];
                    break;
                case 20:
                    nameFlag = [[NSString alloc] initWithUTF8String:"4"];
                    break;
                default:
                    nameFlag = [[NSString alloc] initWithUTF8String:"5"];
                    break;
            }
        }
        else
        {
            nameFlag = [[NSString alloc] initWithUTF8String:"5"];
        }
        
        _ChannelId = [NSString stringWithFormat:@"%@%@%@",mcc,nameFlag,mnc];
        if ([_ChannelId length] > 6 || nameFlag == nil)
        {
            _ChannelId = [[NSString alloc] initWithUTF8String:"460"];
        }
    }

#endif

    Reachability *r = [Reachability reachabilityWithHostName:@"www.apple.com"];
    
    switch ([r currentReachabilityStatus]) {
            
        case NotReachable:
            // 没有网络连接
            _SubChannelId = [[NSString alloc] initWithUTF8String:"0"];//"None"
            break;
        case ReachableViaWWAN:
            // 使用3G网络
            _SubChannelId = [[NSString alloc] initWithUTF8String:"1"];//"3G"
            break;
        case ReachableViaWiFi:
            // 使用WiFi网络
            _SubChannelId = [[NSString alloc] initWithUTF8String:"2"];//"WiFi"
            break;
    }
    _OsVersion = [[UIDevice currentDevice] systemVersion];
    _OsType = [[UIDevice currentDevice] systemName];
    
    if ([_OsType isEqualToString:@"iPhone OS"])
    {
        _OsType = [[NSString alloc] initWithUTF8String:"1"];
    }
    else if ([_OsType isEqualToString:@"iPad OS"])
    {
        _OsType = [[NSString alloc] initWithUTF8String:"2"];
    }
    else
    {
        _OsType = [[NSString alloc] initWithUTF8String:"0"];
    }
    
    [_ChannelId retain];
    [_SubChannelId retain];
    [_OsVersion retain];
    [_OsType retain];
    
    _account = nil;
    _SharePictureInfo = nil;
    return self;
}


- (void)registFinished:(ASIHTTPRequest *)request
{
    // 当以文本形式读取返回内容时用这个方法
    NSString *responseString = [request responseString];
    NSLog(@"string:%@",responseString);
    
    std::string state ;
    const char*  userId = "101";
    int flag = 0;
    const char* loginKey = "10101";
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0)
        {
             json_char *node_value = json_as_string(*i);
            state = std::string(node_value);
        }
        else if (strcmp(node_name, "USER_ID") == 0)
        {
            json_char *node_value = json_as_string(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0)
        {
            json_char *node_value = json_as_string(*i);
            loginKey = node_value;
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0)
        {
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }

    if (strcmp(state.c_str(), "1") != 0) {
        if (flag == 10) {
            SG_SHOW_WINDOW(str_SDKCallBack_2);
        }
        else if (flag == 11)
        {
            SG_SHOW_WINDOW(str_SDKCallBack_3);
        }
        else if (flag == 12)
        {
            SG_SHOW_WINDOW(str_SDKCallBack_4);
        }
        else if (flag == 13)
        {
            SG_SHOW_WINDOW(str_SDKCallBack_1);
        }
    }
    else
    {
        std::string accountId(userId);
        SGMainManager::shareMain()->doRegist(atoi(state.c_str()),accountId,loginKey);
    }
   
}

- (void)loginFinished:(ASIHTTPRequest *)request
{
    // 当以文本形式读取返回内容时用这个方法
    NSString *responseString = [request responseString];
    NSLog(@"string:%@",responseString);
    
    int state = 0;
    const char*  userId = NULL;
    int flag = 0;
    const char* loginKey = NULL;
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);

        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_char * node_value = json_as_string(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            loginKey = node_value;
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
    
    if (state == 1)
    {
         std::string accountId(userId);
        SGMainManager::shareMain()->doLogin(accountId,loginKey);
        
        std::string username = CCUserDefault::sharedUserDefault()->getStringForKey("username");
        [self setAccount:[NSString stringWithFormat:@"%s",accountId.c_str()]
                       accountName:[NSString stringWithFormat:@"%s",username.c_str()]
                       accountType:kAccountType10];
    }
    else
    {
        if (flag == 20)
        {
            SG_SHOW_WINDOW(str_SDKCallBack_5);
        }
        else if (flag == 13)
        {
            SG_SHOW_WINDOW(str_SDKCallBack_1);
            SGMainManager::shareMain()->dealFbBindFail();
        }
    }
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();

}

- (void)fastLoginFinished:(ASIHTTPRequest *)request
{
    NSString *responseString = [request responseString];
    NSLog(@"string:%@",responseString);
    
    int state = 0;
    const char*  userId =  NULL;
    const char* loginKey = NULL;
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_char * node_value = json_as_string(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            loginKey = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
    if (state == 1) {
	
        std::string accountId(userId);
        SGMainManager::shareMain()->doFastLogin(accountId,loginKey);
        
        [self setAccount:[NSString stringWithFormat:@"%s",accountId.c_str()] accountName:nil accountType:kAccountAnonymous];
    }
}
- (void)passwordFinished:(ASIHTTPRequest *)request
{
    NSString *responseString = [request responseString];
    NSLog(@"string:%@",responseString);
    
    int state = 0;
    int userId = 0;
    int flag = 0;
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            json_free(node_value);
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
    
    SGMainManager::shareMain()->doPassword(state,flag);
}
- (void)destroyGuestFinished:(ASIHTTPRequest *)request
{
    NSString *responseString = [request responseString];
    NSLog(@"string:%@",responseString);
    
    int state = 0;
    int userId = 0;
    int flag = 0;
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            json_free(node_value);
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
    
    [_account release];
    _account = nil;
}

- (void)invoiceFail:(ASIHTTPRequest *)request
{
    SGMainManager::shareMain()->doInvoiceFail();
}

- (void)creatInvoiceFinished:(ASIHTTPRequest *)request
{
    // 当以文本形式读取返回内容时用这个方法
    NSString *responseString = [request responseString];
    NSLog(@"string:%@",responseString);
    
    int state = 1;
    const char* description = NULL;
    
    int billId = 0;
    float amount = 0;
    const char* ccy = NULL;
    
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // get the node name and value as a string
        json_char *response_name = json_name(*i);
        
        if (strcmp(response_name, "response") == 0)
        {
            JSONNODE *response = json_as_node(*i);
            
            JSONNODE_ITERATOR ptrRes = json_begin(response);
            while (ptrRes != json_end(response))
            {
                // find out where to store the values
                json_char *child_name = json_name(*ptrRes);
                if (strcmp(child_name, "result_code") == 0){
                    json_int_t node_value = json_as_int(*ptrRes);
                    state = node_value;
                }
                else if (strcmp(child_name, "bill") == 0)
                {
                    JSONNODE *billInfo = json_as_node(*ptrRes);
                    
                    if (billInfo != NULL)
                    {
                        JSONNODE_ITERATOR ptr = json_begin(billInfo);
                        while (ptr != json_end(billInfo))
                        {
                            if (*ptr == NULL){
                                break;
                            }
                            if (json_type(*ptr) == JSON_ARRAY || json_type(*ptr) == JSON_NODE){
                                break;
                            }
                            json_char *node_name = json_name(*ptr);
                            
                            // find out where to store the values
                            if (strcmp(node_name, "bill_id") == 0){
                                json_int_t node_value = json_as_int(*ptr);
                                billId = node_value;
                            }
                            else if (strcmp(node_name, "amount") == 0){
                                json_number node_value = json_as_float(*ptr);
                                amount = node_value;
                                
                            }
                            else if (strcmp(node_name, "ccy") == 0){
                                json_char *node_value = json_as_string(*ptr);
                                ccy = node_value;
                            }
                            
                            // cleanup and increment the iterator
                            json_free(node_name);
                            ++ptr;
                        }
                        
                    }
                    
                }
                else if (strcmp(child_name, "description") == 0){
                    json_char *node_value = json_as_string(*ptrRes);
                    description = node_value;
                }
                
                // cleanup and increment the iterator
                json_free(child_name);
                ++ptrRes;
                
            }
        }
        // cleanup and increment the iterator
        json_free(response_name);
        ++i;
    }
    if (state != 0)
    {
        NSLog(@"%@",[NSString stringWithFormat:@"%s",description]);
    }
    
    SGMainManager::shareMain()->doCreatInvoice(state,billId);
}

- (void)receiveInvoiceFinished:(ASIHTTPRequest *)request
{
    // 当以文本形式读取返回内容时用这个方法
    NSString *responseString = [request responseString];
    NSLog(@"string:%@",responseString);
    
    int state = 1;
    const char* description = NULL;
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // get the node name and value as a string
        json_char *response_name = json_name(*i);
        if (strcmp(response_name, "response") == 0)
        {
            JSONNODE *response = json_as_node(*i);
            
            JSONNODE_ITERATOR ptrRes = json_begin(response);
            while (ptrRes != json_end(response))
            {
                // find out where to store the values
                json_char *child_name = json_name(*ptrRes);
                
                // find out where to store the values
                if (strcmp(child_name, "result_code") == 0){
                    json_int_t node_value = json_as_int(*ptrRes);
                    state = node_value;
                }
                else if (strcmp(child_name, "description") == 0){
                    json_char *node_value = json_as_string(*ptrRes);
                    description = node_value;
                }
                
                // cleanup and increment the iterator
                json_free(child_name);
                ++ptrRes;
            }
        }
        
        // cleanup and increment the iterator
        json_free(response_name);
        ++i;
    }
    if (state != 0)
    {
        NSLog(@"%@",[NSString stringWithFormat:@"%s",description]);
    }
    SGMainManager::shareMain()->doReceiveInvoice(state);
}




- (void)requestFailed:(ASIHTTPRequest *)request
{
//    NSError *error = [request error];
//    NSLog(@"Test Show Failed:%@",error);
    
//    SGMainManager::shareMain()->showConnectTip();
}

#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)

- (void)creatFbSession
{
    if (!FBSession.activeSession.isOpen)
    {
        // create a fresh session object
        FBSession.activeSession = [[FBSession alloc] init];
        
        // if we don't have a cached token, a call to open here would cause UX for login to
        // occur; we don't want that to happen unless the user clicks the login button, and so
        // we check here to make sure we have a token before calling open
        if (FBSession.activeSession.state == FBSessionStateCreatedTokenLoaded) {
            // even though we had a cached token, we need to login to make the session usable
            [FBSession.activeSession openWithCompletionHandler:^(FBSession *session,
                                                             FBSessionState status,
                                                             NSError *error) {
                // we recurse here, in order to update buttons and labels
                [self updateView : NO];//don't set
            }];
        }
    }
    else
    {
        [self logOutFBView:-1];
        
    }
}
- (void)startFbLogin
{
    // this button's job is to flip-flop the session from open to closed
    if (FBSession.activeSession.isOpen)
    {
        // if a user logs out explicitly, we delete any cached token information, and next
        // time they run the applicaiton they will be presented with log in UX again; most
        // users will simply close the app or switch away, without logging out; this will
        // cause the implicit cached-token login to occur on next launch of the application
        [FBSession.activeSession closeAndClearTokenInformation];
        SGMainManager::shareMain()->setFBSessionState(false);
        [self logOutFBView:-1];//set false
    }
    else
    {
        if (FBSession.activeSession.state != FBSessionStateCreated) {
            // Create a new, logged out session.
            FBSession.activeSession = [[FBSession alloc] init];
        }
        
        // if the session isn't open, let's open it now and present the login UX to the user
        [FBSession.activeSession openWithCompletionHandler:^(FBSession *session,
                                                         FBSessionState status,
                                                         NSError *error) {
            // and here we make sure to update our UX according to the new session state
            [self updateView :YES];//set true
        }];
    }
}

- (void)postToWallFinished:(ASIHTTPRequest *)request
{

//    UIAlertView *av = [[[UIAlertView alloc]
//                        initWithTitle:@"Sucessfully posted to photos & wall!"
//                        message:@"Check out your Facebook to see!"
//                        delegate:nil
//                        cancelButtonTitle:@"OK"
//                        otherButtonTitles:nil] autorelease];
//	[av show];
    
}
- (void)shareFBFail:(ASIHTTPRequest *)request
{
    SGMainManager::shareMain()->shareFbDidFail();
}

- (void)getFacebookPhotoFinished:(ASIHTTPRequest *)request
{
    NSString *responseString = [request responseString];
    NSLog(@"Got Facebook Photo: %@", responseString);
    
    const char* photoLink = NULL;
    NSString *link = NULL;
    
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "link") == 0)
        {
            json_char *node_value = json_as_string(*i);
            photoLink = node_value;
            link = [NSString stringWithFormat:@"%s",photoLink];
           json_free(node_value);
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
    
    if (link == nil) return;
    NSLog(@"Link to photo: %@", link);
    
    NSURL *url = [NSURL URLWithString:@"https://graph.facebook.com/me/feed"];
    ASIFormDataRequest *newRequest = [ASIFormDataRequest requestWithURL:url];

    [newRequest setPostValue:link forKey:@"picture"];
    [newRequest setPostValue:FBSession.activeSession.accessTokenData.accessToken forKey:@"access_token"];
    [newRequest setDidFinishSelector:@selector(postToWallFinished:)];
    //[newRequest setDidFailSelector:@selector(shareFBFail:)];
    [newRequest setDelegate:self];
    [newRequest startAsynchronous];
    
}

- (void)sendToPhotosFinished:(ASIHTTPRequest *)request
{
    // Use when fetching text data
    NSString *responseString = [request responseString];
    
    NSLog(@"string:%@",responseString);
    
    const char* myPhotoId = NULL;
    NSString *photoId = NULL;
    
    JSONNODE *n = json_parse([responseString UTF8String]);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "id") == 0){
            json_char *node_value = json_as_string(*i);
            myPhotoId = node_value;
            photoId = [NSString stringWithFormat:@"%s",myPhotoId];
            json_free(node_value);

        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
    
    NSLog(@"Photo id is: %@", photoId);
    /////////
    NSString *urlString = [NSString stringWithFormat:
                           @"https://graph.facebook.com/%@?access_token=%@", photoId,
                           [FBSession.activeSession.accessTokenData.accessToken stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    NSURL *url = [NSURL URLWithString:urlString];
    ASIHTTPRequest *newRequest = [ASIHTTPRequest requestWithURL:url];
    [newRequest setDidFinishSelector:@selector(getFacebookPhotoFinished:)];
    //[newRequest setDidFailSelector:@selector(shareFBFail:)];
    
    [newRequest setDelegate:self];
    [newRequest startAsynchronous];
    
    SGMainManager::shareMain()->finishFBShare();
}

- (void)publishStory
{
    
    SGMainManager::shareMain()->getMainScene()->plat->showConnection(true,false);
    NSString *filePath = _SharePictureInfo;
    
    NSURL *url = [NSURL URLWithString:@"https://graph.facebook.com/me/photos"];
    ASIFormDataRequest *request = [ASIFormDataRequest requestWithURL:url];
    [request addFile:filePath forKey:@"file"];
    

    switch (m_ShareInfo) {
        case 0:
        {
            //SGShareTypeGeneral
            [request setPostValue:str_SGShareBoxInfo_genral forKey:@"message"];
        }
            break;
        case 1:
        {
            //SGShareTypeVisit
            [request setPostValue:str_SGShareBoxInfo_visit forKey:@"message"];
        }
            break;
        case 2:
        {
            //SGShareTypeBossBattel
            [request setPostValue:str_SGShareBoxInfo_boss forKey:@"message"];
        }
            break;
        case 3:
        {
            //SGShareTypeShoujingzhou
           [request setPostValue:str_SGShareBoxInfo_jinzhou forKey:@"message"];
        }
            break;
        case 4:
        {
            //SGShareTypeGeneralFight
            [request setPostValue:str_SGShareBoxInfo_pvefight forKey:@"message"];
        }
            break;
        default:
        {
            //SGShareTypeGeneral
            [request setPostValue:str_SGShareBoxInfo_genral forKey:@"message"];
        }
            break;
    }
    
    
    [request setPostValue:FBSession.activeSession.accessTokenData.accessToken forKey:@"access_token"];
    [request setDidFinishSelector:@selector(sendToPhotosFinished:)];
    [request setDidFailSelector:@selector(shareFBFail:)];
    
    [request setDelegate:self];
    [request startAsynchronous];
    

}
- (void)requestPublishPermissions
{
    //|| [FBSession.activeSession.permissions indexOfObject:@"publish_stream"] == NSNotFound
    // Request publish_actions
    [FBSession.activeSession requestNewPublishPermissions:[NSArray arrayWithObject:@"publish_actions"]
                                          defaultAudience:FBSessionDefaultAudienceFriends
                                        completionHandler:^(FBSession *session, NSError *error) {
//                                            __block NSString *alertText;
//                                            __block NSString *alertTitle;
                                            if (!error) {
                                                if ([FBSession.activeSession.permissions
                                                     indexOfObject:@"publish_actions"] == NSNotFound
                                                    ){
                                                    // Permission not granted, tell the user we will not publish
//                                                    alertTitle = @"Permission not granted";
//                                                    alertText = @"Your action will not be published to Facebook.";
//                                                    [[[UIAlertView alloc] initWithTitle:alertTitle
//                                                                                message:alertText
//                                                                               delegate:self
//                                                                      cancelButtonTitle:@"OK!"
//                                                                      otherButtonTitles:nil] show];
                                                    [self shareFBFail:nil];
                                                } else {
                                                    // Permission granted, publish the OG story
                                                    [self publishStory];
                                                }
                                                
                                            } else {
                                                // There was an error, handle it
                                                // See https://developers.facebook.com/docs/ios/errors/
                                            }
                                        }];
}

- (void)checkFbPermission
{
    
    if ([FBSession.activeSession.permissions indexOfObject:@"publish_actions"] == NSNotFound)
    {
        // Check for publish permissions
        [FBRequestConnection startWithGraphPath:@"/me/permissions"
                              completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
//                                  __block NSString *alertText;
//                                  __block NSString *alertTitle;
                                  if (!error){
                                      NSDictionary *permissions= [(NSArray *)[result data] objectAtIndex:0];
                                      if (![permissions objectForKey:@"publish_actions"]  ){//||![permissions objectForKey:@"publish_stream"]
                                          // Publish permissions not found, ask for publish_actions
                                          [self requestPublishPermissions];
                                          
                                      } else {
                                          // Publish permissions found, publish the OG story
                                          [self publishStory];
                                      }
                                      
                                  }
                                  else
                                  {
                                      // There was an error, handle it
                                      // See https://developers.facebook.com/docs/ios/errors/
                                  }
                              }];
    }
    else
    {
        [self publishStory];
    }

}
- (void)loginForFBShare
{
    //open session with write permission
    [FBSession openActiveSessionWithPublishPermissions:[NSArray arrayWithObject:@"publish_actions"] defaultAudience:FBSessionDefaultAudienceEveryone allowLoginUI:YES completionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
        
        if( !error )
        {
           [self publishStory];
        }
        else
        {
            [self shareFBFail:nil];
        }
        }];
}

- (void)startFbShare:(int)shareInfo sharePictureInfo:(NSString *)sharePictureInfo;
{
    m_ShareInfo = shareInfo;

    if (_SharePictureInfo != nil) {
        [_SharePictureInfo release];

    }
    _SharePictureInfo = [NSString stringWithFormat:@"%@",sharePictureInfo ];
    
    [_SharePictureInfo retain];
    
    
    if (FBSession.activeSession.isOpen) {
        [self checkFbPermission];
    }
    else
    {
        [self loginForFBShare];
    }
}

- (void)updateView:(BOOL)needSetFBlogin
{
    if (FBSession.activeSession.isOpen) {
        // valid account UI is shown whenever the session is open
//        NSLog(@"%@",[NSString stringWithFormat:@"https://graph.facebook.com/me/friends?access_token=%@",
//                     FBSession.activeSession.accessTokenData.accessToken]);
        [[FBRequest requestForMe] startWithCompletionHandler:
         ^(FBRequestConnection *connection,
           NSDictionary<FBGraphUser> *user,
           NSError *error) {
             if (!error) {
                 NSString *facebookId = user.id;
                 
                 //NSString *firstName = user.first_name;
                 //NSString *lastName = user.last_name;
                 //NSString *email = [user objectForKey:@"email"];
                 
                  NSString *fullName = [NSString stringWithFormat:@"%@%@",user.last_name,user.first_name];
                 
                 const char* loginKey = [FBSession.activeSession.accessTokenData.accessToken UTF8String];
                 const char* userId = [facebookId UTF8String];
                 const char* userName = [fullName UTF8String];
                 
                 if (needSetFBlogin)
                 {
                     SGMainManager::shareMain()->setFBUserData(userId,loginKey,userName,true);
                 }
                 else
                 {
                     SGMainManager::shareMain()->setFBUserData(userId,loginKey,userName,false);
                 }
                 
                
                [self setAccount:facebookId
                                   accountName:fullName
                                   accountType:kAccountType10];

             }
             else
             {
                 [self logOutFBView:-1];
             }
         }];
        
        SGMainManager::shareMain()->setFBSessionState(true);
    }
    else
    {
        NSLog(@"Login to create a link to fetch account data");
    }
}
- (void)logOutFBView:(int)value
{
   SGMainManager::shareMain()->clearFBUserData(value);
}

- (void)closeFBSession:(int)value
{
    if (FBSession.activeSession.isOpen)
    {
        [FBSession.activeSession closeAndClearTokenInformation];
        SGMainManager::shareMain()->setFBSessionState(false);
        [self logOutFBView:value];//set false
    }
    
    
//    [FBSession.activeSession close];
//    SGMainManager::shareMain()->setFBSessionState(false);
}

#endif

+ (void)onStart
{
    //globalconfig 中统一指定
    std::string tdAppId = GlobalConfig::gi()->getTalkingDataAppId();
    NSString *talkingDataAppId = [NSString stringWithUTF8String:tdAppId.c_str()];
    
    std::string tdAppChannel = GlobalConfig::gi()->getTalkingDataAppChannel();
    NSString *tdac = [NSString stringWithUTF8String:tdAppChannel.c_str()];
    
    [TalkingDataGA onStart:talkingDataAppId withChannelId:tdac];
    NSLog(@"INSPECT! TD activate!");
}
#if (GAME_HASOFFER_STATUS == GHOS_HASOFFER_ON)
+ (void)onStartHasOffers
{
    [MobileAppTracker initializeWithMATAdvertiserId:HASOFFERADVERTISERID  MATConversionKey:HASOFFERCONVERSIONKEY];
    
    [MobileAppTracker setAppleAdvertisingIdentifier:[[ASIdentifierManager sharedManager] advertisingIdentifier]
                             advertisingTrackingEnabled:[[ASIdentifierManager sharedManager] isAdvertisingTrackingEnabled]];
    
}
#endif


- (void)setAccount:(NSString *)accountId accountName:(NSString *)accountName accountType:(TDGAAccountType)accountType
{
    std::string serverName =  CCUserDefault::sharedUserDefault()->getStringForKey("servername");
    if (accountType == kAccountAnonymous) {
        _account = [TDGAAccount setAccount:accountId];
        [_account setAccountType:kAccountAnonymous];
        [_account setLevel:1];
    }
    else if (accountType == kAccountRegistered){
        if (_account == nil) {
            _account = [TDGAAccount setAccount:accountId];
        }
        [_account setAccountName:accountName];
        [_account setAccountType:kAccountRegistered];
    }
    else
    {
        if (_account != nil) {
            [_account release];
            _account = nil;
        }
        _account = [TDGAAccount setAccount:accountId];
        [_account setAccountName:accountName];
        [_account setAccountType:kAccountRegistered];
        
    }
    [_account setGameServer:[NSString stringWithUTF8String:serverName.c_str()]];
}

- (void)setAccountName:(NSString *)accountName
{
    if (_account != nil) {
        [_account setAccountName:accountName];
    }
}

- (void)setAccountType:(TDGAAccountType)accountType
{
    if (_account != nil) {
        [_account setAccountType:accountType];
    }
}

- (void)setLevel:(int)level
{
    if (_account != nil) {
        [_account setLevel:level];
    }
}

- (void)setGender:(TDGAGender)gender
{
    if (_account != nil) {
        [_account setGender:gender];
    }
}

- (void)setAge:(int)age
{
    if (_account != nil) {
        [_account setAge:age];
    }
}

- (void)setGameServer:(NSString *)gameServer
{
    if (_account != nil) {
        [_account setGameServer:gameServer];
    }
}


+ (void)onChargeRequst:(NSString *)orderId
                 iapId:(NSString *)iapId
        currencyAmount:(double)currencyAmount
          currencyType:(NSString *)currencyType
 virtualCurrencyAmount:(double)virtualCurrencyAmount
           paymentType:(NSString *)paymentType
{
    [TDGAVirtualCurrency onChargeRequst:orderId iapId:iapId  currencyAmount:currencyAmount  currencyType:currencyType  virtualCurrencyAmount:virtualCurrencyAmount paymentType:paymentType];
}

+ (void)onChargeSuccess:(NSString *)orderId
{
    [TDGAVirtualCurrency onChargeSuccess:orderId];
}

+ (void)onReward:(double)virtualCurrencyAmount reason:(NSString *)reason
{
    [TDGAVirtualCurrency onReward:virtualCurrencyAmount reason:reason];
}

+ (void)onPurchase:(NSString *)item itemNumber:(int)number priceInVirtualCurrency:(double)price
{
   [TDGAItem onPurchase:item itemNumber:number priceInVirtualCurrency:price];
}

+ (void)onUse:(NSString *)item itemNumber:(int)number
{
    [TDGAItem onUse: item itemNumber:number];
}

+ (void)onBegin:(NSString *)missionId
{
    [TDGAMission onBegin:missionId];
}

+ (void)onCompleted:(NSString *)missionId
{
    [TDGAMission onCompleted:missionId];
}

+ (void)onFailed:(NSString *)missionId failedCause:(NSString *)cause
{
    [TDGAMission onFailed:missionId failedCause:cause];
}

+ (void)onEvent:(NSString *)eventId eventData:(NSDictionary *)eventData
{
    [TalkingDataGA onEvent:eventId eventData:eventData];
}

+ (void)setLatitude:(double)latitude longitude:(double)longitude
{
    [TalkingDataGA setLatitude:latitude longitude:longitude];
}

+ (void)setVerboseLogEnabled
{
    [TalkingDataGA setVerboseLogEnabled];
}

@end
