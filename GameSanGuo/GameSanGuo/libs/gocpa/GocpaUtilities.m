//
//  GocpaSDKUtilities.m
//  gocpaSDKSample
//
//  Created by seanwong on 13-8-18.
//  Copyright (c) 2013年 gocpa. All rights reserved.
//
#import "GocpaUtilities.h"
#import <Foundation/NSData.h>
#include <sys/socket.h>
#import <ifaddrs.h>
#import <net/if.h>
#import <arpa/inet.h>
#include <net/ethernet.h>

#include <sys/sysctl.h>
#include <net/if_dl.h>
#import "UIDevice-Hardware.h"
#import<CoreTelephony/CTTelephonyNetworkInfo.h>
#import<CoreTelephony/CTCarrier.h>

#import <AdSupport/ASIdentifierManager.h>

@implementation GocpaUtilities


+ (NSString *) getIP {
	NSString *result = nil;
    
	struct ifaddrs*	addrs;
	BOOL success = (getifaddrs(&addrs) == 0);
	if (success)
	{
		const struct ifaddrs* cursor = addrs;
		while (cursor != NULL)
		{
			NSMutableString* ip;
			NSString* interface = nil;
			if (cursor->ifa_addr->sa_family == AF_INET)
			{
				const struct sockaddr_in* dlAddr = (const struct sockaddr_in*) cursor->ifa_addr;
				const uint8_t* base = (const uint8_t*)&dlAddr->sin_addr;
				ip = [NSMutableString new];
				for (int i = 0; i < 4; i++)
				{
					if (i != 0)
						[ip appendFormat:@"."];
					[ip appendFormat:@"%d", base[i]];
				}
				interface = [NSString stringWithFormat:@"%s", cursor->ifa_name];
				if([interface isEqualToString:@"en0"] && result == nil) {
					result = [ip copy];
				}
				if(![interface isEqualToString:@"lo0"] && ![interface isEqualToString:@"en0"] && ![interface isEqualToString:@"fw0"] && ![interface isEqualToString:@"en1"] ) {
					result = [ip copy];
				}
                
			}
			cursor = cursor->ifa_next;
		}
		freeifaddrs(addrs);
	}
    
	if (result == nil) {
        result = @"127.0.0.1";
    }
    
	return result;
}
+(NSString*)getOperator{
    CTTelephonyNetworkInfo *info = [[CTTelephonyNetworkInfo alloc] init];
    CTCarrier *carrier = [info subscriberCellularProvider];
    NSString *countryCode = [carrier mobileCountryCode];

    NSString *mobilecode = [carrier mobileNetworkCode];
    
    
    //NSLog(@"code:%@",code);
    /*NSString*ret=@"Unknown";
    if ([code isEqualToString:@"00"] || [code isEqualToString:@"02"] || [code isEqualToString:@"07"]) {
        
        ret = @"ChinaMobile";
    }
    
    if ([code isEqualToString:@"01"]|| [code isEqualToString:@"06"] ) {
        ret = @"ChinaUnicom";
    }
    
    if ([code isEqualToString:@"03"]|| [code isEqualToString:@"05"] ) {
        ret = @"ChinaTelecom";;
    }
    return ret;*/
    return [NSString stringWithFormat:@"%@%@",countryCode,mobilecode];
}


+(NSString*)getSystemVersion{
    return [[UIDevice currentDevice] systemVersion];
}
+(NSString*)getDeviceModel{
    return [[UIDevice currentDevice] modelName];
}

+ (NSString *) getMacAddress {
    int                 mgmtInfoBase[6];
    char                *msgBuffer = NULL;
    NSString            *errorFlag = NULL;
    size_t              length;
    
    // Setup the management Information Base (mib)
    mgmtInfoBase[0] = CTL_NET;        // Request network subsystem
    mgmtInfoBase[1] = AF_ROUTE;       // Routing table info
    mgmtInfoBase[2] = 0;
    mgmtInfoBase[3] = AF_LINK;        // Request link layer information
    mgmtInfoBase[4] = NET_RT_IFLIST;  // Request all configured interfaces
    
    // With all configured interfaces requested, get handle index
    if ((mgmtInfoBase[5] = if_nametoindex("en0")) == 0) {
        errorFlag = @"if_nametoindex failure";
    }
    // Get the size of the data available (store in len)
    else if (sysctl(mgmtInfoBase, 6, NULL, &length, NULL, 0) < 0) {
        errorFlag = @"sysctl mgmtInfoBase failure";
    }
    // Alloc memory based on above call
    else if ((msgBuffer = malloc(length)) == NULL) {
        errorFlag = @"buffer allocation failure";
    }
    // Get system information, store in buffer
    else if (sysctl(mgmtInfoBase, 6, msgBuffer, &length, NULL, 0) < 0) {
        free(msgBuffer);
        errorFlag = @"sysctl msgBuffer failure";
    }
    else {
        // Map msgbuffer to interface message structure
        struct if_msghdr *interfaceMsgStruct = (struct if_msghdr *) msgBuffer;
        
        // Map to link-level socket structure
        struct sockaddr_dl *socketStruct = (struct sockaddr_dl *) (interfaceMsgStruct + 1);
        
        // Copy link layer address data in socket structure to an array
        unsigned char macAddress[6];
        memcpy(&macAddress, socketStruct->sdl_data + socketStruct->sdl_nlen, 6);
        
        // Read from char array into a string object, into traditional Mac address format
        NSString *macAddressString = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
                                      macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]];
        
        // Release the buffer memory
        free(msgBuffer);
        
        return macAddressString;
    }
    
    return errorFlag;
}

+ (NSString *)urlencode:(NSString*)inputstring {
    NSMutableString *output = [NSMutableString string];
    const unsigned char *source = (const unsigned char *)[inputstring UTF8String];
    int sourceLen = strlen((const char *)source);
    for (int i = 0; i < sourceLen; ++i) {
        const unsigned char thisChar = source[i];
        if (thisChar == ' '){
            [output appendString:@"+"];
        } else if (thisChar == '.' || thisChar == '-' || thisChar == '_' || thisChar == '~' ||
                   (thisChar >= 'a' && thisChar <= 'z') ||
                   (thisChar >= 'A' && thisChar <= 'Z') ||
                   (thisChar >= '0' && thisChar <= '9')) {
            [output appendFormat:@"%c", thisChar];
        } else {
            [output appendFormat:@"%%%02X", thisChar];
        }
    }
    return output;
}

+(NSString*)getAdvertisingId{
    if (!NSClassFromString(@"ASIdentifierManager")) {
        //return [UIDevice currentDevice].uniqueIdentifier;
        return @"";
    }
    NSString *uniqueString = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];

    return uniqueString;
    //return @"";
}

+(NSString*)getDeviceCountry{
    NSLocale *currentLocale = [NSLocale currentLocale];
    NSString *countryCode = [currentLocale objectForKey:NSLocaleCountryCode];
    return countryCode;
}

+(Boolean)getiAd{
    __block Boolean _result=false;

//    // Get a reference to the shared ADClient object instance
//    ADClient * sharedInstance = [ADClient sharedClient];
//    // Check to see if we were installed as the result of an iAd campaign
//    [sharedInstance determineAppInstallationAttributionWithCompletionHandler:^(BOOL appInstallationWasAttributedToiAd) 	{
//        // If we're installed from an iAd campaign
//
//        if(appInstallationWasAttributedToiAd == YES) {
//            
//            _result = true;
//
//        }
//    }];
    return _result;
}
@end
