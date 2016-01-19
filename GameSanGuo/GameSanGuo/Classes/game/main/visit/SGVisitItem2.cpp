//
//  SGVisitItem2.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-18.
//
//

#include "SGVisitItem2.h"
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
SGVisitItem2::SGVisitItem2():wjlayer(NULL)
,zbLayer(NULL)
,zik(NULL)
,time(NULL)
,time1(NULL)
,index(0)
,zbIndex(0)
,labelTime(NULL)
,delegate(NULL)
,labelDesc(NULL)
,isStartActivity(false)
,officerTime(0)
,equipTime(0)
,type(0)
,visit1(NULL)
,left(NULL)
,limit_time(0)
,m_noticeCoinFree(NULL)
{
}
SGVisitItem2::~SGVisitItem2()
{
    
    CCLOG("~SGStoreItem");
}
bool SGVisitItem2::initWithSize(int num,int a,int b ,int c,int d)
{
    if (!SNSTableViewCellItem::init()) {
		return false;
	}

	bgbgbg = NULL;
    officerTime = a;
    equipTime = b;
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    zik = CCSprite::createWithSpriteFrameName("visit_kuang.png");
    zik->setPosition(ccp(0, 319/2 - zik->getContentSize().height/2 + 0));
    //zik->setPosition(ccp(0, 319/2 - zik->getContentSize().height/2 + 20));
    this->addChild(zik,-1);
    
    zik->setScaleX(s.width/zik->getContentSize().width);
    
    float posyRate = 1;
    if (s.height > 1024) {
        posyRate = s.height/940;
    }
    
    int shift = 0;
    int fontShift = 0;
    int timeShift = 0;
    int timeShiftx = 0;
    
    if (CCDirector::sharedDirector()->getWinSize().height == ipadResource.size.height)
    {
        shift = 50;
        fontShift = -25;
        timeShift = -55;
        timeShiftx = 50;
    }
    
    if (CCDirector::sharedDirector()->getWinSize().height == 960)
    {
        shift = 85;
        timeShift = -20;
        fontShift = -25;
        timeShiftx = 0;
    }

    
    zik->setScaleY(posyRate);
    
    
    CCSprite *jindu = CCSprite::createWithSpriteFrameName("visit_jdcao.png");

    CCSprite *font1 = NULL;
    int currTime = 0;
    if (num == 2) {
        currTime = a;
        font1 = CCSprite::createWithSpriteFrameName("tfqlwjdj.png");

    }
    else if(num == 3)
    {
        currTime = b;
        font1 = CCSprite::createWithSpriteFrameName("tfqlzb.png");
    }
    if (font1 != NULL)
    {
        addChild(font1,20);
        font1->setAnchorPoint(ccp(0, 1));
        font1->ignoreAnchorPointForPosition(false);
        font1->setPosition(ccp(-zik->getContentSize().width/2, zik->getContentSize().height/2 - font1->getContentSize().height/2 + fontShift+5));
    }
    


    if (num==2)
    {
        visit1 = SGButton::create("visit_anniuHui.png", "font_300_1.png", this, menu_selector(SGVisitItem2::confirmHandler),ccp(0, 3),false,true);
    }
    else
    {

    }
    visit1->setTag(num+110);
    
    //有免费元宝探访机会
    m_noticeCoinFree = CCSprite::createWithSpriteFrameName("publc_notice.png");
    m_noticeCoinFree->setPosition(ccp(visit1->getContentSize().width*0.92 , visit1->getContentSize().height*0.92));
    m_noticeCoinFree->setVisible(false);
    visit1->addChild(m_noticeCoinFree);
    
//    if(STATE_YES == SGPlayerInfo::sharePlayerInfo()->getPlayerNoticeById(402 , false))
    
    
    SGButton *visit10 = SGButton::create("visit_anniuHui.png", "font_tfz2.png", this, menu_selector(SGVisitItem2::confirmHandler),ccp(0, 3),false,true);
    visit10->setTag(num +120);
    visit10->setAnchorPoint(ccp(1, 1));
    visit10->ignoreAnchorPointForPosition(false);
    visit1->setAnchorPoint(ccp(1, 1));
    visit1->ignoreAnchorPointForPosition(false);
    visit1->setPosition(ccpAdd(zik->getPosition(), ccp(-zik->getContentSize().height / 10, -visit10->getContentSize().height * 2.3 + shift + 30)));  //+30by cgp
    visit10->setPosition(ccpAdd(visit1->getPosition(), ccp(visit10->getContentSize().width * 1.2, 0)));
    
    
    CCSprite *font2 = CCSprite::createWithSpriteFrameName("font_3xzb.png");
    
    if (font2)
    {
        addChild(font2,20);
        font2->setAnchorPoint(ccp(0, 0.5));
        font2->cocos2d::CCNode::ignoreAnchorPointForPosition(false);
        font2->setPosition(ccpAdd(visit1->getPosition(), ccp(-visit1->getContentSize().width / 2, visit1->getContentSize().height)));
    }

    
    
    float headhgt = SGMainManager::shareMain()->getVisitHgt();
    CCMenu *menu2 = CCMenu::create();
	menu2->setTouchPriority(128);
    menu2->setTag(111);
    menu2->setPosition(CCPointZero);
    menu2->setTouchPriority(128);
    this->addChild(menu2,5);
    menu2->addChild(visit1);
    menu2->addChild(visit10);

    int secs = currTime % 60;
    currTime /= 60;
    int minutes = currTime % 60;
    int hours = currTime / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, "%02d:%02d:%02d", hours, minutes, secs);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    if (num == 2) {
        
        if (officerTime == 0)
        {
            CCSprite *sprte = CCSprite::createWithSpriteFrameName("visit_free.png");
            visit1->setFontImage(sprte->displayFrame());
            m_noticeCoinFree->setVisible(true);
        }
        SGCCLabelTTF *timeMsg = SGCCLabelTTF::create(str_TongQueLayer_str21, FONT_PANGWA, FONTSIZE(13));
        this->addChild(timeMsg,10);
        timeMsg->setPosition(ccpAdd(zik->getPosition(), ccp(zik->getContentSize().width / 2.4+timeShiftx, +timeMsg->getContentSize().height * 5 + timeShift)));
        
        time = SGCCLabelTTF::create(timeStr, FONT_PANGWA, FONTSIZE(13));
        this->addChild(time,10);
        time->setPosition(ccpAdd(timeMsg->getPosition(), ccp(-timeMsg->getContentSize().width / 1.8 - time->getContentSize().width / 2, 0)));
        
        wjlayer = CCLayer::create();
        this->addChild(wjlayer,4);
        char wj[30] = {0};
        index = 0;
        zbIndex = 0;
        for (int i = 0; i<6; i++)
        {
            sprintf(wj,"wj_3.%d.png",i+1);
            CCSprite *sp = CCSprite::createWithSpriteFrameName(wj);
            wjlayer->addChild(sp);
            sp->setTag(i+20);
            sp->setScaleY(posyRate);
            sp->setPosition(ccpAdd(zik->getPosition(), ccp(size.width*i,0)));
        }
        this->schedule(schedule_selector(SGVisitItem2::updateTime), 1.0);
        this->startMove();
    }
    
    else if (num == 3) {
        if (equipTime == 0) {
            CCSprite *sprte = CCSprite::createWithSpriteFrameName("visit_free.png");
            visit1->setFontImage(sprte->displayFrame());
            m_noticeCoinFree->setVisible(true);
        }
        SGCCLabelTTF *timeMsg = SGCCLabelTTF::create(str_TongQueLayer_str21, FONT_BOXINFO, FONTSIZE(15));
        this->addChild(timeMsg,10);
        timeMsg->setPosition(ccpAdd(visit1->getPosition(), ccp(-visit1->getContentSize().width/2, visit1->getContentSize().height/2)));
        
        time1 = SGCCLabelTTF::create(timeStr, FONT_BOXINFO, FONTSIZE(15));
        this->addChild(time1,10);
        time1->setPosition(ccpAdd(visit1->getPosition(), ccp( -visit1->getContentSize().width/2, visit1->getContentSize().height)));
        
        zbLayer = CCLayer::create();
        this->addChild(zbLayer,4);
        char zb[30] = {0};
        index = 0;
        zbIndex = 0;
        for (int i = 0; i<6; i++)
        {
            sprintf(zb,"zb_3.%d.png",i+1);
            CCString *str_zbm = CCString::createWithFormat("zbm_3.%d.png",i+1);
            CCSprite *zbm = CCSprite::createWithSpriteFrameName(str_zbm->getCString());
            CCSprite *sp = CCSprite::createWithSpriteFrameName(zb);
            zbLayer->addChild(zbm,1);
            zbm->setTag(i+50);
            zbLayer->addChild(sp);
            sp->setTag(i+40);
            sp->setPosition(ccpAdd(zik->getPosition(), ccp(-zik->getContentSize().width*.16 + size.width*i,-10)));
            zbm->setScale(0.9);
            zbm->setPosition( ccpAdd(sp->getPosition(), ccp(0,zik->getContentSize().height*0.3)));
           
            
        }
        this->zbStartMove();
        this->schedule(schedule_selector(SGVisitItem2::updateTime1), 1.0);
    }
    if (font1) {
        labelDesc = SGCCLabelTTF::create(str_TongQueLayer_str22, FONT_BOXINFO, FONTSIZE(12));
        labelDesc->setAnchorPoint(ccp(1, 1));
        labelDesc->ignoreAnchorPointForPosition(false);
        labelDesc->setPosition(ccpAdd(font1->getPosition(), ccp(zik->getContentSize().width*0.8, -labelDesc->getContentSize().height)));
    }


    labelTime = SGCCLabelTTF::create("00:00", FONT_BOXINFO, FONTSIZE(15));
    labelTime->setAnchorPoint(ccp(0.5f, 0));
    labelTime->ignoreAnchorPointForPosition(false);
    labelTime->setPosition(ccpAdd(labelDesc->getPosition(), ccp(size.width*0.08, labelTime->getContentSize().height/3-15)));
    this->addChild(labelTime,20,1);
    labelTime->setVisible(false);
    
    return true;
}
void SGVisitItem2::updateTime()
{
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
        CCSprite *sprte = CCSprite::createWithSpriteFrameName("visit_free.png");
        visit1->setFontImage(sprte->displayFrame());
        m_noticeCoinFree->setVisible(true);
        return;
    }
    officerTime--;
    time->setString(timeStr);
}
void SGVisitItem2::updateTime1()
{
    int m = equipTime;
    int secs = m % 60;
    m /= 60;
    int minutes = m % 60;
    int hours = m / 60;
    char timeStr[32];
    memset(timeStr, 0, 32);
    snprintf(timeStr, 32, "%02d:%02d:%02d", hours, minutes, secs);
    
    if (equipTime == 0)
    {
        time1->setString(timeStr);
        CCSprite *sprte = CCSprite::createWithSpriteFrameName("visit_free.png");
        visit1->setFontImage(sprte->displayFrame());
        m_noticeCoinFree->setVisible(true);
        return;
    }
    equipTime--;
    time1->setString(timeStr);
}

