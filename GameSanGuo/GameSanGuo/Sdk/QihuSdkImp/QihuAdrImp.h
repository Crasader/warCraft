//
//  QihuAdrImp.h
//  GameSanGuo
//
//  Created by Fygo Woo on 15-1-30.
//
//

#ifndef __GameSanGuo__QihuAdrImp__
#define __GameSanGuo__QihuAdrImp__

#include "cocos2d.h"

/*
 Qihu用户中心sdk安卓版的具体实现
*/

#include "SdkImp.h"

class QihuAdrImp : public SdkImp
{
public:
    
    QihuAdrImp();
    ~QihuAdrImp();
    
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

//定制函数。
public:
    //登陆
    void login();

//定制字段。
public:
    //是否已登录Qihu。
    bool isQhLogin;
    
    //快用token，供发送给server。
    std::string qhToken;
};




#endif /* defined(__GameSanGuo__QihuAdrImp__) */
