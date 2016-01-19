//
//  ExtClass.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-1-7.
//
//

#include "ExtClass.h"
#include "SGMainManager.h"
#include "GlobalConfig.h"
#include "SGWelComeLayer.h"
#include "SGHttpClient.h"
#include "SGUpdateClientBox.h"

#if (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
#include "auxiliary/UCSdk.h"
#include "auxiliary/UCSdkCallback.h"
#include "android/jni/CUCGameSdk.h"
using namespace ucgamesdk;
#endif

#if (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD)
#include "KuaiYongIosImp.h"
#endif

#if (GAME_CHANNEL_TYPE == GCCT_CHINA_QIHU_ADR_THIRD)
#include "QihuSdkImp/QihuAdrImp.h"
#endif


#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h"
#include "AndroidSDKAdp.h"
#endif
#include "SdkController.h"


ExtClassOfSDK::ExtClassOfSDK():channelId(1), isLogin_(false)
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

//@使用中，进行SDK登陆。tag=预留
void ExtClassOfSDK::channelLogin(int tag)
{
	printf("channelLogin! ctype=%d!", GAME_CHANNEL_TYPE);
    //MMD: SDKIF 登陆！所有SDK的登陆判断请在此添加！也注意添加头文件！
#if (GAME_ACCOUNT_TYPE == GCAT_UC) //uc
    if (!UCSdk::s_logined) //uc渠道需要其登陆uc账户后才允许点击进入。
    {
        SGMainManager::shareMain()->showMessage(str_sdk_str1);
    }
    else
    {
        if (!isLogin_) //UC登陆上了，但是服务器没连接上。
        {
            SGMainManager::shareMain()->showMessage(str_sdk_str1);
        }
    }
#elif (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY) //go2play
    SdkImp* si = SdkController::gi()->getSdkImp();
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    UserAndExtInfo uaei;
    if (GameConfig::isExist())
    {
        bool isfast = CCUserDefault::sharedUserDefault()->getBoolForKey("isFastLogin");
        if (isfast)
        {
            si->sdkFastloginGame(uaei);  //cgp come
        }
        else
        {
            uaei.userId = ccud->getStringForKey("userId");
            uaei.userName = ccud->getStringForKey("username");
            uaei.userPwd = ccud->getStringForKey("password");
            si->sdkLoginGame(uaei);
        }
    }
    else
    {
        si->sdkFastloginGame(uaei);
    }
#elif (GAME_ACCOUNT_TYPE == GCAT_QIHU) //TODO: 360 login
    if (!isLogin_) //未登录，则提示。
    {
        SGMainManager::shareMain()->showMessage(str_sdk_str1);
    }
#elif (GAME_ACCOUNT_TYPE == GCAT_KUAIYONG) //kuaiyong login
    KuaiYongIosImp* kyImp = dynamic_cast<KuaiYongIosImp*>(SdkController::gi()->getSdkImp());
    if (!kyImp->isKyLogin) //快用渠道需要其登陆账户后才允许点击进入。
    {
        SGMainManager::shareMain()->showMessage(str_sdk_str1);
    }
    else
    {
        if (!isLogin_) //快用登陆上了，但是服务器没连接上。
        {
            SGMainManager::shareMain()->showMessage(str_sdk_str1);
        }
    }
#endif
}

