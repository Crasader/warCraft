//
//  SGVisitItem1.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-18.
//
//

#include "SGVisitItem1.h"
#include "cocos-ext.h"
#include "SGLayout.h"
#include "SGBaseShow.h"
#include "SGMenu.h"
#include "SGMainManager.h"
#include "Utils.h"

#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "AppMacros.h"

USING_NS_CC_EXT;
SGVisitItem1::SGVisitItem1()
:choose(0)
,layer(NULL)
,zbLayer(NULL)
,zik(NULL)
,time(NULL)
,labelTime(NULL)
,labelDesc(NULL)
,index(0)
,zbIndex(0)
,delegate(NULL)
,officerTime(0)
,visit1(NULL)
,left(NULL)
,limit_time(0)
,isStartActivity(false)
,m_noticeGoldFree(NULL)
{
}
SGVisitItem1::~SGVisitItem1()
{
    
    CCLOG("~SGStoreItem");
}
bool SGVisitItem1::initWithSize(int num,int a,int b,bool huoVisitOpen)
{
    if (!SNSTableViewCellItem::init()) {
		return false;
	}
    officerTime = a;
	bgbgbg = NULL;
    
    s = CCDirector::sharedDirector()->getWinSize();
   
    zik = CCSprite::createWithSpriteFrameName("visit_zik.png");
    
    zik->setAnchorPoint(ccp(0.5, 1));
    zik->ignoreAnchorPointForPosition(false);
    zik->setPosition(ccp(0, zik->getContentSize().height/2 + 190));
    addChild(zik,-10);
//    zik->setScaleX(s.width/zik->getContentSize().width);
    
    float posyRate = 1.1;
    if (s.height > 1024) {
        posyRate = s.height/970;
    }
    int shift = 0;
    int fontShift = 0;
    int timeShift = 0;
    int timeShiftx = 0;
    if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height)
    {
        shift = -30;
        fontShift = 15;
        timeShift = 10;
        timeShiftx = 50;
    }
    if (CCDirector::sharedDirector()->getWinSize().height == 960)
    {
        posyRate = s.height / 940;
        if (CCDirector::sharedDirector()->getWinSize().width <= 640)
        {
            shift = -24;
        }
        else
        {
           shift = 15;
        }
        
        fontShift = 40;
        timeShift = 20;
        timeShiftx = 0;
    }
    else{
        shift = -35;
    }

    //zik->setScaleY(posyRate);
    

    CCSprite *font1 = NULL;
    CCSprite *font2 = NULL;
    int currTime = 0;
    if (num == 0) {       
        font1 = CCSprite::createWithSpriteFrameName("font_tfcqwj.png");
        if (huoVisitOpen)
        {
            
            font2 = CCSprite::createWithSpriteFrameName("font_huosong.png");
        }
        else
        {
            font2 = CCSprite::createWithSpriteFrameName("font_3xwj.png");
        }
    }
    else if(num == 1)
    {        
        font1 = CCSprite::createWithSpriteFrameName("font_tfcqzb.png");
        font2 = CCSprite::createWithSpriteFrameName("font_3xzb.png");        
    }
    if (font1) {
        font1->setAnchorPoint(ccp(0, 1));
        font1->ignoreAnchorPointForPosition(false);
        addChild(font1,20);
        font1->setPosition(ccp(-zik->getContentSize().width/2, zik->getContentSize().height/2 -5 ));
        font1->setVisible(false);
    }
    if (font2) {
        addChild(font2,20);
        font2->setAnchorPoint(ccp(0, 0.5));
        font2->cocos2d::CCNode::ignoreAnchorPointForPosition(false);
        font2->setPosition(ccpAdd(ccp(-zik->getContentSize().width/2, 0), ccp(font2->getContentSize().width / 2.5, -font2->getContentSize().height * 2 + fontShift)));
        font2->setVisible(false);
    }

    if (num == 0) {
        visit1 = SGButton::create("visit_anniu.png", "font_300.png", this, menu_selector(SGVisitItem1::test),CCPointZero,false,true);

    }else
    {
        //visit1 = SGButton::create("visit_anniu.png", "font_100.png", this, menu_selector(SGVisitItem1::test),CCPointZero,false,true);
    }
    

    
    
    visit1->setTag(num+110);
    SGButton *visit10 = SGButton::create("visit_anniu.png", "font_tfz1.png", this, menu_selector(SGVisitItem1::confirmHandler),CCPointZero,false,true);
    visit10->setTag(num +120);
    visit10->setAnchorPoint(ccp(1, 1));
    visit10->ignoreAnchorPointForPosition(false);
    visit1->setAnchorPoint(ccp(1, 1));
    visit1->ignoreAnchorPointForPosition(false);
    visit1->setPosition(ccp(120, 50));
    visit10->setPosition(ccpAdd(visit1->getPosition(), ccp(visit10->getContentSize().width * 1.2, 0)));
    visit10->setVisible(false);
    visit10->setEnabled(false);
    
    CCMenu *menu1 = CCMenu::create();
	menu1->setTouchPriority(128);
    menu1->setTag(111);
    menu1->setPosition(CCPointZero);
    this->addChild(menu1,5);
    menu1->addChild(visit1);
    menu1->addChild(visit10);
    
    
    //买物品送武将
