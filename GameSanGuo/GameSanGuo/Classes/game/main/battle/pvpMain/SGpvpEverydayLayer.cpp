 //
//  SGpvpEverydayLayer.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-12.
//
//

#include "SGpvpEverydayLayer.h"
#include "SGMainManager.h"
#include "SGFirstSubLayer.h"
#include "SGPlayerInfo.h"
#include "SGFriend.h"
#include "SGOtherPlayerInfo.h"
#include "SGChat.h"
#include "SGChatMessageCenter.h"
#include "SGMail.h"
#include "SGStaticDataManager.h"
#include "SGMainLayer.h"
#include "SGAnnouncementLayer.h"
#include "SGFirstStrengBox.h"
#include "SGFirstAdvanceBox.h"
#include "SGMainLayer.h"
#include "SGSocketClient.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "DayTaskModule.pb.h"
#include "SGGuideManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGTeamgroup.h"
#include "SurvivalModule.pb.h"
#include "SGSjzData.h"
#include "SGCantAdvanceBox.h"
#include "PvpFight.pb.h"
#include "SGBattleData.h"
#include "SGBattleMainLayer.h"
#include "SGPvpGiftInfoBox.h"
#include "SGStringConfig.h"


SGpvpEverydayLayer::SGpvpEverydayLayer():
_count(0),
_gifts(NULL),
progress(NULL),
icon1(NULL),
icon2(NULL),
icon3(NULL),
btn1(NULL),
btn2(NULL),
btn3(NULL),
mm(NULL),
bgbox(NULL)
{
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
   
}
SGpvpEverydayLayer::~SGpvpEverydayLayer()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_firstLayer);

    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVP_EVERYDAY_GIFTGET);
   
    
    if (menu) {
//        menu->removeAllChildrenWithCleanup(true);
        menu->removeFromParentAndCleanup(true);
    }
    CC_SAFE_RELEASE(_gifts);
    
}