//@使用中，进行SDK注册。tag=预留。
void ExtClassOfSDK::channelRegister(int tag)
{
    printf("channelRegister! ctype=%d!", GAME_CHANNEL_TYPE);
    //MMD: SDKIF 注册！所有SDK的注册判断请在此添加！也注意添加头文件！
#if (GAME_ACCOUNT_TYPE == GCAT_UC) //uc
    if (!UCSdk::s_logined)
    {
        CUCGameSdk::login(UCSdkCallback::login_callback);
    }
    else
    {
        if (!isLogin_) //UC登陆上了，但是服务器挂了，需要重新尝试。
        {
            SGWelComeLayer* wl = dynamic_cast<SGWelComeLayer*>(SGMainManager::shareMain()->getNowShowLayer());
            if (wl)
            {
                SGHttpClient::shareHttpClient()->reqGetChannelUserId(wl, UCSdk::s_sid.c_str());
            }
        }
        else
        {
            ;
        }
    }
#elif (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY) //go2play
    SGMainManager::shareMain()->showregisterLayer(tag);
#elif (GAME_ACCOUNT_TYPE == GCAT_QIHU) //TODO: qihu register
    QihuAdrImp* qhImp = dynamic_cast<QihuAdrImp*>(SdkController::gi()->getSdkImp());
    if (qhImp)
    {
        if (!qhImp->isQhLogin)
        {
            qhImp->login();
        }
        else
        {
            if (!isLogin_) //qihu登录成功，则重新尝试。
            {
                SGWelComeLayer* wl = dynamic_cast<SGWelComeLayer*>(SGMainManager::shareMain()->getNowShowLayer());
                if (wl)
                {
                    SGHttpClient::shareHttpClient()->reqGetChannelUserId(wl, qhImp->qhToken.c_str());
                }
            }
        }
    }
#elif (GAME_ACCOUNT_TYPE == GCAT_KUAIYONG) //kuaiyong
    KuaiYongIosImp* kyImp = dynamic_cast<KuaiYongIosImp*>(SdkController::gi()->getSdkImp());
    if (kyImp)
    {
        if (!kyImp->isKyLogin)
        {
            kyImp->showUserView();
        }
        else
        {
            if (!isLogin_) //kuaiyong登录成功，则重新尝试。
            {
                SGWelComeLayer* wl = dynamic_cast<SGWelComeLayer*>(SGMainManager::shareMain()->getNowShowLayer());
                if (wl)
                {
                    SGHttpClient::shareHttpClient()->reqGetChannelUserId(wl, kyImp->kyToken.c_str());
                }
            }
            else //登陆成功后，再次点击则打开用户中心。
            {
                kyImp->setUpUser();
            }
        }
    }
#else
    //无界面sdk登陆
#endif
}

//@使用中，退出游戏，部分SDK有特殊需求。
void ExtClassOfSDK::channelExitGame(int tag)
{
    printf("channelExitGame! ctype=%d!", GAME_CHANNEL_TYPE);
#if (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD) //uc android
    CUCGameSdk::exitSDK(UCSdkCallback::exit_callback);
#else
    SGMainScene* sgms = SGMainManager::shareMain()->getMainScene();
    int egBoxTag = 20141114;
    if (sgms->getChildByTag(egBoxTag))
    {
        CCLOG("EGBOX already on.");
        return;
    }
    SGUpdateClientBox* ucb = SGUpdateClientBox::create(SGUCB_EXIT_GAME);
    ucb->setScale(.4);
    ucb->runAction(CCScaleTo::create(ITEMACTIONTIME, 1));
    sgms->addChild(ucb, 99999999, egBoxTag);
#endif
}

//@使用中，掉线，部分SDK有特殊需求。
void ExtClassOfSDK::channelStopConn(int tag)
{
    this->isLogin_ = false;
#if (GAME_ACCOUNT_TYPE == GCAT_KUAIYONG) //kuaiyong，掉线也需要调用sdk退出。
    KuaiYongIosImp* kyImp = (KuaiYongIosImp*) SdkController::gi()->getSdkImp();
    kyImp->userLogOut();
#else
    
#endif
}

//@使用中，设置TD的数据。
void ExtClassOfSDK::setTdData(std::string uid, std::string uname)
{
    printf("setTdData! ctype=%d!", GAME_CHANNEL_TYPE);
#if (PLATFORM == IOS)
    OcCaller::getInstance()->setAccountSDK(uid, uname.c_str());
#else
    AndroidSDKAdp::getInstance()->setAccountSDK(uid, uname.c_str());
#endif
}

//@使用中，进入战斗后的细微处理。uc需要隐藏悬浮按钮。
void ExtClassOfSDK::battleBeginDealing()
{
#if (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
    CUCGameSdk::showFloatButton(100, 30, false);
#else
    
#endif
}

//@使用中，退出战斗后的细微处理，uc需要显示悬浮按钮。
void ExtClassOfSDK::battleEndDealing()
{
#if (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
    CUCGameSdk::showFloatButton(100, 30, true);
#else
    
#endif
}

//ext充值接口
void ExtClassOfSDK::userPay(const char *order, float RMB, const char *payDesc, int channelId)
{
	printf("Ext Pay");
}
//向服务器发送消息
void ExtClassOfSDK::buySuccessAndSendMsg(const char *order, int channelId, int isApp)
{
	CCLOG("Buy Success! orderId=%s, chTp=%d, isApp=%d", order, channelId, isApp);
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

//@使用中 是否完成SDK登陆。
bool ExtClassOfSDK::isLoginSDK()
{
	return isLogin_;
}

//@使用中 设置SDK登陆状态。
void ExtClassOfSDK::setIsLoginSDK(bool bl)
{
    isLogin_ = bl;
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

