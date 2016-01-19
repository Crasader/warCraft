//
//  FMUIWebViewBridge.h
//  test1
//
//  Created by 陈雪龙 on 13-4-25.
//
//

#import <Foundation/Foundation.h>

#import <CoreLocation/CoreLocation.h>

#import <UIKit/UIKit.h>



#import "FMLayerWebView.h"



@interface FMUIWebViewBridge : NSObject<UIWebViewDelegate,UIAlertViewDelegate>{
    
    FMLayerWebView * mLayerWebView;
    
    UIView    *mView;
    
    UIWebView *mWebView;
    
    UIToolbar *mToolbar;
    
    UIBarButtonItem *mBackButton;
    
}



-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString;

-(void) removeMyself;



@end
