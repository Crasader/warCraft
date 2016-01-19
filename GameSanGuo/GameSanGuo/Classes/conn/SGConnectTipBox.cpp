//
//  SGConnectTipBox.cpp
//  GameSanGuo
//
//  Created by wwl on 13-12-18.
//
//

#include "SGConnectTipBox.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGGuideManager.h"
#include "SGShowString.h"
#include "SGLogicManager.h"
//#include "SGRootViewController.h"
//#include "EAGLView.h"
//static SGRootViewController *root = nil;

#include "ExtClass.h"

#define ROUND_PROMPT_W 500
#define ROUND_PROMPT_H 340


SGConnectTipBox::SGConnectTipBox()
{
//    root = [[SGRootViewController alloc]init];
//    [[EAGLView sharedEGLView]addSubview:[root view]];
//    [root showConn];
}

SGConnectTipBox::~SGConnectTipBox()
{
    CCLOG("~SGConnectTipBox");
//    [root hideConn];
//    [root.view removeFromSuperview];
//    [root release];
}

SGConnectTipBox* SGConnectTipBox::create(SGBoxDelegate *target, SGNetworkErrorType type)
{
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    SGConnectTipBox *reviveLayer = new SGConnectTipBox();
    reviveLayer->tipType = type;
//    reviveLayer->cocos2d::CCLayer::init()
    if(reviveLayer && reviveLayer->SGBaseBox::init(target, box_connTip, CCSizeMake(ROUND_PROMPT_W, ROUND_PROMPT_H)))
    {
        reviveLayer->autorelease();
        return reviveLayer;
    }
    
    CC_SAFE_DELETE(reviveLayer);
    return NULL;
}

void SGConnectTipBox::initMsg()
{
    this->setTouchEnabled(true);
    int w = this->getContentSize().width;
    int h = this->getContentSize().height;
    
    SGCCLabelTTF *label = SGCCLabelTTF::create(this->getTipName(tipType), FONT_PANGWA, 30, CCSize(ROUND_PROMPT_W * 0.8f, ROUND_PROMPT_H * 0.5f), kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    label->setPosition(ccp(w * 0.5, h * 0.5-20));
//    label->setColor(ccc3(0, 0, 0));
    this->addChild(label, 1);
    
    
//    SGButton *close = SGButton::create("box_btnbg.png", "public_font_qixiao.png", this, menu_selector(SGConnectTipBox::buttonClose),CCPointZero,false,true);
//    SGButton *close = SGButton::createFromLocal("box_btn_newbg.png", "取消", this, menu_selector(SGConnectTipBox::buttonClose),CCPointZero,FONT_PANGWA,ccWHITE,32);
//    this->addBtn(close);
//    close->setPosition(ccp(screenwidth * 0.5f + ROUND_PROMPT_W * 0.5f - close->getContentSize().width * 1, screenheight * 0.5f - ROUND_PROMPT_H * 0.5f + close->getContentSize().height * 0.9f));
    
//    SGButton *Ok = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGConnectTipBox::buttonOk), CCPointZero, false, true);
    SGButton *Ok = SGButton::createFromLocal("box_btn_newbg.png", str_Ok, this, menu_selector(SGConnectTipBox::buttonOk),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(Ok);
    Ok->setPosition(ccp(screenwidth * 0.5f /*- ROUND_PROMPT_W * 0.5f + Ok->getContentSize().width * 1*/, screenheight * 0.5f - ROUND_PROMPT_H * 0.5f + Ok->getContentSize().height * 0.9f));
//    Ok->setPosition(ccp(screenwidth * 0.5f, screenheight * 0.5f - ROUND_PROMPT_H * 0.5f - Ok->getContentSize().height * 0.5f));

    
}

const char* SGConnectTipBox::getTipName(SGNetworkErrorType type)
{
    CCString *text;
    switch (type) {
        case SGNET_NETWORK_NOT_AVAILABLE:
        {
            text = CCString::create(str_sgnet_network_not_available); //主公，網路發生問題，請檢查一下！
        }
            break;
        case SGNET_KICKED_BY_OTHER_DEVICE:
        {
            text = CCString::create(str_sgnet_kicked_by_other_device); //主公，您的帳號已在其他設備登入!
        }
            break;
        case SGNET_HEART_BEAT_FAILURE:
        {
            text = CCString::create(str_sgnet_heart_beat_failure); //主公，與伺服器連接已中斷!請稍候再試!
        }
            break;
        case SGNET_CANNOT_REACH_SERVER:
        {
            text = CCString::create(str_sgnet_cannot_reach_server); //主公，聯機中斷，網路發生問題。
        }
            break;
        case SGNET_NO_REPLY_ON_REQUEST:
        {
            text = CCString::create(str_sgnet_no_reply_on_request); //主公，伺服器過於火爆，請重試！
        }
            break;
        case SGNET_SEND_ERROR:
        {
            text = CCString::create(str_sgnet_send_error); //主公，伺服器繁忙，請稍後繼續！
        }
            break;
        case SGNET_SEND_BREAK:
        {
            text = CCString::create(str_sgnet_send_break); //主公，伺服器中斷，請稍後繼續！
        }
            break;
        case SGNET_RECEIVE_ERROR:
        {
            text = CCString::create(str_sgnet_receive_error); //主公，伺服器繁忙，請稍後再試！
        }
            break;
        case SGNET_RECEIVE_BREAK:
        {
            text = CCString::create(str_sgnet_receive_break); //主公，伺服器中斷，請稍後再試！
        }
            break;
        default:
            text = CCString::create(str_sgnet_common_problem);
            break;
    }
    return text->getCString();
}


void SGConnectTipBox::buttonOk()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    //MMHACK: SPEED 在这里把自动3倍速关闭，防止可能出现的在自动战斗中被T掉线。
    SGLogicManager::gi()->setGameSpeed(1.0f);
    
//    switch (this->tipType)
//    {
//        case 1:
//        {
//            //重新登入
//            SGMainManager::shareMain()->showwelcomelayer(true);
//        }
//            break;
//            
//        default:
//        {
//            SGMainManager::shareMain()->showwelcomelayer();//回到登入界面
//        }
//            break;
//    }
    
    ExtClassOfSDK::sharedSDKInstance()->channelStopConn();
    
    //掉线后，记得播放场景音乐。
    MUSIC_PLAY(MUSIC_SCENE);
    SGMainManager::shareMain()->showwelcomelayer();//回到登入界面

    SGGuideManager::shareManager()->clearGuideLayer();
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
//    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

void SGConnectTipBox::buttonClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    switch (this->tipType) {
        case 1:
        {
            CCDirector::sharedDirector()->end();
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }
            break;
        default:
        {
            SGMainManager::shareMain()->showwelcomelayer();
        }
            break;
    }
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    SGMainManager::shareMain()->getMainScene()->removeChild(this);
    this->removeFromParentAndCleanup(true);
}

void SGConnectTipBox::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

bool SGConnectTipBox::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}