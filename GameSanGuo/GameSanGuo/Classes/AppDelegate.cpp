//
//  GameSanGuoAppDelegate.cpp
//  GameSanGuo
//
//  Created by wenlong w on 12-11-19.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#include "AppDelegate.h"
#include "GameConfig.h"
#include "AppMacros.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "GlobalConfig.h"
USING_NS_CC;



AppDelegate::AppDelegate()
{
}
AppDelegate::~AppDelegate()
{
}
//重载应用程序启动后调用的处理函数
bool AppDelegate::applicationDidFinishLaunching()
{

    //通过CCDirector的静态函数sharedDirector来获取单件显示设备管理器指针
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView *view = CCEGLView::sharedOpenGLView();
    //通过CCEGLView的静态函数sharedOpenGLView来获取单件管理实例对象并将其地址通过CCDirector的成员函数setOpenGLView传递给显示设备管理器
    pDirector->setOpenGLView(view);
    CCSize frameSize = view->getFrameSize();
    
    //设置图形加载模式。

    //设置图形加载模式。
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    CCTextureCache::sharedTextureCache()->setLoadingMode((LIM_OPTION)CGP_PLATFORM);

    //HACK BEGIN
    std::vector<std::string> resDirVec;
    //绑定初始化资源，这些图从包里读取。
    handleResourceSearchPaths(resDirVec);
    ResourceManager::sharedInstance()->bindInitialImageTexture();
    
    //延后设定新资源搜索目录，剩下的图优先从下载中读取。
    std::string downLoadPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    resDirVec.push_back(downLoadPath);//优先搜索documents路径
    handleResourceSearchPaths(resDirVec);
    //HACK END
    
    GameConfig::prohibitLockScreen();
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	// 绑定publicUI资源
//	ResourceManager::sharedInstance()->bindCommonImageTexture();//加入public.plist
    SGMainManager::shareMain()->startGame();
    UDID;
    CCLOG("UDID:%s",GameConfig::getUdit().c_str());
    

    return true;
}

//重载应用程序转入后台时调用的函数，如电话打进来
void AppDelegate::applicationDidEnterBackground()
{
    /*
     CCDirector::sharedDirector()->end();
     
     #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
     exit(0);
     #endif
     */
    CCLOG("暂停显示设备的渲染处理");
    
    //暂停显示设备的渲染处理
    CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
    // if you use SimpleAudioEngine, it must be paused
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
#endif
    CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_retran_backHaHa", CCUserDefault::sharedUserDefault()->getBoolForKey("pvp_retran")); //PVP登陆
    CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_retran", false); //PVP登陆
    
    //切出游戏发送消息。
    main::AlanWake *aw = new main::AlanWake();
    aw->set_arg1(0);
    SGSocketClient::sharedSocketClient()->send(MSG_ALAN_WAKE, aw, false);
}

//重载应用程序恢复前台时调用的函数
void AppDelegate::applicationWillEnterForeground()
{
    CCLOG("恢复显示设备的渲染处理");
    //恢复显示设备的渲染处理 
    CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    //如果使用了声音引擎，这里进行恢复设置
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
#endif
    CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_retran", CCUserDefault::sharedUserDefault()->getBoolForKey("pvp_retran_backHaHa")); //PVP登陆标志
    
    //重回游戏发送消息。
    main::AlanWake *aw = new main::AlanWake();
    aw->set_arg1(1);
    SGSocketClient::sharedSocketClient()->send(MSG_ALAN_WAKE, aw, false);
}

void AppDelegate::handleResourceSearchPaths(const std::vector<std::string>& vec)
{
    CCEGLView *view = CCEGLView::sharedOpenGLView();
    CCSize frameSize = view->getFrameSize();
    
    std::vector<std::string> resDirOrder;
    
    for (int k = 0; k < vec.size(); k++)
    {
        resDirOrder.push_back(vec.at(k));
    }
    
    if (frameSize.width == iphoneResource.size.width)
    {
        CCLOG("iphoneResource");
        designResolutionSize = DESIGN_RESOLUTION_640X960;
        resDirOrder.push_back(ipadResource.directory);
        CCFileUtils::sharedFileUtils()->setSearchPaths(resDirOrder);
        view->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionExactFit);
    }
    else if (frameSize.width == iphonehdResource.size.width)
    {
        CCLOG("iphonehdResource");
        designResolutionSize = DESIGN_RESOLUTION_320X480;
        resDirOrder.push_back(ipadResource.directory);
        CCFileUtils::sharedFileUtils()->setSearchPaths(resDirOrder);
        CCLOG("designResolutionSize:%f",designResolutionSize.width);
        
    }
    else if (frameSize.width == ipadResource.size.width)
    {
        CCLOG("ipadResource");
        designResolutionSize = DESIGN_RESOLUTION_768X1024;
        resDirOrder.push_back(ipadResource.directory);
        CCFileUtils::sharedFileUtils()->setSearchPaths(resDirOrder);
    }
    else if (frameSize.width == ipadhdResource.size.width)
    {
        designResolutionSize = DESIGN_RESOLUTION_768X1024;
        resDirOrder.push_back(ipadResource.directory);
        CCFileUtils::sharedFileUtils()->setSearchPaths(resDirOrder);
        CCLOG("ipadhdResource%f",ipadhdResource.size.width/designResolutionSize.width);
        
        view->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionExactFit);
    }
    else //hell boy...
    {
        CCLOG("iphoneResource");
        designResolutionSize = CCSizeMake(640, 1136);
        resDirOrder.push_back(ipadResource.directory);
        CCFileUtils::sharedFileUtils()->setSearchPaths(resDirOrder);
        view->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
    }
}

