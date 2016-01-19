//
//  gmfTradeService.h
//
//  Created by GMobi on 14-3-12.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "gmfActions.h"


@interface gmfTradeService : NSObject 

+ (void)start: (UIWindow*) appWindow
          aid: (NSString*) appId
   secrectkey: (NSString*) secretKey
   extraparam: (NSDictionary*) extraParams;

+ (void)setDebug:(BOOL)state;

+ (BOOL)isStarted;

+ (gmfTradeService*) getInstance;

+ (void)execute: (NSString*) action
    gmfcallback: (GMFAppCallHandler) callback
     extraparam: (NSDictionary*) extraParams;


@end
