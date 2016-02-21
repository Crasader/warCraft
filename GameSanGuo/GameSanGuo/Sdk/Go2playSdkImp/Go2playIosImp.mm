//
//  Go2playIosImp.cpp
//  GameSanGuo
//
//  Created by bugcode on 15-1-30.
//
//

#include "Go2playIosImp.h"

//framework for go2play

#import "gmfTradeService.h"
#import "gmfActions.h"
#include "GlobalConfig.h"
#include "ExtClass.h"
#include "SGMainManager.h"
#include "md5.h"
#include "GameConfig.h"

//debug
//#define GO2PLAY_APPID       @"82929b0ddd0644e1a07ade68b86cb502"
//#define GO2PLAY_APPKEY      @"ccfbb8be27ff4673ba8943e2b26f3066"

//release
#define GO2PLAY_APPID       @"acb1e5702e0d464a93aa271e6ec9a20f"
#define GO2PLAY_APPKEY      @"032f72d81cda4db5b372422fcbd1c2ae"


//各种出错提示

#define LOGIN_GAME_FAILED   "登入遊戲失敗，請檢查後再嘗試。"
#define LOGIN_FAILED        "登入失敗，請檢查後再嘗試。"
#define REG_FAILED          "用戶名已存在，請重新輸入。"
#define FAST_LOGIN_FAILED   "快速登入失敗，請檢查後再嘗試。"
#define FB_LOGIN_FAILED     "facebook登入失敗，請檢查後再嘗試。"
#define LOGOUT_FAILED       "facebook登出失敗，請檢查後再嘗試。"

#define DELAY_REMOVE        10


Go2playIosImp::Go2playIosImp()
{
    
}

Go2playIosImp::~Go2playIosImp()
{
    
}

void Go2playIosImp::setUIWindow(UIWindow *window)
{
    win = window;
}

//go2play的sdk启动
void Go2playIosImp::sdkStart()
{
    [gmfTradeService setDebug:NO];
    [gmfTradeService start:win aid: GO2PLAY_APPID secrectkey: GO2PLAY_APPKEY extraparam:nil];
}



//oc 向go2play 请求注册消息
void Go2playIosImp::sdkRegister(UserAndExtInfo &uei)
{

    NSString *uName = [NSString stringWithUTF8String:uei.userName.c_str()];
    NSString *uPw = [NSString stringWithUTF8String:uei.userPwd.c_str()];
    
    //保存成员
    pwd = uei.userPwd;
    
    if (uName.length == 0 || uPw.length == 0)
    {
        //id or pwd error
        return;
    }
    
    NSMutableDictionary *jsonDic = [NSMutableDictionary dictionary];
    
    [jsonDic setObject:uName forKey:GMF_PARAM_USERNAME];
    [jsonDic setObject:uPw forKey:GMF_PARAM_PASSWORD];
    std::string uid = GameConfig::getUdit();
    NSString *uuid = [NSString stringWithUTF8String:uid.c_str()];
    [jsonDic setObject:uuid forKey:GMF_PARAM_CPUUID];

    
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    
    [gmfTradeService execute:GMF_SSO_REGISTER gmfcallback:^(BOOL success, NSDictionary *resultData)
    {
        UserAndExtInfo _uei;
        bool isSuc = false;
        
        //register success
        if (success == YES)
        {
            NSLog(@"register success");
            
            if (resultData != nil)
            {
                isSuc = true;
                //获取userId和userName
                NSNumber* uidNum = [resultData objectForKey:GMF_RESULT_USER_ID];
                NSNumberFormatter* nf = [[NSNumberFormatter alloc] init];
                NSString *uId = [nf stringFromNumber:uidNum];
                [nf release];
//                NSString* uId = [resultData objectForKey:GMF_RESULT_USER_ID]; //MM: ??
                NSString *uName = [resultData objectForKey:GMF_RESULT_USER_NAME];
                
                
                //userid 与 userName
                std::string _uId= [uId cStringUsingEncoding: NSUTF8StringEncoding];
                std::string _uName = [uName cStringUsingEncoding:NSUTF8StringEncoding];
                
                //密码也写入本地 uei中存储的密码
                _uei.userId = _uId;
                _uei.userName = _uName;
                _uei.userPwd = pwd;
                
                NSString *uExtra = [resultData objectForKey:GMF_RESULT_EXTRA];
                //如果有额外参数
                if (uExtra != nil)
                {
                    NSString *tRet = [NSString stringWithFormat:@"%@", uExtra];
                    NSLog(@"%@", tRet);
                    //other handle
                }
            }
            else if (resultData == nil)
            {
                SGMainManager::shareMain()->showMessage(REG_FAILED);
            }
        }
        else
        {
            SGMainManager::shareMain()->showMessage(REG_FAILED);
            NSString *errorStr = [resultData objectForKey:GMF_RESULT_ERROR_CODE];
            if (errorStr != nil)
            {
                NSString *tRet = [NSString stringWithFormat:@"%@", errorStr];
                NSLog(@"%@", tRet);
                //other code
            }
        }
        
        this->sdkPostProcess(isSuc, _uei);
    } extraparam:jsonDic];
}

