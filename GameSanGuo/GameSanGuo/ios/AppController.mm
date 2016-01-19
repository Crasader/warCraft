//
//  GameSanGuoAppController.mm
//  GameSanGuo
//
//  Created by wenlong w on 12-11-19.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "CCUserDefault.h"
#import "RootViewController.h"
#import "OcCaller.h"
#import "SdkController.h"
#if (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY)
#import "Go2playIosImp.h"
#endif

#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
#import <FacebookSDK/FacebookSDK.h>
#endif

#if (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD)
#import <AlipaySDK/AlipaySDK.h>
#endif

@implementation AppController

@synthesize window;
@synthesize viewController;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

//#define  LOGODELAYTIME  5
//#define  SHOWVIDEOFIRST 1
//modified by cgp
#define  LOGODELAYTIME  0
//#define  SHOWVIDEOFIRST 0


#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation
{
    FBSession *sanguoSession = [FBSession activeSession];
    // attempt to extract a token from the url
    return [FBAppCall handleOpenURL:url
                  sourceApplication:sourceApplication
                        withSession:sanguoSession];
}

#else

//这里是针对快用sdk需求的支付宝相关要求。
#if (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD)
- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation
{
    //如果极简SDK不可用，会跳转支付宝钱包进行支付，需要将支付宝钱包的支付结果回传给SDK
    if ([url.host isEqualToString:@"safepay"]) {
        [[AlipaySDK defaultService] processOderWithPaymentResult:url];
    }
    return YES;
}

#endif

#endif

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen]bounds]] autorelease];
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    //消息推送支持的类型
    UIRemoteNotificationType types = (UIRemoteNotificationTypeBadge|UIRemoteNotificationTypeSound|UIRemoteNotificationTypeAlert);
    //注册消息推送
    [[UIApplication sharedApplication]registerForRemoteNotificationTypes:types];
    
    [[UIApplication sharedApplication] setStatusBarHidden: YES];
//    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
//                                     pixelFormat: kEAGLColorFormatRGBA8
//                                     depthFormat: GL_DEPTH_COMPONENT16
//                              preserveBackbuffer: NO
//                                      sharegroup: nil
//                                   multiSampling: NO
//                                 numberOfSamples:0 ];
    
    //modified by cgp
   // window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];

    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    //sleep(LOGODELAYTIME);//logo延时
#ifdef SHOWVIDEOFIRST
    float vw = 0.5;
    if ([__glView getWidth] == 768) {
        vw = 1;
    }
    //[self playVideo: CGRectMake(0, 0, [__glView getWidth] * vw, [__glView getHeight] * vw)];
#else
    cocos2d::CCApplication::sharedApplication()->run();
#endif
    
    //启动go2play的sdk
#if (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY)
    static_cast<Go2playIosImp *>(SdkController::gi()->getSdkImp())->setUIWindow(self.window);
#endif
    SdkController::gi()->getSdkImp()->sdkStart();
    
    //这里初始化AF的初始信息
    OcCaller::getInstance()->initAppsFlysWithAppid();
    return YES;
}

//获取DeviceToken成功
- (void)application:(UIApplication *)application
didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSLog(@"DeviceToken: {%@}",deviceToken);
    std::string dtk = [[deviceToken description] UTF8String];
    cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("deviceToken", dtk);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    //这里进行的操作，是将Device Token发送到服务端
    
//    const void *devTokenBytes = [deviceToken bytes];
//    self.registered = YES;
//    [self sendProviderDeviceToken:devTokenBytes]; // custom method
    
}

//注册消息推送失败
- (void)application:(UIApplication *)application
didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"Register Remote Notifications error:{%@}",[error localizedDescription]);
}