SGpvpEverydayLayer *SGpvpEverydayLayer::create(int count ,CCArray*arrary)
{
    SGpvpEverydayLayer *firstLayer = new SGpvpEverydayLayer();
    if (firstLayer && firstLayer->init(NULL,sg_pvpeverydaylayer))
    {
//        firstLayer->_gifts = arrary;
        firstLayer->_count = count;
        firstLayer->initView(arrary);
        firstLayer->autorelease();
        return firstLayer;
    }
    CC_SAFE_DELETE(firstLayer);
    return NULL;
}
 bool SGpvpEverydayLayer::init(const char *fileName_, int tag_)
{
    if (!SGBaseLayer::init(NULL, sg_pvpeverydaylayer))
    {
        return false;
    }

    _gifts=CCArray::create();
    _gifts->retain();
    return true;
}
void SGpvpEverydayLayer::initView(CCArray *arrary)
{
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_pvpeverydaylayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpvpeverydaylayer/sgpvpeverydaylayer.plist", RES_TYPE_LAYER_UI, sg_pvpeverydaylayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->addObserver(MSG_PVPENTER_SEASONWAR, this, callfuncO_selector(SGpvpEverydayLayer::showSeasonMainLayer));
    notification->addObserver(MSG_PVP_EVERYDAY_GIFTGET, this, callfuncO_selector(SGpvpEverydayLayer::aftergetgift));
    
    float b = SGMainManager::shareMain()->getBottomHeight();
    float headH = SGMainManager::shareMain()->getVisitHgt();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
//      _gifts=CCArray::create();
    _gifts->addObjectsFromArray(arrary);
//      _gifts->retain();
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,2);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,3);
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_pvpeverydaylayer);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30, bg->getContentSize().width, s.height - 45 - gonggao_3_2->getContentSize().height - b);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b)));
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_SeasonListLayer_str8, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this,
                                                  menu_selector(SGpvpEverydayLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
	
    this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, title_bg->getPosition().y-title_bg->getContentSize().height/2));
    
    bgbox = CCSprite::createWithSpriteFrameName("pvpeveryday.png");
    int x = (s.height-headH-b)/2+b;
    bgbox->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter),ccp(0,x)));
    this->addChild(bgbox);
    
    mm = CCMenu::create();
    mm->setPosition(ccp(0,0));
    bgbox->addChild(mm);
    
    progress = CCProgressTimer::create( CCSprite::createWithSpriteFrameName("pvpeveryprogress.png"));
    progress->setType(kCCProgressTimerTypeBar);
    progress->setMidpoint(ccp(1,0));
    progress->setBarChangeRate(ccp(0, 1));
    progress->setPosition(ccpAdd(ccp(0,0), ccp(progress->getContentSize().width*.71,progress->getContentSize().height*.52)));
    bgbox->addChild(progress);
    
    
    icon1 = CCSprite::create();
    icon2 = CCSprite::create();
    icon3 = CCSprite::create();
    
    CCSprite* gift = CCSprite::createWithSpriteFrameName("5reward.png");//礼包图tag1
    icon1->addChild(gift,1,1);
    
    icon1->setPosition(ccp(gift->getContentSize().width*3.7,gift->getContentSize().height*1.1));
    icon2->setPosition(ccpAdd(icon1->getPosition(),ccp(0,gift->getContentSize().height*2.2)));
    icon3->setPosition(ccpAdd(icon2->getPosition(),ccp(0,gift->getContentSize().height*2.2)));
    icon1->setTag(200);
    icon2->setTag(201);
    icon3->setTag(202);
    
    bgbox->addChild(icon1);
    bgbox->addChild(icon2);
    bgbox->addChild(icon3);
    
    CCSprite *kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");//礼包框tag2
    icon1->addChild(kuang,1,2);
    CCSprite *ylq = CCSprite::createWithSpriteFrameName("pvpyilingqu.png");//已领取tag3
    ylq->setPosition(ccp(-gift->getContentSize().width*.4,gift->getContentSize().height*.4));
    icon1->addChild(ylq,1,3);
    
    CCSprite* gift2 = CCSprite::createWithSpriteFrameName("10reward.png");//礼包图tag1
    icon2->addChild(gift2,1,1);
    CCSprite *kuang2 = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");//礼包框tag2
    icon2->addChild(kuang2,1,2);
    CCSprite *ylq2 = CCSprite::createWithSpriteFrameName("pvpyilingqu.png");//已领取tag3
    ylq2->setPosition(ylq->getPosition());
    icon2->addChild(ylq2,1,3);
    
    CCSprite* gift3 = CCSprite::createWithSpriteFrameName("15reward.png");//礼包图tag1
    icon3->addChild(gift3,1,1);
    CCSprite *kuang3 = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");//礼包框tag2
    icon3->addChild(kuang3,1,2);
    CCSprite *ylq3 = CCSprite::createWithSpriteFrameName("pvpyilingqu.png");//已领取tag3
    ylq3->setPosition(ylq->getPosition());
    icon3->addChild(ylq3,1,3);
    
    btn1 = SGButton::createFromLocal("store_exchangebtnbg.png", str_SeasonListLayer_str7, this, menu_selector(SGpvpEverydayLayer::btnclick), CCPointZero, FONT_PANGWA, ccc3(0x43,0x1f,0x0e),32, false, true);
    btn1->setOutsideColor(ccYELLOW);
    btn1->setTag(100);
    btn1->setPosition(ccpAdd(icon1->getPosition(),ccp(gift->getContentSize().width*1.5,0)));
    mm->addChild(btn1);
    
    btn2 = SGButton::createFromLocal("store_exchangebtnbg.png", str_SeasonListLayer_str7, this, menu_selector(SGpvpEverydayLayer::btnclick), CCPointZero, FONT_PANGWA, ccc3(0x43,0x1f,0x0e),32, false, true);
    btn2->setOutsideColor(ccYELLOW);
    btn2->setTag(101);
    btn2->setPosition(ccpAdd(icon2->getPosition(),ccp(gift->getContentSize().width*1.5,0)));
    mm->addChild(btn2);
    
    btn3 = SGButton::createFromLocal("store_exchangebtnbg.png", str_SeasonListLayer_str7, this, menu_selector(SGpvpEverydayLayer::btnclick), CCPointZero, FONT_PANGWA, ccc3(0x43,0x1f,0x0e),32, false, true);
    btn3->setOutsideColor(ccYELLOW);
    btn3->setTag(102);
    btn3->setPosition(ccpAdd(icon3->getPosition(),ccp(gift->getContentSize().width*1.5,0)));
    mm->addChild(btn3);
    
    this->setView();
    
 
}

