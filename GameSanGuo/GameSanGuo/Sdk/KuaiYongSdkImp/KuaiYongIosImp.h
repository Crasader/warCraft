//
//  KuaiYongIosImp.h
//  GameSanGuo
//
//  Created by Fygo Woo on 15-3-26.
//
//

#ifndef __GameSanGuo__KuaiYongIosImp__
#define __GameSanGuo__KuaiYongIosImp__


/*
 快用用户中心sdk的具体实现
*/

#include "SdkImp.h"
#include "cocos2d.h"
#include <UIKit/UIKit.h>

USING_NS_CC;


class KuaiYongIosImp : public SdkImp
{
//通用接口。
public:
    
    KuaiYongIosImp();
    ~KuaiYongIosImp();
    
    //sdk注册ios实现
    virtual void sdkRegister(UserAndExtInfo &uei);
    //sdk登陆ios实现
    virtual void sdkLogin(UserAndExtInfo &uei);
    //sdk启动ios实现
    virtual void sdkStart();
    
    //sdk快速登陆ios实现
    virtual void sdkFastloginGame(UserAndExtInfo &uei);
    //验证与登陆一体化sdk的ios实现
    virtual void sdkLoginGame(UserAndExtInfo &uei);
    //facebook登陆
    virtual void sdkFBLogin(UserAndExtInfo &uei);
    //facebook 登出
    virtual void sdkFBLogout();
    virtual std::string sdkGetEncryptLoginKey(const std::string& lkStr);

//定制接口。
public:
    //显示登录注册界面。
    void showUserView();
    //设置运行窗口。
    void setUIWindow(UIWindow *window);
    //退出登录。
    void userLogOut();
    //显示用户设置界面。
    void setUpUser();
    //打开支付界面。
    void showPaymentView();
    
//定制字段。
public:
    //是否已登录快用。
    bool isKyLogin;
    
    //快用token，供发送给server。
    std::string kyToken;

private:
    UIWindow *win;

};




#endif /* defined(__GameSanGuo__KuaiYongIosImp__) */
