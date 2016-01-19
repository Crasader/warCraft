//
//  SGUpdateClientBox.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-18.
//
//

#include "SGUpdateClientBox.h"
#include "GameMusicmanage.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "PlatformAdp.h"
#include "SGUpdateManager.h"
#include "SGMainManager.h"
#include "SGFirstLayer.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "AndroidSDKAdp.h"
#endif
#include "GlobalConfig.h"

#if GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD
#include "auxiliary/UCSdk.h"
#include "auxiliary/UCSdkCallback.h"
#include "android/jni/CUCGameSdk.h"
using namespace ucgamesdk;
#endif

#if (GCCT_CHINA_KUAIYONG_IOS_THIRD == GAME_CHANNEL_TYPE)
#include "KuaiYongIosImp.h"
#endif

#include "SdkController.h"

SGUpdateClientBox::SGUpdateClientBox()
:updateType(SGUCB_RECOMMEND_UPDATE_CLIENT)
{
    ;
}

SGUpdateClientBox::~SGUpdateClientBox()
{
    this->setTouchEnabled(false);
    menu->setTouchEnabled(false);
}

SGUpdateClientBox* SGUpdateClientBox::create(SG_UPDATE_CLIENT_BOX_TYPE type, std::string extArg /*= ""*/)
{
    SGUpdateClientBox *update = new SGUpdateClientBox();
    if(update && update->init(NULL, box_updateClientBox, CCSizeMake(500, 340), true))
    {
        update->_extArg = extArg;
        update->updateType = type;
        update->viewDidLoad();
        update->autorelease();
        return update;
    }
    CC_SAFE_DELETE(update);
    return NULL;
}

void SGUpdateClientBox::viewDidLoad()
{
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    fontbg->setPreferredSize(CCSizeMake(430, 170));
    fontbg->setPosition(ccpAdd(biao->getPosition(), ccp(0, -120)));
    
    SGButton *buttonOk = SGButton::createFromLocal("box_btn_newbg.png",
                                                  str_Ok,
                                                  this,
                                                  menu_selector(SGUpdateClientBox::buttonClickOK),
                                                  CCPointZero,
                                                  FONT_PANGWA,
                                                  ccWHITE,32);

    int okBtnShift = 0;
    const char *titleTips = str_UpdateFail_str3;
    const char *tips = str_UpdateFail_str8;
    int titleSize = 34;
    int noticeSize = 28;
    //不同更新类型给不同提示
    if (updateType == SGUCB_FORCE_UPDATE_CLIENT)
    {
        //强制更新
        tips = str_UpdateFail_str4;
    }
    else if (updateType == SGUCB_RECOMMEND_UPDATE_CLIENT)
    {
        SGButton *skipUpdateBtn = NULL;
        //建议更新
        tips = str_UpdateFail_str8;
        //建议更新中加入 “继续游戏”按钮
        skipUpdateBtn = SGButton::createFromLocal("box_btn_newbg.png",
                                                  str_SkipUpdate,
                                                  this,
                                                  menu_selector(SGUpdateClientBox::buttonClickSkipUpdate),
                                                  CCPointZero,
                                                  FONT_PANGWA,
                                                  ccWHITE,28);
        this->addBtn(skipUpdateBtn);
        skipUpdateBtn->setPosition(ccpAdd(center, ccp(120, -120)));
        //确定按钮向左偏移
        okBtnShift = -120;
    }
    else if (updateType == SGUCB_NETWORK_BUSY)//网络忙，各种联不上
    {
        titleTips = str_NetworkExp_str1;
        tips = str_UpdateFail_str12;
    }
    else if (updateType == SGUCB_RESPONSE_ERR)//联上了，但是服务器回的不对
    {
        titleTips = str_ServerExp_str1;
        tips = str_UpdateFail_str7;
    }
    else if (updateType == SGUCB_EXIT_GAME)
    {
        SGButton* cancelBtn = NULL;
        titleTips = str_exit_game_title;
        tips = str_exit_game_notice;
        //加入 “取消”按钮
        cancelBtn = SGButton::createFromLocal("box_btn_newbg.png",
                                                  str_Cancel,
                                                  this,
                                                  menu_selector(SGUpdateClientBox::buttonClickSkipUpdate),
                                                  CCPointZero,
                                                  FONT_PANGWA,
                                                  ccWHITE,28);
        this->addBtn(cancelBtn);
        cancelBtn->setPosition(ccpAdd(center, ccp(120, -120)));
        //确定按钮向左偏移
        okBtnShift = -120;
    }
    
    //提示标题
    SGCCLabelTTF *labelTitle = SGCCLabelTTF::create(titleTips, FONT_PANGWA, titleSize);

    //提示内容
    SGCCLabelTTF *labelContent = SGCCLabelTTF::create(tips, FONT_PANGWA, noticeSize, CCSizeMake(380, 200));

    labelTitle->setPosition(ccpAdd(center, ccp(0, 125)));
    labelContent->setPosition(ccpAdd(center, ccp(0, -30)));
    
    buttonOk->setPosition(ccpAdd(center, ccp(okBtnShift, -120)));
    
    addChild(labelTitle);
    addChild(labelContent);
    addBtn(buttonOk);
    
    //本游戏的最高触控层级-384，用于退出游戏。
    menu->setTouchMode(kCCTouchesOneByOne);
    menu->setTouchPriority(-384);
    menu->setTouchEnabled(true);
    //这里设置为-321是因为firstlayer上的menu层级有-254，退出BOX出现时候，必须屏蔽一切触控。
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchPriority(-321);
    this->setTouchEnabled(true);
}

