//
//  Go2playAdrImp.h
//  GameSanGuo
//
//  Created by Fygo Woo on 15-1-30.
//
//

#ifndef __GameSanGuo__Go2playAdrImp__
#define __GameSanGuo__Go2playAdrImp__

#include "cocos2d.h"

/*
 go2play用户中心sdk安卓版的具体实现
*/

#include "SdkImp.h"

class Go2playAdrImp : public SdkImp
{
public:
    
    Go2playAdrImp();
    ~Go2playAdrImp();
    
    //注册账号
    virtual void sdkRegister(UserAndExtInfo& uei);
    //登陆账号
    virtual void sdkLogin(UserAndExtInfo& uei);
    //快速开始
    virtual void sdkFastloginGame(UserAndExtInfo &uei);
    //用账号登陆游戏
    virtual void sdkLoginGame(UserAndExtInfo &uei);
    //facebook登陆
    virtual void sdkFBLogin(UserAndExtInfo &uei);
    //facebook登出
    virtual void sdkFBLogout();
    //sdk loginkey可能的特殊处理。有些没有验证的渠道sdk需要关注这个，添加私钥并进行加密。默认是不做任何处理，直接返回写入的字符串的值。
    virtual std::string sdkGetEncryptLoginKey(const std::string& lkStr);
};




#endif /* defined(__GameSanGuo__Go2playAdrImp__) */
