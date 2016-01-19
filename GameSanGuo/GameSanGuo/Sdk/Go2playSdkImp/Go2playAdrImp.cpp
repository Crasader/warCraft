//
//  Go2playAdrImp.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 15-1-30.
//
//

#include "Go2playAdrImp.h"
#include "AndroidSDKAdp.h"
#include "SGMainManager.h"
#include "md5.h"

Go2playAdrImp::Go2playAdrImp()
{
    
}

Go2playAdrImp::~Go2playAdrImp()
{
    
}

//向go2play 请求注册消息
void Go2playAdrImp::sdkRegister(UserAndExtInfo& uei)
{
	CCLOG("Go2playAdrImp sdkRegister");
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    this->pwd = uei.userPwd;
    AndroidSDKAdp::getInstance()->registerUser(uei.userName, uei.userPwd);
}

//向go2play 请求登陆消息
void Go2playAdrImp::sdkLogin(UserAndExtInfo& uei)
{
	CCLOG("Go2playAdrImp sdkLogin");
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    this->pwd = uei.userPwd;
    AndroidSDKAdp::getInstance()->login(uei.userName, uei.userPwd);
}

//快速开始
void Go2playAdrImp::sdkFastloginGame(UserAndExtInfo &uei)
{
	CCLOG("Go2playAdrImp sdkFastloginGame");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	this->pwd = "adrPwd";
	AndroidSDKAdp::getInstance()->fastLoginGame();
}

//用账号登陆游戏
void Go2playAdrImp::sdkLoginGame(UserAndExtInfo &uei)
{
	CCLOG("Go2playAdrImp sdkFastloginGame");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	this->pwd = uei.userPwd;
	AndroidSDKAdp::getInstance()->loginGame(uei.userName, uei.userPwd);
}

//facebook登陆
void Go2playAdrImp::sdkFBLogin(UserAndExtInfo &uei)
{
	CCLOG("Go2playAdrImp sdkFBLogin");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	this->pwd = "fbLoginPwd";
	AndroidSDKAdp::getInstance()->fbLoginGo2play();
}

//facebook登出
void Go2playAdrImp::sdkFBLogout()
{
	CCLOG("Go2playAdrImp sdkFBLogout");
	SGMainManager::shareMain()->getMainScene()->plat->showConnection();
	AndroidSDKAdp::getInstance()->fbLogoutGo2play();
}

//sdk loginkey可能的特殊处理。有些没有验证的渠道sdk需要关注这个，添加私钥并进行加密。默认是不做任何处理，直接返回写入的字符串的值。
std::string Go2playAdrImp::sdkGetEncryptLoginKey(const std::string& lkStr)
{
	std::string enStr = lkStr + "gmobi.xdyou.com";
    MD5 md5(enStr);
    std::string finalStr = md5.md5();
    CCLOG("Go2playAdrImp sgelk %s", finalStr.c_str());
    return finalStr;
}





