//
//  SGRootViewController.m
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#import "SGRootViewController.h"
#include "SGMainManager.h"


#if (PLATFORM == IOS)

@interface SGRootViewController ()

@end

@implementation SGRootViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    

    loading = [[SGLoadingView alloc]init];
    
    pushCenter = [[SGPushCenterOBJC alloc]init];
}

#pragma mark - 联网转圈

-(void)showConn
{
    if(![loading.view isDescendantOfView:self.view])
    {
        [self.view addSubview:loading.view];
    }
}

-(void)hideConn
{
    if([loading.view isDescendantOfView:self.view])
    {
        [loading.view removeFromSuperview];
        SGMainManager::shareMain()->showLoadLayer(false);
    }
}

#pragma mark - 推送消息

-(void)pushActivities
{
    [pushCenter pushActivities];
}

-(void)pushFriendMessage:(const char *)message
{
    [pushCenter pushFriendMessage:message];
}

-(void)pushPowerFull:(int)timeLeft
{
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)dealloc
{
    [loading release];
    [super dealloc];
}

@end

#else
//android code here

#include "cocos2d.h"
USING_NS_CC;

bool SGRootViewController::init()
{
	loading=CCLayer::create();
	pushCenter=SGPushCenterOBJC::create();
	
	return true;
}

bool SGRootViewController::addlayer()
{
	if(init())
	{
		this->addChild(loading);
		return true;
	}
	else
		return false;
}

bool SGRootViewController::removelayer()
{
	if(init())
	{
		this->removeChild(loading);
		return true;
	}
	else
		return false;
}

void SGRootViewController::pushActivities()
{
	pushCenter->pushActivities();
}


void SGRootViewController::pushFriendMessage(const char * message)
{
	pushCenter->pushFriendMessage(message);
}


void SGRootViewController::pushPowerFull(int timeLeft)
{
	
}




#endif
