//
//  SGPlatFormIos.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#include "SGPlatFormIos.h"
#include "SGLoadingView.h"
#include "SGHeartbeatDetection.h"
#include "SGRootViewController.h"
#include "SGMainManager.h"
#include "PlatformAdp.h"
#include "SGSocketClient.h"

#if (PLATFORM == IOS)
	#include "OcCaller.h"
#endif

//1秒检测一次，30秒内没有除心跳外的有效消息回复，则提示网络断开。
#define CONNETCOUNTLIMIT    (30)
static SGRootViewController *root = NULL;

SGPlatFormIos::SGPlatFormIos():m_connetCount(0), _isShowCircle(false)
{
#if (PLATFORM == IOS)
	
	OcCaller::getInstance()->createController(&root);
	/*
    root = [[SGRootViewController alloc]init];
    [[EAGLView sharedEGLView]addSubview:[root view]];
	 */
#else
	root = new SGRootViewController;
#endif
}

SGPlatFormIos::~SGPlatFormIos()
{
#if (PLATFORM == IOS)

	OcCaller::getInstance()->releaseController(&root);
	/*
    [root.view removeFromSuperview];
    [root release];
	 */
#else
	this->removeChild(root);
#endif
}

SGPlatFormIos *SGPlatFormIos::create()
{
    SGPlatFormIos *ios = new SGPlatFormIos();
    if(ios && ios->init())
    {
        ios->autorelease();
        return ios;
    }
    
    CC_SAFE_DELETE(ios);
    return NULL;
}

#if (PLATFORM == ANDROID)
bool SGPlatFormIos::init()
{

    CCLayer::init();
	this->addChild(root);
	
	return true;
}
#endif


void SGPlatFormIos::connetDetection(float dt)
{
    m_connetCount++;
    if (m_connetCount >= CONNETCOUNTLIMIT )
    {
        unschedule(schedule_selector(SGPlatFormIos::connetDetection));
        SGSocketClient::sharedSocketClient()->stopSocketConn(SGNET_NO_REPLY_ON_REQUEST);
    }
}

void SGPlatFormIos::showConnection(bool isshow,bool startCount)
{
    _isShowCircle = true;
    m_connetCount = 0;
    if (startCount)
    {
        schedule(schedule_selector(SGPlatFormIos::connetDetection), 1.);
    }
#if (PLATFORM == IOS)
    if (isshow)
		//SGMainManager::shareMain()->showLoadLayer(true);  //cgp come
        SGMainManager::shareMain()->showLoadLayer(false);

	OcCaller::getInstance()->showIOSConnection(&root);
//    [root showConn];
#else
	//SGMainManager::shareMain()->showLoadLayer(true);
    SGMainManager::shareMain()->showLoadLayer(false);    //modified by cgp
#endif
}
void SGPlatFormIos::removeConnection()
{
    m_connetCount = 0;
    unschedule(schedule_selector(SGPlatFormIos::connetDetection));
    
#if (PLATFORM == IOS)
	OcCaller::getInstance()->removeIOSConnection(&root);
//    [root hideConn];
#else
	SGMainManager::shareMain()->showLoadLayer(false);
	root->removelayer();
#endif
    _isShowCircle = false;
}

void SGPlatFormIos::pushFriendMessage(const char *name)
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->pushIOSFriendMessage(&root, name);
//    [root pushFriendMessage:name];
#else
	//android code here
#endif
}

void SGPlatFormIos::pushActivities()
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->pushIOSActivities(&root);
//    [root pushActivities];
#else
	//android code
#endif
}

void SGPlatFormIos::pushPowerFull(int timeLeft)
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->pushIOSPowerFull(&root, timeLeft);
//    [root pushPowerFull:timeLeft];
#else
	//android code
#endif
}

//获得当前发送转圈的状态。
bool SGPlatFormIos::getIsShowCircle()
{
    return _isShowCircle;
}



