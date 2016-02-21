//
//  SdkImp.cpp
//  GameSanGuo
//
//  Created by bugcode on 15-2-2.
//
//

#include <stdio.h>
#include "SdkImp.h"
#include "GlobalConfig.h"
#include "ExtClass.h"
#include "SGMainManager.h"
#include "SGWelComeLayer.h"

//请求到sdk数据,写入本地（CCUSERDEFAULT）
void SdkImp::sdkPostProcess(bool isSuc, UserAndExtInfo &uei)
{
    if (isSuc)
    {
        //@必读！以下逻辑与21号HTTP消息中处理逻辑相同，可参考sgwelcomlayer，不过本逻辑“!只针对借用我方注册/登陆界面!"的渠道。
        
        CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
        ccud->setStringForKey("userId", uei.userId);
        ccud->setStringForKey("username", uei.userName);
        ccud->setStringForKey("password", uei.userPwd);
        ccud->setIntegerForKey("channelId", GlobalConfig::gi()->getRealChannelId());
        //go2play默认使用 userId 当做loginKey
        ccud->setStringForKey("loginKey", sdkGetEncryptLoginKey(uei.userId));
        
        ccud->setBoolForKey("isExist", true);
        ccud->setBoolForKey("isAccount", true);
        //是否是快速登陆的帐号状态
        ccud->setBoolForKey("isFastLogin", false);
        ccud->setBoolForKey("isSdkFBLogin", false);
        
        ccud->flush();
        ExtClassOfSDK::sharedSDKInstance()->setIsLoginSDK(true);
        //添加的针对非注册体系下的td数据设置
        ExtClassOfSDK::sharedSDKInstance()->setTdData(uei.userId, uei.userName);
        
        //写入完成，直接进入welcomelyer，显示逻辑已在welcomelayer中完成。
        SGMainManager::shareMain()->showwelcomelayer();
        
    }
    //显示完成之后移除对应的网络锁定
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
};


//快速登陆请求sdk获取数据，开始一些本地的游戏登陆相关的数据处理
void SdkImp::sdkFastloginGameProcess(bool isSuc, UserAndExtInfo &uei) //cgp come
{
    //将帐号相关的数据写入本地，此次快速登陆，标记isExist设置为false

    if (isSuc)
    {
        //登陆成功，开始写入本地
        CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
        ccud->setStringForKey("userId", uei.userId);
        ccud->setStringForKey("username", uei.userName);
        ccud->setStringForKey("password", uei.userPwd);
        ccud->setIntegerForKey("channelId", GlobalConfig::gi()->getRealChannelId());
        //go2play默认使用 userId 当做loginKey
        ccud->setStringForKey("loginKey", sdkGetEncryptLoginKey(uei.userId));
        
        //快速登陆
        ccud->setBoolForKey("isExist", true);
        ccud->setBoolForKey("isAccount", true);
        //是否是快速登陆的帐号状态
        ccud->setBoolForKey("isFastLogin", true);
        ccud->setBoolForKey("isSdkFBLogin", false);
        ccud->flush();
        
        //这里后续应该直接开始登陆游戏
        this->sdkLoginGameProcess(isSuc, uei);
        
    }
    //完成，移除网络锁定
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
}

//本地已存在帐户数据，这里直接做验证后的登陆游戏处理
void SdkImp::sdkLoginGameProcess(bool isSuc, UserAndExtInfo &uei)
{
     /*
        这里是直接开始验证之后的登陆游戏处理，
        至此，本地是一定存有帐号，这里直接使用本地的帐号登陆游戏，不存在再次设置帐号信息
     */

    SGWelComeLayer* wl = dynamic_cast<SGWelComeLayer*>(SGMainManager::shareMain()->getNowShowLayer());
    //有welcomelayer并且向sdk验证成功，才开始登陆
    if (wl && isSuc)
    {
        //sdk已登陆标记
        ExtClassOfSDK::sharedSDKInstance()->setIsLoginSDK(true);
        //添加的针对非注册体系下的td数据设置
        ExtClassOfSDK::sharedSDKInstance()->setTdData(uei.userId, uei.userName);
        wl->loginGameProcess();
        //MM: 务必设置按钮文本！因为后面有依据文本的逻辑！
        wl->setRegisterBtnText(uei.userName);
    }
    
    //完成，移除网络锁定
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
};

//facebook登陆处理
void SdkImp::sdkFBLoginProcess(bool isSuc, UserAndExtInfo &uei)
{
    //处理facebook登陆后的处理
    if (isSuc)
    {
        //登陆成功，开始写入本地
        CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
        ccud->setStringForKey("userId", uei.userId);
        ccud->setStringForKey("username", uei.userName);
        ccud->setStringForKey("password", uei.userPwd);
        ccud->setIntegerForKey("channelId", GlobalConfig::gi()->getRealChannelId());
        //go2play默认使用 userId 当做loginKey
        ccud->setStringForKey("loginKey", sdkGetEncryptLoginKey(uei.userId));
        
        //快速登陆
        ccud->setBoolForKey("isExist", true);
        ccud->setBoolForKey("isAccount", true);
        //是否是快速登陆的帐号状态
        ccud->setBoolForKey("isFastLogin", false);
        ccud->setBoolForKey("isSdkFBLogin", true);
        ccud->flush();
        
        //后续登陆逻辑，更新视图
        SGWelComeLayer* wl = dynamic_cast<SGWelComeLayer*>(SGMainManager::shareMain()->getNowShowLayer());
        if (wl)
        {
            //facebook 登陆成功
            wl->updateViewFBForSdk(true);
            //MM: 务必设置按钮文本！因为后面有依据文本的逻辑！
            wl->setRegisterBtnText(uei.userName);
        }
        //sdk已登陆标记
        ExtClassOfSDK::sharedSDKInstance()->setIsLoginSDK(true);
        //添加的针对非注册体系下的td数据设置
        ExtClassOfSDK::sharedSDKInstance()->setTdData(uei.userId, uei.userName);
        
    }
    //完成，移除网络锁定
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
}

//facebook 登出
void SdkImp::sdkFBLogoutProcess(bool isSuc)
{
    //如果登陆成功，设置本地标记变量
    if (isSuc)
    {
        //登陆成功，开始写入本地
        CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
        //快速登陆
        ccud->setBoolForKey("isExist", false);
        ccud->setBoolForKey("isAccount", false);
        ccud->setBoolForKey("isFastLogin", false);
        ccud->setBoolForKey("isSdkFBLogin", false);
        ccud->flush();
        SGWelComeLayer* wl = dynamic_cast<SGWelComeLayer*>(SGMainManager::shareMain()->getNowShowLayer());
        if (wl)
        {
            //facebook 登陆成功
            wl->updateViewFBForSdk(false);
            //MM: 务必设置按钮文本！因为后面有依据文本的逻辑！
            wl->setRegisterBtnText(str_ServerItem_str17);
        }
        //sdk已登陆标记
        ExtClassOfSDK::sharedSDKInstance()->setIsLoginSDK(false);
    }
    //完成，移除网络锁定
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
}


//sdk loginkey的特殊处理，有些没有验证的渠道sdk需要关注这个，默认是直接写入字符
std::string SdkImp::sdkGetEncryptLoginKey(const std::string& lkStr)
{
    std::string ret = lkStr;
    return ret;
}