//处理收到的消息推送
- (void)application:(UIApplication *)application
didReceiveRemoteNotification:(NSDictionary *)userInfo
{
//    NSLog(@"Receive remote notification : %@",userInfo);
//    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"温馨提示"
//                               message:@"推送成功！"
//                              delegate:nil
//                     cancelButtonTitle:@"确定"
//                     otherButtonTitles:nil];
//    [alert show];
//    [alert release];
    
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
    
    //这里启动AppsFlyer
    OcCaller::getInstance()->startAppsFlys();
    
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    FBSession *se = [FBSession activeSession];
    [FBAppCall handleDidBecomeActiveWithSession:se];
    //MM: FB统计
    [FBSettings setDefaultAppID:@"1543699082536762"];
    [FBAppEvents activateApp];
    NSLog(@"INSPECT! FB activate!");
#endif
    
#if (GAME_HASOFFER_STATUS == GHOS_HASOFFER_ON)
    [MobileAppTracker measureSession];
#endif
    
#if (GAME_HASOFFER_STATUS == GCBS_CHARTBOOST_ON)
    Chartboost *cb = [Chartboost sharedChartboost];
    
    cb.appId = CHARTBOOSTAPPID;
    cb.appSignature = CHARTBOOSTAPPSIGNATURE;
    
//    // Required for use of delegate methods. See "Advanced Topics" section below.
//    cb.delegate = self;
    
    // Begin a user session. Must not be dependent on user actions or any prior network requests.
    // Must be called every time your app becomes active.
    [cb startSession];
    
    // Show an interstitial
    [cb showInterstitial];
#endif
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    FBSession *sanguoSession = [FBSession activeSession];
    [sanguoSession closeAndClearTokenInformation];
#endif
    
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
    CCLOG("#########################内存警告##############################");
    cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}

-(void)playVideo:(CGRect) rect
{
    @try {
        
        NSLog(@"%f version", [[[UIDevice currentDevice] systemVersion] floatValue]);
        
        NSString *path = [[NSBundle mainBundle] bundlePath];
        
        NSString *finalPath = [path stringByAppendingPathComponent:@"IPHONE4.mp4"];
        
        if ([[NSFileManager defaultManager] fileExistsAtPath:finalPath])
        {
            
            [self performSelector:@selector(moviePlaybackComplete:) withObject:nil afterDelay:30.0f];
            
            if ([[[UIDevice currentDevice] systemVersion] floatValue]>3.21)
            {
                
                [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(moviePlaybackComplete:) name:MPMoviePlayerPlaybackDidFinishNotification object:nil];
                
                mpviewController = [[MPMoviePlayerViewController alloc] initWithContentURL:[NSURL fileURLWithPath:finalPath]];
                
                //                [mpviewController shouldAutorotateToInterfaceOrientation:UIDeviceOrientationLandscapeRight];
                
                [window addSubview:mpviewController.view];
                
                [window makeKeyAndVisible];
//                mpviewController.view.transform = CGAffineTransformMakeRotation(90 * M_PI / 180);
                
                MPMoviePlayerController *mp = [mpviewController moviePlayer];
                
                UIButton *control = [[UIButton alloc] init];
                
                [control addTarget:self action:@selector(exitVideo) forControlEvents:UIControlEventTouchUpInside];
                
                control.frame = CGRectMake(0, 0, 2048, 1536);
                
                [mp.view addSubview:control];
                
                mp.controlStyle = MPMovieControlStyleNone;
                
                [mp.view setFrame:rect];
                
                [mp setScalingMode:MPMovieScalingModeFill];
                
                [mp setMovieSourceType:(MPMovieSourceTypeUnknown)];
                
                [mp prepareToPlay];
                
                [[mpviewController moviePlayer] play];
                
                [control autorelease];
                
                return;
            }
        }
    }
    
    @catch (NSException *exception) {
        
        NSLog(@"播放异常: %@", exception);
        
    }
    
}

- (void)moviePlaybackComplete:(NSNotification *)notification

{
    
    [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(moviePlaybackComplete:) object:nil];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self
     
                                                    name:MPMoviePlayerPlaybackDidFinishNotification
     
                                                  object:nil];
    //[[MPMusicPlayerController applicationMusicPlayer] setVolume:soundVolume];   //restore volume setting
    
    if ([[[UIDevice currentDevice] systemVersion] floatValue]>=3.0)
        
        [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation: UIStatusBarAnimationNone];
    
    else
        
        [[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    [self finishLaunching];
    
}



- (void)finishLaunching

{
    //        window.transform = CGAffineTransformMakeRotation(90 * M_PI / 180);
    
    [[mpviewController moviePlayer] stop];
    
    [mpviewController.view removeFromSuperview];
    
    [mpviewController release];
    
    mpviewController = nil;
    
    cocos2d::CCApplication::sharedApplication()->run();
    
}

-(void)exitVideo
{
    [self moviePlaybackComplete:nil];
}


- (void)dealloc {
    [super dealloc];
}


@end