void SGpvpEverydayLayer::setIsCanTouch(bool isTouch)
{
    mm->setEnabled(isTouch);
    menu->setEnabled(isTouch);

}

void SGpvpEverydayLayer::btnclick(SGButton *btn)
{
    int tag = btn->getTag();
    SGVipGift* gift = (SGVipGift*)_gifts->objectAtIndex(tag-100);
    
    SGPvpGiftInfoBox *GiftInfoBox = SGPvpGiftInfoBox::create(this, gift,1);
    SGMainManager::shareMain()->showBox(GiftInfoBox);

}
void SGpvpEverydayLayer::showSeasonMainLayer(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PvpEntryResponse *response = (main::PvpEntryResponse *)request->m_msg;
    SGBattleData *battleData = NULL;
    SGRankPlayer *selfPlayer = NULL;
    if (response) {
        
        float var1 = response->winratio();//当前胜率
        int var2 = response->prestige();//天梯声望
        int var3 = response->continuouswin();//连胜
        int var4 = response->maxcontinuouswin();//最大连胜
        int var5 = response->awardscore();//天梯积分
        int var6 = response->prestigerank();//声望排名
        int var7 = response->pvpcount();//第几界
        int var8 = response->pvprank();//天梯排名
        int var9 = response->pvpscore();//天梯等级
        int var10= response->pvpfightcount();//参战次数
        
        bool hasDayReward = response->hasdayreward();//每日奖励是否可领取
        bool hasSeasonReward = response->hasseasonreward();//赛季奖励是否可领取

        
        std::string str = response->pvprankname();//当前军衔
        std::string str2 = response->pvpenddate();//截止时间
        
        CCString *name = SGPlayerInfo::sharePlayerInfo()->getNickName();
        int iconId = SGPlayerInfo::sharePlayerInfo()->getPlayerImageID();
        std::string servername = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
        std::string serverid = CCUserDefault::sharedUserDefault()->getStringForKey("serverid");
        CCString *str3 = CCString::createWithFormat("%s",serverid.c_str());
        // create(const char *name, int hid, int rank, int levl, int ftime, float rwin)
        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var8,var9,var10, var1,str3->intValue(),servername.c_str());
        battleData = SGBattleData::create(str2.c_str(), var4, var3, var5, var2,var1,var6,str.c_str(),var7 , hasDayReward , hasSeasonReward);
        
        
        //        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, 0, 0, var4, winr);
        battleData->setStartTime(CCString::create(response->specstarttime()));
        battleData->setEndTime(CCString::create(response->specendtime()));
        SGPlayerInfo::sharePlayerInfo()->setPvpRank(selfPlayer);
        if (selfPlayer) {
            SGPlayerInfo::sharePlayerInfo()->setplayerPvpLevel(selfPlayer->getLevel());
        }
        SGPlayerInfo::sharePlayerInfo()->setplayerPvpNum(response->awardscore());
    }
    if (battleData != NULL) {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
        if (!layer) {
            layer = SGBattleMainLayer::create(battleData);
        }
        mainScene->setVisible(true);
        mainScene->addShowLayer(layer);
    }
}

void SGpvpEverydayLayer::backHandler()
{
//    main::PvpEntryRequest *request = new main::PvpEntryRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
    SGMainManager::shareMain()->showPvpMatchLayer();
}

