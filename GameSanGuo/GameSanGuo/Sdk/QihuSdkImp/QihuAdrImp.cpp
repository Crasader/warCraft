//
//  QihuAdrImp.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 15-1-30.
//
//

#include "QihuAdrImp.h"
#include "AndroidSDKAdp.h"
#include "SGMainManager.h"
#include "md5.h"
#include "QihuC2jCaller.h"

QihuAdrImp::QihuAdrImp()
{
    
}

QihuAdrImp::~QihuAdrImp()
{
    
}

//向go2play 请求注册消息
void QihuAdrImp::sdkRegister(UserAndExtInfo& uei)
{
	CCLOG("QihuAdrImp sdkRegister");
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    this->pwd = uei.userPwd;
    AndroidSDKAdp::getInstance()->registerUser(uei.userName, uei.userPwd);
}

//向go2play 请求登陆消息
void QihuAdrImp::sdkLogin(UserAndExtInfo& uei)
{
	CCLOG("QihuAdrImp sdkLogin");
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    this->pwd = uei.userPwd;
    AndroidSDKAdp::getInstance()->login(uei.userName, uei.userPwd);
}

//快速开始
void QihuAdrImp::sdkFastloginGame(UserAndExtInfo &uei)
{
	CCLOG("QihuAdrImp sdkFastloginGame");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	this->pwd = "adrPwd";
	AndroidSDKAdp::getInstance()->fastLoginGame();
}

//用账号登陆游戏
void QihuAdrImp::sdkLoginGame(UserAndExtInfo &uei)
{
	CCLOG("QihuAdrImp sdkFastloginGame");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	this->pwd = uei.userPwd;
	AndroidSDKAdp::getInstance()->loginGame(uei.userName, uei.userPwd);
}

//facebook登陆
void QihuAdrImp::sdkFBLogin(UserAndExtInfo &uei)
{
	CCLOG("QihuAdrImp sdkFBLogin");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	this->pwd = "fbLoginPwd";
	AndroidSDKAdp::getInstance()->fbLoginGo2play();
}

//facebook登出
void QihuAdrImp::sdkFBLogout()
{
	CCLOG("QihuAdrImp sdkFBLogout");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	AndroidSDKAdp::getInstance()->fbLogoutGo2play();
}

//sdk loginkey可能的特殊处理。有些没有验证的渠道sdk需要关注这个，添加私钥并进行加密。默认是不做任何处理，直接返回写入的字符串的值。
std::string QihuAdrImp::sdkGetEncryptLoginKey(const std::string& lkStr)
{
	std::string enStr = lkStr + "gmobi.xdyou.com";
    MD5 md5(enStr);
    std::string finalStr = md5.md5();
    CCLOG("QihuAdrImp sgelk %s", finalStr.c_str());
    return finalStr;
}

    //登陆
void QihuAdrImp::login()
{
	QihuC2jCaller::gi()->loginCall();
}





