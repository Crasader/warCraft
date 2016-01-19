//
//  GameSanGuoAppController.h
//  GameSanGuo
//
//  Created by wenlong w on 12-11-19.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//


#import <MediaPlayer/MediaPlayer.h>

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
    MPMoviePlayerViewController *mpviewController;
    
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;

@end