//oc 向go2play 请求登陆消息
void Go2playIosImp::sdkLogin(UserAndExtInfo &uei)
{
    //从输入框中获取用户名和密码
    NSString *uName = [NSString stringWithUTF8String:uei.userName.c_str()];
    NSString *uPw = [NSString stringWithUTF8String:uei.userPwd.c_str()];
    
    pwd = uei.userPwd;
    
    NSMutableDictionary *jsonDic = [NSMutableDictionary dictionary];
    
    //用户名至少一个字符
    if (uName.length > 0)
    {
        [jsonDic setObject:uName forKey:GMF_PARAM_USERNAME];
    }
    //密码强制6位
    if (uPw.length >= 6)
    {
        [jsonDic setObject:uPw forKey:GMF_PARAM_PASSWORD];
    }
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();

    [gmfTradeService execute:GMF_SSO_LOGIN gmfcallback:^(BOOL success, NSDictionary *resultData)
    {
        UserAndExtInfo _uei;
        bool isSuc = false;
        
        if (success == YES)
        {
            //登陆成功，获取对应的用户信息
            if (resultData != nil)
            {
                isSuc = true;
                
                NSNumber* uidNum = [resultData objectForKey:GMF_RESULT_USER_ID];
                NSNumberFormatter* nf = [[NSNumberFormatter alloc] init];
                NSString *uId = [nf stringFromNumber:uidNum];
                [nf release];
//                NSString *uId = [resultData objectForKey:GMF_RESULT_USER_ID]; //MM: ??
                NSString *uName = [resultData objectForKey:GMF_RESULT_USER_NAME];
                
                //userid 与 userName
                std::string _uId= [uId cStringUsingEncoding: NSUTF8StringEncoding];
                std::string _uName = [uName cStringUsingEncoding:NSUTF8StringEncoding];
                
                //密码也写入本地 uei中存储的密码
                _uei.userId = _uId;
                _uei.userName = _uName;
                _uei.userPwd = pwd;
                
                NSString *uExtra = [resultData objectForKey:GMF_RESULT_EXTRA];
                
                //额外的参数
                if (uExtra != nil)
                {
                    NSString *tRet = [NSString stringWithFormat:@"%@", uExtra];
                    NSLog(@"%@", tRet);
                    //other handle
                }
            }
            else if (resultData == nil)
            {
                SGMainManager::shareMain()->showMessage(LOGIN_FAILED);
            }
        }
        else
        {
            //login failed
            SGMainManager::shareMain()->showMessage(LOGIN_FAILED);
            NSString *errorStr = [resultData objectForKey:GMF_RESULT_ERROR_CODE];
            //出错信息
            if (errorStr != nil)
            {
                NSString *tRet = [NSString stringWithFormat:@"%@", errorStr];
                NSLog(@"%@", tRet);
            }
        }
        
        this->sdkPostProcess(isSuc, _uei);
    } extraparam:jsonDic];

}


