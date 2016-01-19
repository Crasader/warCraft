//
//  SGRootViewController.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#ifndef __GameSanGuo__SGRootViewController__
#define __GameSanGuo__SGRootViewController__



#include "PlatformAdp.h"

#if (PLATFORM == IOS)
#import <UIKit/UIKit.h>
#import "SGLoadingView.h"
#import "SGPushCenterOBJC.h"
#import <Foundation/Foundation.h>



@interface SGRootViewController : UIViewController
{
    SGLoadingView *loading;

    SGPushCenterOBJC *pushCenter;
}
-(void)showConn;
-(void)hideConn;
-(void)pushFriendMessage:(const char *)message;
-(void)pushActivities;
-(void)pushPowerFull:(int)timeLeft;
@end
#else

	//android code here

#include "cocos2d.h"
#include "game/main/push_center/SGPushCenterOBJC.h"
class SGRootViewController : public cocos2d:: CCLayer
{
public:
	
	CCLayer* loading;
	SGPushCenterOBJC* pushCenter;
	virtual bool init();
	
	bool addlayer();
	bool removelayer();
	void pushActivities();
	void pushFriendMessage(const char * message);
	void pushPowerFull(int timeLeft);
	
	CREATE_FUNC(SGRootViewController);
	
	/*static cocos2d::CCScene* scene();*/
};
#endif

#endif