//确定
void SGUpdateClientBox::buttonClickOK()
{
    this->setTouchEnabled(false);
    menu->setTouchEnabled(false);
    EFFECT_PLAY(MUSIC_BTN);

#if (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
    CUCGameSdk::destroyFloatButton();
    CUCGameSdk::exitSDK(NULL);
#elif (GAME_ACCOUNT_TYPE == GCAT_KUAIYONG) //kuaiyong
    KuaiYongIosImp* kyImp = (KuaiYongIosImp*) SdkController::gi()->getSdkImp();
    kyImp->userLogOut();
#else
    
#endif

    //如果是更新，转到appstore 或者 googleplay
    if (updateType == SGUCB_FORCE_UPDATE_CLIENT || updateType == SGUCB_RECOMMEND_UPDATE_CLIENT)
    {
#if (PLATFORM == IOS)
        //跳转 appstore
        CCLOG("Enter App_Store");
        OcCaller::getInstance()->openUrl(GlobalConfig::gi()->getClientUpdateUrl());
        //强制退出
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#else
        CCDirector::sharedDirector()->end();
#endif
#else
        //跳转到google play
        CCLOG("Enter Google Play");
        this->boxClose();
        AndroidSDKAdp::getInstance()->openUrl(GlobalConfig::gi()->getClientUpdateUrl());
        //安卓不能end()，因为这样会把activity也给关掉了，所以交给玩家去选择。
        //强制退出
//        CCDirector::sharedDirector()->end();
#endif
    }
    else if (updateType == SGUCB_NETWORK_BUSY || updateType == SGUCB_RESPONSE_ERR)// 网络繁忙 -> 包括N种网络不好，或者直接没联网
    {
        //点击确定直接退出，六次都连不上，木法玩
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#else
        CCDirector::sharedDirector()->end();
#endif
    }
    else if ( updateType == SGUCB_EXIT_GAME)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#else
        CCDirector::sharedDirector()->end();
#endif
    }
    else//其他情况 预留
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#else
        CCDirector::sharedDirector()->end();
#endif
    }
}

void SGUpdateClientBox::buttonClickSkipUpdate(CCObject *obj)
{
    this->setTouchEnabled(false);
    menu->setTouchEnabled(false);
    EFFECT_PLAY(MUSIC_BTN);
    if (updateType == SGUCB_RECOMMEND_UPDATE_CLIENT) //跳过更新，继续游戏
    {
        SGUpdateManager::gi()->trigger();
    }
    this->boxClose();
}

bool SGUpdateClientBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