//快速登陆实现
void Go2playIosImp::sdkFastloginGame(UserAndExtInfo &uei)
{
    NSMutableDictionary *jsonDic = [NSMutableDictionary dictionary];

    std::string uid = GameConfig::getUdit();
    NSString *uuid = [NSString stringWithUTF8String:uid.c_str()];
    [jsonDic setObject:uuid forKey:GMF_PARAM_CPUUID];

    SGMainManager::shareMain()->getMainScene()->plat->showConnection();

    [gmfTradeService execute:GMF_SSO_CONNECT gmfcallback:^(BOOL success, NSDictionary *resultData)
    {
        //标记是否快速登陆成功
        UserAndExtInfo _uei;
        bool isSuccess = false;
        //快速登陆成功
        if (success == YES)
        {
            if (resultData != nil)
            {
                isSuccess = true;
                NSNumber* uidNum = [resultData objectForKey:GMF_RESULT_USER_ID];
                NSNumberFormatter* nf = [[NSNumberFormatter alloc] init];
                NSString *uId = [nf stringFromNumber:uidNum];
                [nf release];
//                NSString *uId = [resultData objectForKey:GMF_RESULT_USER_ID];
                NSString *uName = [resultData objectForKey:GMF_RESULT_USER_NAME];
                
                //userid 与 userName
                std::string _uId= [uId cStringUsingEncoding: NSUTF8StringEncoding];
                std::string _uName = [uName cStringUsingEncoding:NSUTF8StringEncoding];
                
                //初始至uei中
                _uei.userPwd = "password";
                _uei.userId = _uId;
                _uei.userName = _uName;
                
                
                NSString *uExtra = [resultData objectForKey:GMF_RESULT_EXTRA];
                if (uExtra != nil)
                {
                    NSString *tRet = [NSString stringWithFormat:@"%@", uExtra];
                    NSLog(@"%@", tRet);
                }
            }
            else if (resultData == nil)
            {
                SGMainManager::shareMain()->showMessage(FAST_LOGIN_FAILED);
            }
        }
        else
        {
            //错误处理
            SGMainManager::shareMain()->showMessage(FAST_LOGIN_FAILED);
            NSString *errorStr = [resultData objectForKey:GMF_RESULT_ERROR_CODE];
            if (errorStr != nil)
            {
                NSString *tRet = [NSString stringWithFormat:@"%@", errorStr];
                NSLog(@"%@", tRet);
            }
        }
        //验证后开始登陆游戏
        this->sdkFastloginGameProcess(isSuccess, _uei);
        
    } extraparam:jsonDic];
}

//普通登陆，本地已有帐户的情况
void Go2playIosImp::sdkLoginGame(UserAndExtInfo &uei)
{
    NSMutableDictionary *jsonDic = [NSMutableDictionary dictionary];
    
    NSString *uName = [NSString stringWithUTF8String:uei.userName.c_str()];
    NSString *uPw = [NSString stringWithUTF8String:uei.userPwd.c_str()];
    
    if (uName.length > 0)
    {
        [jsonDic setObject:uName forKey:GMF_PARAM_USERNAME];
    }
    //密码强制6位
    if (uPw.length >= 6)
    {
        [jsonDic setObject:uPw forKey:GMF_PARAM_PASSWORD];
    }
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();

    [gmfTradeService execute:GMF_SSO_LOGIN gmfcallback:^(BOOL success, NSDictionary *resultData)
     {
         UserAndExtInfo ueiInner;
         bool isSuc = false;
         
         if (success == YES)
         {
             //登陆成功，获取对应的用户信息
             if (resultData != nil)
             {
                 isSuc = true;
                 
                 NSNumber* uidNum = [resultData objectForKey:GMF_RESULT_USER_ID];
                 NSNumberFormatter* nf = [[NSNumberFormatter alloc] init];
                 NSString *uId = [nf stringFromNumber:uidNum];
                 [nf release];
//                 NSString *uId = [resultData objectForKey:GMF_RESULT_USER_ID];
                 NSString *uName = [resultData objectForKey:GMF_RESULT_USER_NAME];
                 
                 //userid 与 userName
                 std::string _uId= [uId cStringUsingEncoding: NSUTF8StringEncoding];
                 std::string _uName = [uName cStringUsingEncoding:NSUTF8StringEncoding];
                 
                 //密码也写入本地 uei中存储的密码
                 ueiInner.userId = _uId;
                 ueiInner.userName = _uName;
                 ueiInner.userPwd = pwd;
                 
                 NSString *uExtra = [resultData objectForKey:GMF_RESULT_EXTRA];
                 
                 //额外的参数
                 if (uExtra != nil)
                 {
                     NSString *tRet = [NSString stringWithFormat:@"%@", uExtra];
                     NSLog(@"%@", tRet);
                     //other handle
                 }
             }
             else if (resultData == nil)
             {
                 SGMainManager::shareMain()->showMessage(LOGIN_GAME_FAILED);
             }
         }
         else
         {
             //login failed
             SGMainManager::shareMain()->showMessage(LOGIN_GAME_FAILED);
             NSString *errorStr = [resultData objectForKey:GMF_RESULT_ERROR_CODE];
             //出错信息
             if (errorStr != nil)
             {
                 NSString *tRet = [NSString stringWithFormat:@"%@", errorStr];
                 NSLog(@"%@", tRet);
             }
         }
         
         this->sdkLoginGameProcess(isSuc, ueiInner);
         
     } extraparam:jsonDic];
}


