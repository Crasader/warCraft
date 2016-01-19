//
//  SGPushCenterOBJC.m
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-15.
//
//

#include "PlatformAdp.h"

#if(PLATFORM == IOS)

#import "SGPushCenterOBJC.h"

@implementation SGPushCenterOBJC
-(id)init
{
    self = [super init];
    if(self)
    {
        _isInbackground = false;

    }
    
    return self;
}

-(void)pushActivities
{
}

-(void)pushPowerFull:(int)timeLeft
{
}

-(void)pushPowerFull:(int)timeLeft andLeftTime:(int)leftTime{
    NSLog(@"test");
}

-(void)pushFriendMessage:(const char *)friendName
{
    
}

-(void)pushNotification:(NSString*)notificationBody andLeftTime:(int)leftTime{
    UILocalNotification *notification = [[[UILocalNotification alloc] init] autorelease];
    NSDate *date = [NSDate dateWithTimeIntervalSinceNow:leftTime];
    if (notification != nil) {
        // 设置推送时间
        notification.fireDate = date;
        // 设置时区
        notification.timeZone = [NSTimeZone defaultTimeZone];
        // 设置重复间隔
        notification.repeatInterval = kCFCalendarUnitDay;
        // 推送声音
        notification.soundName = UILocalNotificationDefaultSoundName;
        // 推送内容
        notification.alertBody = notificationBody;
        //显示在icon上的红色圈中的数子
        notification.applicationIconBadgeNumber = 1;
        //设置userinfo 方便在之后需要撤销的时候使用
        NSDictionary *info = [NSDictionary dictionaryWithObject:@"name" forKey:@"key"];
        notification.userInfo = info;
        //添加推送到UIApplication
        UIApplication *app = [UIApplication sharedApplication];
        [app scheduleLocalNotification:notification];
//        [app presentLocalNotificationNow:notification];
        
    }
}

-(void)dealloc
{
    [super dealloc];
}

@end

#else

//android code
#include  "SGPushCenterOBJC.h"
#include "time.h"


bool SGPushCenterOBJC::init()
{
	_isInbackground = false;
	return true;
}

void SGPushCenterOBJC::pushActivities()
{
	
}
void SGPushCenterOBJC::pushPowerFull(int timeLeft, int leftTime)
{
	
}
void SGPushCenterOBJC::pushFriendMessage(const char* friendName)
{
	
}
void pushNotification(CCString*notificationBody,int leftTime)
{
	//UILocalNotification *notification = new UILocalNotification();
	CCNotificationCenter *notification=new CCNotificationCenter();
	if(notification && notification->sharedNotificationCenter())
	{
		notification->autorelease();
		// return notification;
	}
	
}

SGPushCenterOBJC *SGPushCenterOBJC::create()
{
	SGPushCenterOBJC *pRet = new SGPushCenterOBJC();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

#endif