void SGpvpEverydayLayer::getHandle(SGVipGift *gif)
{
    if (gif->getHaveBuy()==2) {
        SG_SHOW_WINDOW(str_SeasonListLayer_str2);
    }
    else if (gif->getHaveBuy()==3)
    {
        SG_SHOW_WINDOW(str_SeasonListLayer_str3);
    }
    else if (gif->getHaveBuy()==1)
    {
        main::PvPDayAwardGiftGetRequest *request = new main::PvPDayAwardGiftGetRequest();
        request->set_key(gif->getPvpgifttype());
        SGSocketClient::sharedSocketClient()->send(MSG_PVP_EVERYDAY_GIFTGET, request);

    }
}

void SGpvpEverydayLayer::aftergetgift(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PvPDayAwardGiftGetResponse *response = (main::PvPDayAwardGiftGetResponse *)request->m_msg;
    if (response->state()==1) {
        SG_SHOW_WINDOW(str_Get_succeed);
        int count = response->cout();
        SGVipGift* pvpgift=NULL;
        switch (count) {
            case 5:
               pvpgift= (SGVipGift*)_gifts->objectAtIndex(0);
                break;
            case 10:
                pvpgift = (SGVipGift*)_gifts->objectAtIndex(1);
                break;
            case 15:
                pvpgift = (SGVipGift*)_gifts->objectAtIndex(2);
                break;
            default:
                break;
        }
        if (pvpgift) {
            pvpgift->setHaveBuy(3);
            this->setgiftsbtns();
        }
    }
    else
    {
        SG_SHOW_WINDOW(str_SeasonListLayer_str4);
    }

}

void SGpvpEverydayLayer::setView()
{
    int round = _count;
    if (round>15) {
        round=15;
    }
    float rate = (float)round/15;
    progress->setPercentage(rate*100);
    this->setgiftsbtns();

}

void SGpvpEverydayLayer::setgiftsbtns()
{
    for (int i=0;i<_gifts->count();i++)
    {
        SGVipGift *pvpgift = (SGVipGift* )_gifts->objectAtIndex(i);
        if (pvpgift->getHaveBuy()==1)
        {//能领
            SGButton* btn =(SGButton*)mm->getChildByTag(100+i);
            btn->thisinit("reward_lingq.png", "", this, menu_selector(SGpvpEverydayLayer::btnclick), CCPointZero, FONT_PANGWA, ccc3(0x43,0x1f,0x0e),32, false, true);           
            
            CCSprite *icon = (CCSprite*)bgbox->getChildByTag(200+i);
            CCSprite* y =(CCSprite*)icon->getChildByTag(1);
            y->setColor(ccWHITE);
            y =(CCSprite*)icon->getChildByTag(2);
            y->setColor(ccWHITE);
            y =(CCSprite*)icon->getChildByTag(3);
            y->setVisible(false);
            
        }
        else if (pvpgift->getHaveBuy()==2)
        {//不能领
            //            SGButton* btn =(SGButton*)mm->getChildByTag(100+i);
            
            CCSprite *icon = (CCSprite*)bgbox->getChildByTag(200+i);
            CCSprite* y =(CCSprite*)icon->getChildByTag(1);
            y->setColor(ccWHITE);
            y =(CCSprite*)icon->getChildByTag(2);
            y->setColor(ccWHITE);
            y =(CCSprite*)icon->getChildByTag(3);
            y->setVisible(false);
            
        }
        else if (pvpgift->getHaveBuy()==3)//领过了
        {
            SGButton* btn =(SGButton*)mm->getChildByTag(100+i);
            btn->thisinit("reward_lingq_hui.png", "", this, menu_selector(SGpvpEverydayLayer::btnclick), CCPointZero, FONT_PANGWA, ccc3(0x43,0x1f,0x0e),32, false, true);
            
            CCSprite *icon = (CCSprite*)bgbox->getChildByTag(200+i);
            CCSprite* y =(CCSprite*)icon->getChildByTag(1);
            y->setColor(ccGRAY);
            y =(CCSprite*)icon->getChildByTag(2);
            y->setColor(ccGRAY);
            y =(CCSprite*)icon->getChildByTag(3);
            y->setVisible(true);
            
        }
    }
    
}