//facebook登陆实现
void Go2playIosImp::sdkFBLogin(UserAndExtInfo &uei)
{
    NSMutableDictionary *jsonDic = [NSMutableDictionary dictionary];
    std::string uid = GameConfig::getUdit();
    NSString *uuid = [NSString stringWithUTF8String:uid.c_str()];
    [jsonDic setObject:uuid forKey:GMF_PARAM_CPUUID];

    [jsonDic setObject:@"facebook" forKey:GMF_PARAM_PROVIDER];
    
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    
    SGPlatFormIos *pt = SGMainManager::shareMain()->getMainScene()->plat;
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SGPlatFormIos::removeConnection),
                                                                   pt, DELAY_REMOVE, false);

    
    //不加网络锁屏，因为没有中断回调
    [gmfTradeService execute:GMF_SSO_LOGIN gmfcallback:^(BOOL success, NSDictionary *resultData)
    {
        bool isSuc = false;
        UserAndExtInfo _uei;
        if (success == YES)
        {
            if (resultData != nil)
            {
                isSuc = true;
//                NSNumber* uidNum = [resultData objectForKey:GMF_RESULT_USER_ID];
//                NSNumberFormatter* nf = [[NSNumberFormatter alloc] init];
//                NSString *uId = [nf stringFromNumber:uidNum];
//                [nf release];
                NSString *uId = [resultData objectForKey:GMF_RESULT_USER_ID];
                NSString *uName = [resultData objectForKey:GMF_RESULT_USER_NAME];
                
                //userid 与 userName
                std::string _uId= [uId cStringUsingEncoding: NSUTF8StringEncoding];
                std::string _uName = [uName cStringUsingEncoding:NSUTF8StringEncoding];
                
                //初始至uei中
                _uei.userPwd = "password";
                _uei.userId = _uId;
                _uei.userName = _uName;
                
                NSString *uExtra = [resultData objectForKey:GMF_RESULT_EXTRA];
                if (uExtra != nil)
                {
                    NSString *tRet = [NSString stringWithFormat:@"%@", uExtra];
                    NSLog(@"%@", tRet);
                }
            }
            else if (resultData == nil)
            {
                SGMainManager::shareMain()->showMessage(FB_LOGIN_FAILED);
            }
        }
        else
        {
            
            SGMainManager::shareMain()->showMessage(FB_LOGIN_FAILED);
            NSString *errorStr = [resultData objectForKey:GMF_RESULT_ERROR_CODE];
            if (errorStr != nil)
            {
                NSString *tRet = [NSString stringWithFormat:@"%@", errorStr];
                NSLog(@"%@", tRet);
            }
        }
        //处理后续的登陆逻辑
        this->sdkFBLoginProcess(isSuc, _uei);
    } extraparam:jsonDic];

}
//facebook 登出
void Go2playIosImp::sdkFBLogout()
{
    NSMutableDictionary *jsonDic = [NSMutableDictionary dictionary];
    //1.2 sdk新增参数传入。
    [jsonDic setObject:@"facebook" forKey:GMF_PARAM_PROVIDER];
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    std::string cppUid = ccud->getStringForKey("userId");
    NSString* userId = [NSString stringWithUTF8String:cppUid.c_str()];
    if (userId != nil)
        [jsonDic setObject:userId forKey:GMF_PARAM_USERNAME];
    
    [gmfTradeService execute:GMF_SSO_LOGOUT gmfcallback:^(BOOL success, NSDictionary *resultData)
    {
        if (success == YES)
        {
            NSLog(@"logout success");
        }
        else
        {
            NSLog(@"logout failed");
            SGMainManager::shareMain()->showMessage(LOGOUT_FAILED);
        }
        //登出的后续处理
        this->sdkFBLogoutProcess(success);
        
    } extraparam:jsonDic];

}
//加密字符串
std::string Go2playIosImp::sdkGetEncryptLoginKey(const std::string& lkStr)
{
    std::string enStr = lkStr + "gmobi.xdyou.com";
    MD5 md5(enStr);
    std::string finalStr = md5.md5();
    return finalStr;
}









