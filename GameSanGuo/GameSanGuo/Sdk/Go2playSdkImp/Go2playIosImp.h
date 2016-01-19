//
//  Go2playIosImp.h
//  GameSanGuo
//
//  Created by bugcode on 15-1-30.
//
//

#ifndef __GameSanGuo__Go2playIosImp__
#define __GameSanGuo__Go2playIosImp__


/*
 go2play用户中心sdk的具体实现
*/

#include "SdkImp.h"
#include "cocos2d.h"
#import <UIKit/UIKit.h>

USING_NS_CC;


class Go2playIosImp : public SdkImp
{
public:
    
    Go2playIosImp();
    ~Go2playIosImp();
    
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
    void setUIWindow(UIWindow *window);

private:
    UIWindow *win;

};




#endif /* defined(__GameSanGuo__Go2playIosImp__) */
