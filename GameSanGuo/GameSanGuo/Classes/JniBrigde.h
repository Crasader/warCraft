//
//  JniCaller.h
//  GameSanGuo
//
//  Created by bugcode on 14-7-16.
//
//

#ifndef __GameSanGuo__JniCaller__
#define __GameSanGuo__JniCaller__

#include "PlatformAdp.h"
#include "cocos2d.h"
#include "SGMainManager.h"
#include <cstring>
#include "SdkController.h"

using namespace cocos2d;

#if (PLATFORM == ANDROID)

//通过java的端的native方法调用，在session状态改变之后，获取fb个人信息
void fbUserData(const char * userName, const char *userId, const char *loginKey, bool state)
{
    CCLOG("go2play -- execute FBUserData function!");
    SGMainManager::shareMain()->setFBUserData(userId, loginKey, userName, state);
    SGMainManager::shareMain()->setFBSessionState(true);
    SGMainManager::shareMain()->setIsFBLogin(true);

}
//设置fb的session的状态，变量是本地使用，与sdk无关
void fbSetSessionState(bool state)
{
    SGMainManager::shareMain()->setFBSessionState(state);
}
//分享成功与否的提示
void fbShareInfo(const char *info)
{
    SGMainManager::shareMain()->showMessage(info);
}
//通过java的调用关闭分享的弹窗
void closeBoxFromJava()
{
    SGMainManager::shareMain()->closeBox();
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
}
//注销的时候清除帐号信息
void clearAccount()
{
    CCLOG("clearFBUserData!");
    SGMainManager::shareMain()->clearFBUserData();
}
//facebook 成功发布状态之后，调用这个方法向服务器发送消息，映射到对应的java端的native方法
void sendPostStatusAndPicSuccess()
{
    CCLOG("call SGMainManager::finishFBShare()");
    SGMainManager::shareMain()->finishFBShare();
}

// 充值成功，通知服务器，求给予元宝。本方法映射到对应的java端的native方法
void sendChargeSuccessBridge(const char* bId, const int bNum)
{
    CCLOG("call SGMainManager::chargeSuccessForAndroid()");
    SGMainManager::shareMain()->chargeSuccessForAndroid(bId, bNum);
}

//SDK注册与登陆之后往本地写数据，及附加处理。
void setSdkUserData(const bool cissuc, const std::string& cuid, const std::string& cuname, const bool cisfb)
{
    UserAndExtInfo uaei;
    uaei.userId = cuid;
    uaei.userName = cuname;
    uaei.userPwd = SdkController::gi()->getSdkImp()->sdkGetPwd();

    if (cisfb)
    {
        SdkController::gi()->getSdkImp()->sdkFBLoginProcess(cissuc, uaei);
    }
    else
    {
        SdkController::gi()->getSdkImp()->sdkPostProcess(cissuc, uaei);
    }
}

//登陆游戏与快速登陆游戏。
void loginGameNow(const bool isFast, const bool cissuc, const std::string& cuid, const std::string& cuname)
{
    UserAndExtInfo uaei;
    uaei.userId = cuid;
    uaei.userName = cuname;
    uaei.userPwd = SdkController::gi()->getSdkImp()->sdkGetPwd();
    if (isFast)
    {
        SdkController::gi()->getSdkImp()->sdkFastloginGameProcess(cissuc, uaei);
    }
    else
    {
        SdkController::gi()->getSdkImp()->sdkLoginGameProcess(cissuc, uaei);
    }
}

//FB登出。
void fbLogoutNow(const bool cissuc)
{
    SdkController::gi()->getSdkImp()->sdkFBLogoutProcess(cissuc);
}


#endif
#endif /* defined(__GameSanGuo__JniCaller__) */