void SGVisitItem2::startMove()
{
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    CCCallFunc *call = CCCallFunc::create(this , callfunc_selector(SGVisitItem2::moveEnd));
    wjlayer->runAction(CCSequence::create(CCDelayTime::create(5),CCMoveBy::create(1, ccp(-winSize.width, 0)),call,NULL));
}
void SGVisitItem2::moveEnd()
{
    index++;
    if (index >5)
    {
        index = 0;
    }
    int tag = (index -1)>=0?(index-1):5;

    CCSprite *sp = (CCSprite *)wjlayer->getChildByTag(tag+20);
//    CCSprite *wjm = (CCSprite *)wjlayer->getChildByTag(tag+30);
    this->startMove();
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
//    wjm->setPosition(ccpAdd(wjm->getPosition(), ccp(6*zik->getContentSize().width, 0)));
    sp->setPosition(ccpAdd(sp->getPosition(), ccp(6*winSize.width, 0)));

}
void SGVisitItem2::zbStartMove()
{
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    CCCallFunc *call = CCCallFunc::create(this , callfunc_selector(SGVisitItem2::zbMoveEnd));
    zbLayer->runAction(CCSequence::create(CCDelayTime::create(5),CCMoveBy::create(1, ccp(-size.width, 0)),call,NULL));
}
void SGVisitItem2::zbMoveEnd()
{
    zbIndex++;
    if (zbIndex >5)
    {
        zbIndex = 0;
    }
    int tag = (zbIndex -1)>=0?(zbIndex-1):5;
//    CCLOG("tag:%d",tag);
    CCSprite *sp = (CCSprite *)zbLayer->getChildByTag(tag+40);
    CCSprite *zbm = (CCSprite *)zbLayer->getChildByTag(tag+50);
    this->zbStartMove();
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    zbm->setPosition(ccpAdd(zbm->getPosition(), ccp(6*winSize.width, 0)));
    sp->setPosition(ccpAdd(sp->getPosition(), ccp(6*winSize.width, 0)));
    
}
void SGVisitItem2::confirmHandler(cocos2d::CCNode *sender)
{
    
    //////kanata
    //PAUSE_MUSIC;
    ////
    type = sender->getTag();
    if (delegate)
    {
        delegate->buyHandler(this);
    }

}