//    SGCCLabelTTF *infoTips = SGCCLabelTTF::create("赠送奖励：武将或者物品", FONT_PANGWA, 30);
//    this->addChild(infoTips, 8);
//    infoTips->setPosition(ccpAdd(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2),
//                                 ccp(0, visit10->getPositionY())));
    
    //有免费元宝探访机会
    m_noticeGoldFree = CCSprite::createWithSpriteFrameName("publc_notice.png");
    m_noticeGoldFree->setPosition(ccp(visit1->getContentSize().width*0.92 , visit1->getContentSize().height*0.92));
    m_noticeGoldFree->setVisible(false);
    visit1->addChild(m_noticeGoldFree);

    if (officerTime == 0)
    {
        CCSprite *sprte = CCSprite::createWithSpriteFrameName("visit_free.png");
        visit1->setFontImage(sprte->displayFrame());
        m_noticeGoldFree->setVisible(true);
    }
   
    
//    if(STATE_YES == SGPlayerInfo::sharePlayerInfo()->getPlayerNoticeById(401 , false))
    
    currTime = a;
    //left->setPercentage(b);
    int secs = currTime % 60;
    currTime /= 60;
    int minutes = currTime % 60;
    int hours = currTime / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, "%02d:%02d:%02d", hours, minutes, secs);
    SGCCLabelTTF *timeMsg = SGCCLabelTTF::create(str_TongQueLayer_str21, FONT_PANGWA, FONTSIZE(13));
    this->addChild(timeMsg,10);
    timeMsg->setPosition(ccpAdd(zik->getPosition(), ccp(zik->getContentSize().width / 2.4 +timeShiftx, -timeMsg->getContentSize().height  + timeShift)));
    
    time = SGCCLabelTTF::create(timeStr, FONT_PANGWA, FONTSIZE(13));
    this->addChild(time,10);
    time->setPosition(ccpAdd(timeMsg->getPosition(), ccp(-timeMsg->getContentSize().width / 1.8 - time->getContentSize().width / 2, 0)));
     CCSize size=CCDirector::sharedDirector()->getWinSize();
    
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/wj_6.1.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/wj_6.2.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/wj_6.3.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/wj_6.4.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/wj_6.5.plist",RES_TYPE_LAYER_UI ,sg_visitShowCard);

    
    if (num == 0)
    {
        
    layer = CCLayer::create();
    this->addChild(layer,4);
    char wj[30] = {0};
    index = 0;
    zbIndex = 0;
    for (int i = 0; i<5; i++)
    {
        sprintf(wj,"wj_6.%d.png",i+1);
        CCSprite *sp = CCSprite::createWithSpriteFrameName(wj);
        layer->addChild(sp,1);
        sp->setAnchorPoint(ccp(0, 0));
        sp->setTag(i);
        sp->setScaleY(posyRate);
        sp->setPosition(ccpAdd(zik->getPosition(), ccp(size.width * i - sp->getContentSize().width/2,
                                                       -zik->getContentSize().height*.8 * posyRate)));
        sp->setVisible(false);
    }        
        this->startMove();
    }
    
    else if (num == 1) {      
        
        zbLayer = CCLayer::create();
        this->addChild(zbLayer);
        char zb[30] = {0};
        index = 0;
        zbIndex = 0;
        for (int i = 0; i<6; i++)
        {
            sprintf(zb,"zb_6.%d.png",i+1);
            CCString *str_zbm = CCString::createWithFormat("zbm_6.%d.png",i+1);
            CCSprite *zbm = CCSprite::createWithSpriteFrameName(str_zbm->getCString());
            CCSprite *sp = CCSprite::createWithSpriteFrameName(zb);
            zbLayer->addChild(zbm,1);
            zbm->setTag(i+100);
            zbLayer->addChild(sp);
            zbm->setPosition(ccpAdd(zik->getPosition(), ccp(- zik->getContentSize().width*.17 + size.width*i, zik->getContentSize().height*.26)));
            sp->setAnchorPoint(ccp(0, 0));
            sp->setTag(i+10);
            sp->setPosition(ccpAdd(zik->getPosition(), ccp(size.width * i - zik->getContentSize().width*.6,-zik->getContentSize().height*.4)));
        }
        this->zbStartMove();
    }
    this->schedule(schedule_selector(SGVisitItem1::updateTime), 1.0);
    
    if (font1) {
        labelDesc = SGCCLabelTTF::create(str_TongQueLayer_str22, FONT_BOXINFO, FONTSIZE(12));
        labelDesc->setAnchorPoint(ccp(1, 1));
        labelDesc->ignoreAnchorPointForPosition(false);
        labelDesc->setPosition(ccpAdd(font1->getPosition(), ccp(zik->getContentSize().width*0.8  , -labelDesc->getContentSize().height)));
    }

    labelTime = SGCCLabelTTF::create("00:00", FONT_BOXINFO, FONTSIZE(15));

    labelTime->setAnchorPoint(ccp(0.5f, 0));
    labelTime->ignoreAnchorPointForPosition(false);

    labelTime->setPosition(ccpAdd(labelDesc->getPosition(), ccp(size.width*0.08, labelTime->getContentSize().height/3-15)));
    labelTime->setVisible(false);
    this->addChild(labelTime,20,1);

    return true;
}
void SGVisitItem1::test(CCNode *psender)
{
    //////kanata
   // PAUSE_MUSIC;
  //  MUSIC_STOP;
   //////
    CCLOG("....");
    choose  = psender->getTag();
   // CCLOG("%d",choose);
    if (delegate) {
        delegate->buyLegHandler(this);
    }
}
void SGVisitItem1::startMove()
{
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    CCCallFunc *call = CCCallFunc::create(this , callfunc_selector(SGVisitItem1::moveEnd));
    layer->runAction(CCSequence::create(CCDelayTime::create(8),CCMoveBy::create(0.8f, ccp(-winSize.width, 0)),call,NULL));
}
void SGVisitItem1::moveEnd()
{
    index++;
    if (index >4)
    {
        index = 0;
    }
    int tag = (index -1)>=0?(index-1):4;
//    CCLOG("tag:%d",tag);
    CCSprite *sp = (CCSprite *)layer->getChildByTag(tag);
//    CCSprite *wjm = (CCSprite *)layer->getChildByTag(tag+6);
    this->startMove();
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
//    wjm->setPosition(ccpAdd(wjm->getPosition(), ccp(5*zik->getContentSize().width, 0)));
    sp->setPosition(ccpAdd(sp->getPosition(), ccp(5*winSize.width, 0)));
//    CCLOG("moveEnd%d",index);;
}
void SGVisitItem1::zbStartMove()
{
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    CCCallFunc *call = CCCallFunc::create(this , callfunc_selector(SGVisitItem1::zbMoveEnd));
    zbLayer->runAction(CCSequence::create(CCDelayTime::create(4),CCMoveBy::create(0.8f, ccp(-size.width, 0)),CCDelayTime::create(4),call,NULL));
}
void SGVisitItem1::zbMoveEnd()
{
    zbIndex++;
    if (zbIndex >5)
    {
        zbIndex = 0;
    }
    int tag = (zbIndex -1)>=0?(zbIndex-1):5;
//    CCLOG("tag:%d",tag);
    CCSprite *sp = (CCSprite *)zbLayer->getChildByTag(tag+10);
    CCSprite *zbm = (CCSprite *)zbLayer->getChildByTag(tag+100);
    this->zbStartMove();
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    zbm->setPosition(ccpAdd(zbm->getPosition(), ccp(6*winSize.width, 0)));
    sp->setPosition(ccpAdd(sp->getPosition(), ccp(6*winSize.width, 0)));

}

