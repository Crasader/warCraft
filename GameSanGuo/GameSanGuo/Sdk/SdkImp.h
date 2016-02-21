//
//  SdkImp.h
//  GameSanGuo
//
//  Created by bugcode on 15-1-30.
//
//

#ifndef __GameSanGuo__SdkImp__
#define __GameSanGuo__SdkImp__


/*
 所有对应外部用户sdk的父类，实现一般的的【登陆】,【注册】等功能
*/

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

//基本用户信息，可随时扩展
typedef struct _userAndExtInfo
{
    //用户名
    std::string userName;
    //用户密码
    std::string userPwd;
    //用户id
    std::string userId;
    
}UserAndExtInfo;


class SdkImp
{
public:
    //至少实现用户中心sdk的最基本的注册与登陆的接口
    
    //sdk的启动
    virtual void sdkStart(){};
    
    //sdk的注册
    virtual void sdkRegister(UserAndExtInfo &uei){};
    
    //sdk的登陆
    virtual void sdkLogin(UserAndExtInfo &uei){};
    
    //请求到sdk数据,写入本地（CCUSERDEFAULT）
    void sdkPostProcess(bool isSuc, UserAndExtInfo &uei);
    
    
    
    //sdk的快速登陆（包括与sdk的验证与登陆一起）
    virtual void sdkFastloginGame(UserAndExtInfo &uei){};
    
    //快速登陆请求sdk获取数据，开始一些处理
    void sdkFastloginGameProcess(bool isSuc, UserAndExtInfo &uei);
    
    //sdk存在帐号在本地，这里直接登陆（在不多次点击按钮的情况下直接完成向sdk验证帐户有效性和登陆游戏二步）
    virtual void sdkLoginGame(UserAndExtInfo &uei){};
    
    //本地已存在帐户数据，这里直接做验证后的登陆游戏处理
    void sdkLoginGameProcess(bool isSuc, UserAndExtInfo &uei);
    
    //sdk的Facebook帐号登陆
    virtual void sdkFBLogin(UserAndExtInfo &uei){};
    
    //Facebook登陆之后的本地处理
    void sdkFBLoginProcess(bool isSuc, UserAndExtInfo &uei);
    
    //facebook logout 登出处理
    virtual void sdkFBLogout(){};
    
    //登出处理之后的本地处理
    void sdkFBLogoutProcess(bool isSuc);
    
    //sdk loginkey可能的特殊处理。有些没有验证的渠道sdk需要关注这个，添加私钥并进行加密。默认是不做任何处理，直接返回写入的字符串的值。
    virtual std::string sdkGetEncryptLoginKey(const std::string& lkStr);
    
    //获得PWD，不允许改写。
    std::string sdkGetPwd() { return pwd; };
    
    
protected:
    //记录用户密码，方便ADR调用。
    std::string pwd;
};





#endif /* defined(__GameSanGuo__SdkImp__) */
