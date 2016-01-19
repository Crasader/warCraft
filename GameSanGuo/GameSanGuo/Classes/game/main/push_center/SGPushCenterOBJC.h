//
//  SGPushCenterOBJC.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-15.
//
//

#include "PlatformAdp.h"


#if (PLATFORM == IOS)

#import <Foundation/Foundation.h>

@interface SGPushCenterOBJC : NSObject
{
    BOOL _isInbackground;
}

-(void)pushActivities;
-(void)pushPowerFull:(int)timeLeft andLeftTime:(int)leftTime;
-(void)pushFriendMessage:(const char *)friendName;
-(void)pushNotification:(NSString*)notificationBody andLeftTime:(int)leftTime;
@end

#else
//android code

#include "cocos2d.h"
using namespace cocos2d;

class SGPushCenterOBJC:CCObject
{
public:
	bool _isInbackground;
	void pushActivities();
	void pushPowerFull(int timeLeft, int leftTime);
	void pushFriendMessage(const char* friendName);
	virtual bool init();
	static SGPushCenterOBJC *create(void);
	
};

#endif
