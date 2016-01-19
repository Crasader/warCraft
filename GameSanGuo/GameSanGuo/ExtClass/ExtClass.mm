//
//  ExtClass.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-1-7.
//
//

#include "ExtClass.h"
#include "SGMainManager.h"

ExtClassOfSDK::ExtClassOfSDK():channelId(1)
{
	
}
ExtClassOfSDK::~ExtClassOfSDK()
{
	
}

//扩展类单例
static ExtClassOfSDK *SDKInstance = NULL;
ExtClassOfSDK *ExtClassOfSDK::sharedSDKInstance()
{
	if (!SDKInstance)
	{
		SDKInstance = new ExtClassOfSDK();
	}
	return SDKInstance;
}

//初始化SDK
void ExtClassOfSDK::initSDK()
{
	printf("init SDK of Channel");
}
//登陆
void ExtClassOfSDK::channelLogin(int tag)
{
	printf("Channel Login!");
}
//ext充值接口
void ExtClassOfSDK::userPay(const char *order, float RMB, const char *payDesc, int channelId)
{
	printf("Ext Pay");
}
//向服务器发送消息
void ExtClassOfSDK::buySuccessAndSendMsg(const char *order, int channelId, int isApp)
{
	printf("Buy Success!");
	SGMainManager::shareMain()->buySuccessAndSendMsg(order, channelId, isApp);
}

//切换帐号
void ExtClassOfSDK::exchangeAccount()
{
}
//进入社区
void ExtClassOfSDK::enterCommunity()
{
	CCLOG("Enter Community From Here!");
}
//游戏内注销后断线重连
void ExtClassOfSDK::sdkLogoutExitGame()
{
	SGMainManager::shareMain()->showwelcomelayer();
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
}

bool ExtClassOfSDK::isLoginSDK()
{
	//返回sdk是否登陆
	bool isLogin = true;
	return isLogin;
}
//获取渠道返回的不同数据
///////////////////////////////////////////////////
std::string ExtClassOfSDK::getUserId()
{
	return userId;
}
std::string ExtClassOfSDK::getSessionId()
{
	return sessionId;
}
std::string ExtClassOfSDK::getNickName()
{
	return nickName;
}
int ExtClassOfSDK::getChannelId()
{
	return channelId;
}
///////////////////////////////////////////////////