void SGVisitItem1::updateTime()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile("sgvisitlayer/sgvisitlayer1.plist");
    int m = officerTime;
    int secs = m % 60;
    m /= 60;
    int minutes = m % 60;
    int hours = m / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, "%02d:%02d:%02d", hours, minutes, secs);
    
    if (officerTime == 0)
    {
        time->setString(timeStr);
//        officerTime = 30;
//        this->unschedule(schedule_selector(SGVisitItem1::updateTime));
        CCSprite *sprte = CCSprite::createWithSpriteFrameName("visit_free.png");
        visit1->setFontImage(sprte->displayFrame());
        m_noticeGoldFree->setVisible(true);
        return;
    }

    officerTime--;
    time->setString(timeStr);
}

void SGVisitItem1::confirmHandler(cocos2d::CCNode *sender)
{
    choose = sender->getTag();
    if (delegate)
    {
        delegate->buyLegHandler(this);
    }   

}
SGVisitItem1* SGVisitItem1::create(SGVisitDelegate *dele_,int num,int time,int per,bool huoVisitOpen)
{
	SGVisitItem1 *instance = new SGVisitItem1();
    instance->delegate = dele_;
	if (instance && instance->initWithSize(num,time,per,huoVisitOpen)) {
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}

void SGVisitItem1::setIsCanTouch(bool touch)
{
    CCMenu *menu = (CCMenu *)this->getChildByTag(111);
    if (menu) {
        menu->setTouchEnabled(touch);
    }
}

void SGVisitItem1::setActivityMsg(const time_t& currentTime,const time_t& startTime,const time_t& endTime,const std::string& activityMsg)
{
    CCString *timeStr = NULL;
    struct tm * ptm1 = localtime(&currentTime);
    char stime1[21] = {0};
    memset(stime1, 0, 21);
    strftime(stime1, 20, "%H:%M:%S", ptm1);
    std::string pRet1 = stime1;
    CCLOG("LocalTime: %s",pRet1.c_str());
    
    
    struct tm * ptm = localtime(&startTime);
    char stime[21] = {0};
    memset(stime, 0, 21);
    strftime(stime, 20, "%H:%M:%S", ptm);
    std::string pRet = stime;
    CCLOG("ServerTime: %s",pRet.c_str());
    isStartActivity = false;
    labelTime->setVisible(true);
    CCLOG("Oher ServerTime:%s",getCurTimeStringTo1970(startTime));
    if (currentTime < startTime)
    {
        //开始活动
        limit_time = (int)(startTime - currentTime);
        if (limit_time > 3600 *2)
        {
//            lableDes = "时开启"; //1113暂时注释
//            timeStr = CCString::createWithFormat("%s时开启", pRet.c_str());
            lableDes = " ";
            labelTime->setVisible(false);
            timeStr = CCString::createWithFormat("");
        }
        else
        {
//            lableDes = "秒后开启";  //1113暂时注释
//            timeStr = CCString::createWithFormat("%s秒后开启", getTimeString(limit_time));
            labelTime->setVisible(false);
            timeStr = CCString::createWithFormat("");
        }
        isStartActivity = true;
    }
    else if (currentTime > startTime && currentTime < endTime)
    {
        //结束活动
//        limit_time = (int)(endTime - currentTime); //1113暂时注释
//        lableDes = "秒后结束";
//        timeStr = CCString::createWithFormat("%s秒后结束", getTimeString(limit_time));
        labelTime->setVisible(false);
        timeStr = CCString::createWithFormat("");
        isStartActivity = false;
    }
    else
    {
		//进到这里timeStr可能会出现null//10.23//george
		timeStr = CCString::createWithFormat("");
        //活动已结束
        lableDes = " ";
        labelTime->setVisible(false);
        isStartActivity = false;
    }
    labelTime->setString(timeStr->getCString());
   // labelDesc->setString(activityMsg.c_str());
//    labelTime->setVisible(false);/////11.13暂时添加
//    labelDesc->setVisible(false);/////
    unschedule(schedule_selector(SGVisitItem1::refreshTimer));
    this->schedule(schedule_selector(SGVisitItem1::refreshTimer), 1.0f);
}

void SGVisitItem1::refreshTimer()
{
    limit_time -= 1;
    if (limit_time <= 3600*2  || !isStartActivity) {
        if (lableDes.compare(str_TongQueLayer_str23) == 0) {
            lableDes = str_TongQueLayer_str24;
        }
        CCString *timeStr = CCString::createWithFormat("%s%s", getTimeString(limit_time),lableDes.c_str());
        labelTime->setString(timeStr->getCString());

    }
    if (limit_time <= 1) {
        // 刷新数据，将过期的数据隐藏掉
        this->unschedule(schedule_selector(SGVisitItem1::refreshTimer));
        if (isStartActivity) {
            labelTime->setString(str_TongQueLayer_str25);
        }
        else{
            labelTime->setString(str_TongQueLayer_str26);
        }
    }
}