SGVisitItem2* SGVisitItem2::create(SGVisitDelegate2 *dele_,int num,int a,int b ,int c,int d)
{
	SGVisitItem2 *instance = new SGVisitItem2();
    instance->delegate = dele_;
	if (instance && instance->initWithSize(num,a,b,c,d)) {
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}

void SGVisitItem2::setIsCanTouch(bool touch)
{
    CCMenu *menu = (CCMenu *)this->getChildByTag(111);
    menu->setTouchEnabled(touch);
}

void SGVisitItem2::setActivityMsg(const time_t& currentTime,const time_t& startTime,const time_t& endTime,const std::string& activityMsg)
{
    CCString *timeStr = NULL;
    
    struct tm * ptm1 = localtime(&currentTime);
    char stime1[21] = {0};
    //strftime(stime1, 20, "%Y-%m-%dc %H:%M:%S", ptm1);
    strftime(stime1, 20, "%H:%M:%S", ptm1);
    std::string pRet1 = stime1;
    CCLOG("LocalTime: %s",pRet1.c_str());
    
    
    struct tm * ptm = localtime(&startTime);
    char stime[21] = {0};
    strftime(stime, 20, "%H:%M:%S", ptm);
    std::string pRet = stime;
    CCLOG("ServerTime: %s",pRet.c_str());
    labelTime->setVisible(true);
    isStartActivity = false;
    if (currentTime < startTime)
    {
        //开始活动
        limit_time = (int)(startTime - currentTime);
        if (limit_time > 3600 *2)
        {
            //lableDes = "时开启";//11.13暂时注释
            lableDes = " ";
            labelTime->setVisible(false);
            timeStr = CCString::createWithFormat("");
            //timeStr = CCString::createWithFormat("%s时开启", pRet.c_str());
        }
        else
        {
//            lableDes = "秒后开启";
//            timeStr = CCString::createWithFormat("%s秒后开启", getTimeString(limit_time));
            labelTime->setVisible(false);
            timeStr = CCString::createWithFormat("");
        }
        isStartActivity = true;
        
    }
    else if (currentTime > startTime && currentTime < endTime)
    {
        //结束活动
//        limit_time = (int)(endTime - currentTime);   //11.13暂时注释
//        lableDes = "秒后结束";
//        timeStr = CCString::createWithFormat("%s秒后结束", getTimeString(limit_time));
        labelTime->setVisible(false);
        timeStr = CCString::createWithFormat("");
        isStartActivity = false;
    }
    else
    {
		//这里timeStr可能为null//10.23//george
		timeStr = CCString::createWithFormat("");
		
        //活动已结束
        lableDes = " ";
        labelTime->setVisible(false);
        isStartActivity = false;
    }
    labelTime->setString(timeStr->getCString());
    unschedule(schedule_selector(SGVisitItem2::refreshTimer));
    this->schedule(schedule_selector(SGVisitItem2::refreshTimer), 1.0f);
}

void SGVisitItem2::refreshTimer()
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
        this->unschedule(schedule_selector(SGVisitItem2::refreshTimer));
        if (isStartActivity) {
            labelTime->setString(str_TongQueLayer_str25);
        }
        else{
            labelTime->setString(str_TongQueLayer_str26);
        }
    }
}


