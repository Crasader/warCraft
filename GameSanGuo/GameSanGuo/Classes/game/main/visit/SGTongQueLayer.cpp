//
//  SGTongQueLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 2014-2-26.
//
//

#include "SGTongQueLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "cocos-ext.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "LotteryModule.pb.h"
#include "SGStaticDataManager.h"

#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#define ZHUFUEFFECTAG   112233
#define TONGQUEITEMSTARTAG  300
#define AUTOREFRESH      0
#define SHOUDONGEFRESH   1
#define GETYANGXIN  1

#define ITEMNAMETAG  996
USING_NS_CC_EXT;
SGTongQueLayer::SGTongQueLayer():startPosId(0)
,stopPosId(10)
,m_yangxingItem()
,havegold(NULL)
,havejg(NULL)
,yangxinLabel(NULL)
,updateProgress(NULL)//进度条
,tognqueDokbBt(NULL)
,guangBioa(NULL)
,refeshTimeCount(NULL)
,dokbCostLabel(NULL)
,yongqiCount(NULL)
,nXunZhangMax(0)//勋章上限
,refeshCostCount(NULL)
,refeshCost(NULL)
,rewardEffect(NULL)
,guangBioaGuan(NULL)
,yangxindian(NULL)
,refreshBtn(NULL)
,m_dokbFlag(NULL)
, refeshFlag(NULL)
,moveHaHaTime(0)
,refreshTimeCount(0)
,gotItem(NULL)
, wid(0)
, canDobk(false)
, m_spendMedal (0)//当前翻牌所需要的勋章数目
, m_spendGold (0)//消耗元宝数
, m_medal (0)//当前勋章数
, m_spendCoins (0)//刷新消耗铜钱数
, m_blessValue (0)//祝福值
,itemsPos(CCPointZero)
,dobkTimeLimit(false)
{
   
}
SGTongQueLayer::~SGTongQueLayer()
{
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_TONGQUE_DOBK);
    notification->removeObserver(this, MSG_TONGQUE_REFRESH);
    notification->removeObserver(this, MSG_TONGQUE_YANGXIN);
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_tongQueLayer);
    unschedule(schedule_selector(SGTongQueLayer::startMoveHaHa));
    unschedule(schedule_selector(SGTongQueLayer::startRefreshTimeHaHa));

}
SGTongQueLayer *SGTongQueLayer::create(int spendMedal,int spendGold,int medal,int spendCoins,int refreshTime,int blessValue, std::vector<SGItemInfo> &tongqueItem, SGItemInfo &yangxing)
{
    SGTongQueLayer *visitLayer = new SGTongQueLayer();
    if (visitLayer && visitLayer->init(NULL, sg_tongQueLayer))
    {
        visitLayer->initData(spendMedal,spendGold,medal,spendCoins,refreshTime,blessValue,tongqueItem, yangxing);
        visitLayer->initView();
        visitLayer->autorelease();
        return visitLayer;
    }
    CC_SAFE_DELETE(visitLayer);
    return NULL;
}
void SGTongQueLayer::initData(int spendMedal,int spendGold,int medal,int spendCoins,int refreshTime,int blessValue, std::vector<SGItemInfo> &tongqueItem, SGItemInfo &yangxing)
{
    m_spendMedal = spendMedal;//当前翻牌所需要的勋章数目
    m_spendGold = spendGold;//消耗元宝数
    m_medal = medal;//当前勋章数
    m_spendCoins = spendCoins;//刷新消耗铜钱数
    refreshTimeCount = refreshTime;//倒计时时间
    m_blessValue = blessValue;//祝福值
    
    m_tongqueItem.clear();
    m_tongqueItem = tongqueItem;
    m_yangxingItem = yangxing;
    gotItem = NULL;
}

void SGTongQueLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgvisitlayer/sgvisitlayer2.plist",RES_TYPE_LAYER_UI ,sg_tongQueLayer);
    ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);
    ResourceManager::sharedInstance()->bindTexture("sgtongquelayer/sgtongquelayer.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/G_Startlight.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist",RES_TYPE_LAYER_UI ,sg_tongQueLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_TONGQUE_DOBK,
                              this,
                              callfuncO_selector(SGTongQueLayer::tongQueDobkListener));
    
    notification->addObserver(MSG_TONGQUE_REFRESH,
                              this,
                              callfuncO_selector(SGTongQueLayer::tongQueRefreshListener));
    notification->addObserver(MSG_TONGQUE_YANGXIN,
                              this,
                              callfuncO_selector(SGTongQueLayer::tongQueYangXinListener));
    

    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    wid = (int)CCDirector::sharedDirector()->getWinSize().width;

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
    
    float setWidRate = s.width/640;
    float setHightRate = s.height/960;
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(title_bg);
    
    SGCCLabelTTF *sprite = SGCCLabelTTF::create(str_TongQueLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//sprite->setColor(ccc3(0xff, 0x95, 0x0c));
    sprite->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,-title_bg->getContentSize().height/2)) );
    this->addChild(sprite);
  
    CCSprite *heitiao = CCSprite::createWithSpriteFrameName("visit_heitiao.png");
    if (!heitiao) {
        return;
    }
    this->addChild(heitiao);
    heitiao->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width*.5f - heitiao->getContentSize().width/2, -title_bg->getContentSize().height -heitiao->getContentSize().height*.5f)));
    
    CCSprite *bg = NULL;
    if (s.height > 1024) {
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tongQueBg.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);

        bg =  CCSprite::createWithSpriteFrameName("tongQueBg.png");
        bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt-60)));
    }
    else
    {
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tongQueBg.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);

        CCRect resetRect = CCRectMake(0, btmhgt*0.8, 736, heitiao->getPosition().y - btmhgt*0.8);
        bg =  CCSprite::createWithSpriteFrameName("tongQueBg.png");
        bg->cocos2d::CCNode::setScale(resetRect.size.width / bg->getContentSize().width, resetRect.size.height / bg->getContentSize().height);
        bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt*0.66)));
    }
    bg->setScaleX(setWidRate);
    bg->setAnchorPoint(ccp(0.5, 0));
    this->addChild(bg,-1);
    CCSize bigBgSize = bg->getContentSize();
    
    CCSprite *tongCoin = CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(tongCoin,10);
    tongCoin->setPosition(ccp(wid/2- tongCoin->getContentSize().width*0.2, heitiao->getPosition().y));
    
    havejg = SGCCLabelTTF::create(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString(), FONT_BOXINFO, 28);
    addChild(havejg,10);
    havejg->setAnchorPoint(ccp(0, .5f));
    havejg->setPosition(ccpAdd(tongCoin->getPosition(), ccp(tongCoin->getContentSize().width/2+5, 3.5)));
    
    CCSprite *yb = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yb,10);
    yb->setPosition(ccpAdd(tongCoin->getPosition(), ccp(-wid*0.16 - yb->getContentSize().width ,2)));
    
    havegold = SGCCLabelTTF::create(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString(), FONT_BOXINFO, 28);
    addChild(havegold,10);
    havegold->setAnchorPoint(ccp(0, .5f));
    havegold->setPosition(ccpAdd(yb->getPosition(), ccp(yb->getContentSize().width/2+5, 0)));
    
    CCSprite *tongMedal = CCSprite::createWithSpriteFrameName("tongqueXunLittle.png");
    this->addChild(tongMedal,10);
    tongMedal->setPosition(ccpAdd(tongCoin->getPosition(), ccp(wid*0.2 + tongMedal->getContentSize().width , 0)));
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("4");
     nXunZhangMax=dic1->valueForKey("value")->intValue();
    

    yongqiCount = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",m_medal,nXunZhangMax)->getCString(), FONT_BOXINFO, 28);
    addChild(yongqiCount,1);
    yongqiCount->setPosition( ccpAdd(tongMedal->getPosition(), ccp(tongMedal->getContentSize().width/2+yongqiCount->getContentSize().width/2+5,3.5)) );
    
    SGCCLabelTTF *font1 = SGCCLabelTTF::create(str_TongQueLayer_str2, FONT_BOXINFO, 28);
    addChild(font1,11);
    font1->setPosition(ccpAdd(yb->getPosition(), ccp(-font1->getContentSize().width*0.8, 0)));
    
    
    ///////////////////////////////////////
    //back
    SGButton *backBtn = SGButton::create("tongQueBackBt.png", NULL, this, menu_selector(SGTongQueLayer::backHandler));
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(s.width - backBtn->getContentSize().width,heitiao->getPosition().y - 100*setHightRate));
    
    //夺宝按钮
    tognqueDokbBt = SGButton::create("tongqueDobk.png", NULL, this, menu_selector(SGTongQueLayer::tongQueDobkSend),CCPointZero,false,true);
    tognqueDokbBt->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -tognqueDokbBt->getContentSize().height*0.86)));
    addBtn(tognqueDokbBt);
    
    if (m_spendMedal > 0) {
        m_dokbFlag = CCSprite::createWithSpriteFrameName("tongqueXunLittle.png");
        dokbCostLabel = SGCCLabelTTF::create(CCString::createWithFormat("x %d",m_spendMedal)->getCString(), FONT_BOXINFO, 30);
    }
    else if (m_spendGold > 0)
    {
        m_dokbFlag = CCSprite::createWithSpriteFrameName("store_yb.png");
        dokbCostLabel = SGCCLabelTTF::create(CCString::createWithFormat("x %d",m_spendGold)->getCString(), FONT_BOXINFO, 30);
    }
    else
    {
        m_dokbFlag = CCSprite::createWithSpriteFrameName("tongqueXunLittle.png");
        dokbCostLabel = SGCCLabelTTF::create(CCString::createWithFormat("x %d",500)->getCString(), FONT_BOXINFO, 30);
    }
    addChild(m_dokbFlag,100);
    m_dokbFlag->setPosition(ccpAdd(tognqueDokbBt->getPosition(),ccp(-m_dokbFlag->getContentSize().width * 0.9, -m_dokbFlag->getContentSize().height/2-8)));

    addChild(dokbCostLabel,100);
    dokbCostLabel->setPosition(ccpAdd(tognqueDokbBt->getPosition(),ccp(m_dokbFlag->getContentSize().width/2, -m_dokbFlag->getContentSize().height/2-8)));
    
    ////////////////
    
    SGCCLabelTTF *refeshTime = SGCCLabelTTF::create(str_TongQueLayer_str3, FONT_PANGWA, 28 ,ccc3(0xf2,0x6a,0x0e));
    addChild(refeshTime);
    //modify by:zyc. merge into create.
    //refeshTime->setColor(ccc3(0xf2,0x6a,0x0e));
    refeshTime->setPosition(ccp((bigBgSize.width * 0.3 * setWidRate - 60 )*setWidRate,tognqueDokbBt->getPosition().y - tognqueDokbBt->getContentSize().height/2*setHightRate));
    
    refeshTimeCount = SGCCLabelTTF::create("01 : 00 : 00 ", FONT_PANGWA, 28);
    addChild(refeshTimeCount);
    refeshTimeCount->setPosition(ccpAdd(refeshTime->getPosition(), ccp(refeshTime->getContentSize().width/2 + refeshTimeCount->getContentSize().width/2 + 10,0)));
    
    refreshBtn = SGButton::createFromLocal("tongqueRefresh.png", " ", this, menu_selector(SGTongQueLayer::tongQueRefreshSend),ccp(0,3),FONT_PANGWA,ccWHITE,32,false,true);
    refreshBtn->setPosition(ccpAdd(refeshTimeCount->getPosition(), ccp(refeshTimeCount->getContentSize().width/2 + refreshBtn->getContentSize().width/2 +10,0)));
    addBtn(refreshBtn);
    
    refeshCost = SGCCLabelTTF::create(str_TongQueLayer_str4, FONT_BOXINFO, 34);
    addChild(refeshCost,100);
    refeshCost->setPosition(ccpAdd(refreshBtn->getPosition(), ccp(refeshCost->getContentSize().width/2 - refreshBtn->getContentSize().width/2 +4,0)));
    
    refeshFlag = CCSprite::createWithSpriteFrameName("tongqueCoin.png");
    addChild(refeshFlag,100);
    refeshFlag->setPosition(ccpAdd(refreshBtn->getPosition(), ccp(0,-refeshFlag->getContentSize().height/2 +4)));
    
    refeshCostCount = SGCCLabelTTF::create(CCString::createWithFormat("%d",m_spendCoins)->getCString(), FONT_BOXINFO, 24);
    addChild(refeshCostCount,100);
    refeshCostCount->setPosition(ccpAdd(refeshFlag->getPosition(), ccp(refeshFlag->getContentSize().width/2 + refeshCostCount->getContentSize().width/2,0)));
    
    if (m_spendCoins <= 0) {
        refeshCost->setString(str_TongQueLayer_str5);
        refeshCostCount->setVisible(false);
        refeshFlag->setVisible(false);
        refeshCost->setPosition(ccpAdd(refreshBtn->getPosition(), ccp(refeshCost->getContentSize().width/2 - refreshBtn->getContentSize().width/2 +4,0)));
    }

    SGCCLabelTTF *dobkDetail1 = SGCCLabelTTF::create(str_TongQueLayer_str6, FONT_BOXINFO, 22);
    addChild(dobkDetail1);
    dobkDetail1->setPosition(ccp(s.width/2,refeshTime->getPosition().y - 40*setHightRate));
    
    SGCCLabelTTF *dobkDetail2 = SGCCLabelTTF::create(str_TongQueLayer_str7, FONT_BOXINFO, 22);
    addChild(dobkDetail2);
    dobkDetail2->setPosition(ccp(s.width/2 - dobkDetail1->getContentSize().width/2 + dobkDetail2->getContentSize().width/2,dobkDetail1->getPosition().y - 30*setHightRate));
    
    
    
    //翻牌物品
    itemsPos = ccp(s.width/2, heitiao->getPosition().y - 220*setHightRate);
    createTongQueCard(itemsPos);
    
    //移动光标
    guangBioa = SGButton::create("equipPinzhi_5.png", NULL, NULL, NULL);
    guangBioa->setEnabled(false);
    guangBioa->setZOrder(100);
    addBtn(guangBioa);
    if (getBtnByTag(TONGQUEITEMSTARTAG)) {
        guangBioa->setPosition(getBtnByTag(TONGQUEITEMSTARTAG)->getPosition());
    }
    else
    {
        guangBioa->setPosition(tognqueDokbBt->getPosition());
    }
    
    guangBioa->setVisible(false);
    guangBioaGuan = CCSprite::createWithSpriteFrameName("firstChargeGuang.png");
    if (guangBioaGuan) {
        guangBioaGuan->setAnchorPoint(ccp(0.5,0.5));
        guangBioaGuan->ignoreAnchorPointForPosition(false);
        guangBioaGuan->setScale(1.4);
        guangBioaGuan->setPosition(guangBioa->getPosition());
        guangBioaGuan->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
        guangBioaGuan->setVisible(false);
        addChild(guangBioaGuan);
        guangBioaGuan->pauseSchedulerAndActions();
    }
    
    
    //进度条和进度条背景
    CCSprite* scalebg =  CCSprite::createWithSpriteFrameName("tongqueGrossBg.png");
	CCSprite *progressSprite = CCSprite::createWithSpriteFrameName("tongqueGrass.png");
    updateProgress = CCProgressTimer::create(progressSprite);
	updateProgress->setType(kCCProgressTimerTypeBar);
	updateProgress->setMidpoint(ccp(0, 0));
	updateProgress->setBarChangeRate(ccp(1, 0));
	
    scalebg->setAnchorPoint(ccp(0, 0.5));
    updateProgress->setAnchorPoint(ccp(0, 0.5));

    scalebg->setPosition(ccp(s.width/2 - scalebg->getContentSize().width/2,dobkDetail2->getPosition().y/2 + btmhgt/2 - 30 * setHightRate));
    updateProgress->setPosition(ccpAdd(scalebg->getPosition(), ccp(scalebg->getContentSize().width/2-updateProgress->getContentSize().width/2-12, 0)) );
    addChild(scalebg,1);
    addChild(updateProgress);
    updateProgress->setPercentage(m_blessValue);
    
    //养心殿
    yangxindian = NULL;
    if (m_blessValue < 100) {
        yangxindian = SGButton::create("yangxin1.png", NULL, this, menu_selector(SGTongQueLayer::showYangXinItem),CCPointZero,false,true);
        yangxindian->setPosition(ccpAdd(updateProgress->getPosition(), ccp(updateProgress->getContentSize().width + 20, 0)));
        yangxinLabel = SGCCLabelTTF::create(str_TongQueLayer_str8, FONT_BOXINFO, 20);
    }
    else
    {
        yangxinLabel = SGCCLabelTTF::create(str_TongQueLayer_str9, FONT_BOXINFO, 20);
        yangxindian = SGButton::create("yangxin2.png", NULL, this, menu_selector(SGTongQueLayer::showYangXinItem),CCPointZero,false,true);
        yangxindian->setPosition(ccpAdd(updateProgress->getPosition(), ccp(updateProgress->getContentSize().width + 20, yangxindian->getContentSize().height*0.2)));
    }
    
    if (yangxindian) {
        yangxindian->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(4),
                                                                          CCRotateBy::create(0.1, -10),
                                                                          CCRotateBy::create(0.14, 18),
                                                                          CCRotateBy::create(0.1, -14),
                                                                          CCRotateBy::create(0.14, 10),
                                                                          CCRotateBy::create(0.1, -4),  //和为0
                                                                          NULL)));
        yangxinRotation();
    }

    addBtn(yangxindian);
    yangxinLabel->setPosition(ccpAdd(yangxindian->getPosition(), ccp(0, -yangxindian->getContentSize().height/2- yangxinLabel->getContentSize().height/2)));
    addChild(yangxinLabel);
    
    rewardEffect = CCSprite::createWithSpriteFrameName("yangxinGuang.png");
    if (rewardEffect) {
        rewardEffect->setAnchorPoint(ccp(0.5,0.5));
        rewardEffect->ignoreAnchorPointForPosition(false);
        rewardEffect->setPosition(ccp(yangxindian->getContentSize().width*0.44,yangxindian->getContentSize().height*0.34));
        rewardEffect->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
        yangxindian->addChild(rewardEffect,-10,-10);
        if (m_blessValue < 100)
        {
            rewardEffect->setVisible(false);
           
        }
    }
    
    ////////////////////////
    schedule(schedule_selector(SGTongQueLayer::startRefreshTimeHaHa), 1);
    dobkTimeLimit = true;
}
void SGTongQueLayer::showDobkCostInfo()
{
    if (m_dokbFlag && dokbCostLabel) {
        m_dokbFlag->setVisible(canDobk);
        dokbCostLabel->setVisible(canDobk);
    }
}


void SGTongQueLayer::selectTongQueCard(CCObject* obj)
{
    if (obj == NULL) {
        return;
    }
    int tag = ((CCNode*)obj)->getTag() - TONGQUEITEMSTARTAG;
    if (tag < m_tongqueItem.size()) {
        showtongqueItemDetail(m_tongqueItem[tag]);
    }
}
void SGTongQueLayer::showYangXinItem(CCObject* obj)
{
    if (!yangxindian->isEnabled()) {
        return;
    }
    if (updateProgress->getPercentage() >= 100) {
        SGYangXinBox *yangXinBox = SGYangXinBox::create(this,m_yangxingItem,true);
        SGMainManager::shareMain()->showBox(yangXinBox);
    }
    else
    {
        SGYangXinBox *yangXinBox = SGYangXinBox::create(this,m_yangxingItem,false);
        SGMainManager::shareMain()->showBox(yangXinBox);
    }
    yangxindian->setEnabled(false);
}

void SGTongQueLayer::showBoxCall(CCObject *obj)
{
    yangxindian->setEnabled(true);
    tognqueDokbBt->setEnabled(true);
}

void SGTongQueLayer::setDobkLimit(float dt)
{
   unschedule(schedule_selector(SGTongQueLayer::setDobkLimit));
   dobkTimeLimit = true;
}


void SGTongQueLayer::tongQueDobkSend(CCObject* obj)
{
    if (!dobkTimeLimit)
    {
        return;
    }
    
    dobkTimeLimit = false;
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    if (updateProgress->getPercentage() >= 100) {
        SGMainManager::shareMain()->showMessage(str_TongQueLayer_str10);
        
        showYangXinItem(NULL);
        tognqueDokbBt->setEnabled(false);
        schedule(schedule_selector(SGTongQueLayer::setDobkLimit),1);
        return;
    }

    if (!canDobk)
    {
        SGMainManager::shareMain()->showMessage(str_TongQueLayer_str11);
        schedule(schedule_selector(SGTongQueLayer::setDobkLimit),1);
    }
    else
    {
        if ( (m_spendMedal != 0 && m_medal >= m_spendMedal) ||
              (m_spendGold != 0 && playerInfo->getPlayerGold() >= m_spendGold)) {
            tognqueDokbBt->setEnabled(false);
            main::LotteryRequest* req = new main::LotteryRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_TONGQUE_DOBK, req);
            
            if (gotItem)
            {
                CCSprite *haveGot = CCSprite::createWithSpriteFrameName("tongQueItemBg.png");
                gotItem->addChild(haveGot,9);
                haveGot->setScale(1.3*gotItem->getScale());
                haveGot->setPosition(ccp(gotItem->getContentSize().width/2 , gotItem->getContentSize().height/2));
                ((SGButton*)gotItem)->setEnabled(false);
            }
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_TongQueLayer_str12);
            schedule(schedule_selector(SGTongQueLayer::setDobkLimit),1);
        }
    }
}

void SGTongQueLayer::tongQueDobkListener(CCObject* obj)
{
    SGSocketRequest *request = (SGSocketRequest *)obj;
    main::LotteryResponse *response = (main::LotteryResponse *)request->m_msg;
    if (response)
    {
        if (response->state() == 1 || response->state() == 2) {
            
            stopPosId = response->id();
            
            m_medal = response->medal();

            yongqiCount->setString(CCString::createWithFormat("%d/%d",m_medal,nXunZhangMax)->getCString());
            m_blessValue = response->blessvalue();
 
            /////////刷新夺宝消耗
            m_spendMedal = response->spendmedal();
            m_spendGold = response->spendgold();
            
            if (m_spendMedal > 0) {
                m_dokbFlag->setDisplayFrame(CCSprite::createWithSpriteFrameName("tongqueXunLittle.png")->displayFrame());
                dokbCostLabel->setString(CCString::createWithFormat("x %d",m_spendMedal)->getCString());
            }
            else if (m_spendGold > 0)
            {
                m_dokbFlag->setDisplayFrame(CCSprite::createWithSpriteFrameName("store_yb.png")->displayFrame());
                dokbCostLabel->setString(CCString::createWithFormat("x %d",m_spendGold)->getCString());
            }
            else
            {
                m_dokbFlag->setDisplayFrame(CCSprite::createWithSpriteFrameName("tongqueXunLittle.png")->displayFrame());
            }
            
             /////////刷新夺宝消耗
        
            
            guangBioa->setVisible(true);
            guangBioaGuan->setVisible(false);

            unschedule(schedule_selector(SGTongQueLayer::startMoveHaHa));
            schedule(schedule_selector(SGTongQueLayer::startMoveHaHa), 0.06);
            EFFECT_PLAY(MUSIC_68);
            
            if (stopPosId < m_tongqueItem.size()) {
                m_tongqueItem[stopPosId].setItemBtId(1);
            }
            bool isLastOne = true;
            for (int i = 0 ; i< m_tongqueItem.size(); i++) {
                if (m_tongqueItem[i].getItemBtId() != 1) {
                    isLastOne = false;
                    break;
                }
            }
            if (isLastOne) {
                 moveHaHaTime = 1.3;
            }
            else
            {
                moveHaHaTime = 0;
            }

            refreshGold();
            
            if (response->state() == 2) {
                canDobk = false;
                tognqueDokbBt->init("tongqueDobkDis.png", NULL, this, menu_selector(SGTongQueLayer::tongQueDobkSend),CCPointZero,false,true);
            }
            else
            {
                canDobk = true;
            }
            
            showDobkCostInfo();
            schedule(schedule_selector(SGTongQueLayer::setDobkLimit),1);
            return;
        }
    }
    schedule(schedule_selector(SGTongQueLayer::setDobkLimit),1);
    tognqueDokbBt->setEnabled(true);
}

void SGTongQueLayer::tongQueRefreshSend(CCObject* obj)
{
    if (obj == NULL) {
        //自动刷新
        main::TongQueRefreshRequest* req = new main::TongQueRefreshRequest();
        req->set_signal(AUTOREFRESH);
        SGSocketClient::sharedSocketClient()->send(MSG_TONGQUE_REFRESH, req);
        
        gotItem = getBtnByTag(stopPosId + TONGQUEITEMSTARTAG);
        if (gotItem) {
            CCSprite * haveGot = CCSprite::createWithSpriteFrameName("tongqueGet.png");
            gotItem->addChild(haveGot,10);
            haveGot->setPosition(ccp(gotItem->getContentSize().width - haveGot->getContentSize().width*0.2, haveGot->getContentSize().height*0.1));
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_TongQueLayer_str13,
                                                                               ((SGCCLabelTTF*)gotItem->getChildByTag(ITEMNAMETAG))->getString())->getCString());
        }
    }
    else
    {
        if (guangBioa->isVisible()) {
            //正在跑马，不刷新
            SGMainManager::shareMain()->showMessage(str_TongQueLayer_str14);
            return;
        }
        
        //手动刷新
        if (SGPlayerInfo::sharePlayerInfo()->getPlayerCoins() >= m_spendCoins) {
            main::TongQueRefreshRequest* req = new main::TongQueRefreshRequest();
            req->set_signal(SHOUDONGEFRESH);
            SGSocketClient::sharedSocketClient()->send(MSG_TONGQUE_REFRESH, req);
        }
        else
        {
            //刷新钱不够
            SGMainManager::shareMain()->showMessage(str_TongQueLayer_str15);
        }
    }

}

void SGTongQueLayer::tongQueRefreshListener(CCObject* obj)
{
    SGSocketRequest *request = (SGSocketRequest *)obj;
    main::TongQueRefreshResponse *response = (main::TongQueRefreshResponse *)request->m_msg;
    
    if (response)
    {
        /////////刷新夺宝消耗
        m_spendCoins = response->spendcoins();
        if (m_spendCoins > 0) {
            refeshCost->setString(str_TongQueLayer_str4);
            refeshFlag->setVisible(true);
            refeshCostCount->setString(CCString::createWithFormat("%d",m_spendCoins)->getCString());
            refeshCostCount->setPosition(ccpAdd(refeshFlag->getPosition(), ccp(refeshFlag->getContentSize().width/2 + refeshCostCount->getContentSize().width/2,0)));
            refeshCostCount->setVisible(true);
        }
        else
        {
            //int wid = refeshCost->getContentSize().width/2;
            refeshCost->setString(str_TongQueLayer_str5);
            refeshCostCount->setVisible(false);
            refeshFlag->setVisible(false);
            refeshCost->setPosition(ccpAdd(refreshBtn->getPosition(), ccp(refeshCost->getContentSize().width/2 - refreshBtn->getContentSize().width/2 +4,0)));
        }
        
        m_spendMedal = response->spendmedal();
        m_spendGold = response->spendgold();
        if (m_spendMedal > 0) {
            m_dokbFlag->setDisplayFrame(CCSprite::createWithSpriteFrameName("tongqueXunLittle.png")->displayFrame());
             dokbCostLabel->setString(CCString::createWithFormat("x %d",m_spendMedal)->getCString());
        }
        else if (m_spendGold > 0)
        {
            m_dokbFlag->setDisplayFrame(CCSprite::createWithSpriteFrameName("store_yb.png")->displayFrame());
             dokbCostLabel->setString(CCString::createWithFormat("x %d",m_spendGold)->getCString());
        }
        else
        {
            m_dokbFlag->setDisplayFrame(CCSprite::createWithSpriteFrameName("tongqueXunLittle.png")->displayFrame());
        }
       
        /////////刷新夺宝消耗
        
        ////刷新夺宝item
        int itemLen = response->itemlist_size();
        m_tongqueItem.clear();
        for (int it = 0; it<itemLen; it++) {
            
            SGItemInfo itemInfo(response->itemlist(it).itemtype(),
                                response->itemlist(it).itemid(),
                                response->itemlist(it).itemcount(),
                                response->itemlist(it).itemlevel(),
                                response->itemlist(it).itembtnid());
            m_tongqueItem.push_back(itemInfo);
        }

        createTongQueCard(itemsPos);
        ////刷新夺宝item
        
        tognqueDokbBt->setEnabled(true);
        startPosId = 0;
        stopPosId = 10;
        guangBioa->setVisible(false);
        guangBioaGuan->setVisible(false);
        
        refreshTimeCount = 60 * 60;
        unschedule(schedule_selector(SGTongQueLayer::startRefreshTimeHaHa));
        schedule(schedule_selector(SGTongQueLayer::startRefreshTimeHaHa), 1);
        refreshGold();
        
        SGMainManager::shareMain()->showMessage(str_TongQueLayer_str16);
    }
}
void SGTongQueLayer::tongQueYangXinSend()
{
    main::TongQueYangXinRequest* req = new main::TongQueYangXinRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_TONGQUE_YANGXIN, req);
}

void SGTongQueLayer::tongQueYangXinListener(CCObject* obj)
{
    SGSocketRequest *request = (SGSocketRequest *)obj;
    main::TongQueYangXinResponse *response = (main::TongQueYangXinResponse *)request->m_msg;
    
    if (response && response->state() == GETYANGXIN)
    {
        m_yangxingItem.setItemId(response->rewardinfo().itemid());
        m_yangxingItem.setItemType(response->rewardinfo().itemtype());
        m_yangxingItem.setItemLevel(response->rewardinfo().itemlevel());
        m_yangxingItem.setItemCount(response->rewardinfo().itemcount());
        m_yangxingItem.setItemBtId(response->rewardinfo().itembtnid());

        updateProgress->setPercentage(0);
        m_blessValue = 0;
        
        yangxindian->init("yangxin1.png", NULL, this, menu_selector(SGTongQueLayer::showYangXinItem),CCPointZero,false,true);
        yangxindian->setPosition(ccpAdd(updateProgress->getPosition(), ccp(updateProgress->getContentSize().width + 20, 0)));
        
        yangxinLabel->setString(str_TongQueLayer_str8);
        yangxinLabel->setPosition(ccpAdd(yangxindian->getPosition(), ccp(0, -yangxindian->getContentSize().height/2 - yangxinLabel->getContentSize().height/2)));
        
        rewardEffect->setPosition(ccp(yangxindian->getContentSize().width*0.44,yangxindian->getContentSize().height*0.34));
        rewardEffect->setVisible(false);
        yangxinRotation();
        
    }
}

void SGTongQueLayer::startMoveHaHa(float dt)
{
    moveHaHaTime += dt;
    if (moveHaHaTime < 2) {
        if (startPosId >= movePosList.size()) {
            startPosId = 0;
        }
        
        guangBioa->setPosition(movePosList[startPosId]);
        
        if (moveHaHaTime > 1.2) {
            if (startPosId == stopPosId) {
                unschedule(schedule_selector(SGTongQueLayer::startMoveHaHa));
                dealGetDobkItem();
                guangBioa->setVisible(false);
                return;
            }
        }
        
        startPosId++;
        
    }
    else
    {
        guangBioa->setVisible(false);
        unschedule(schedule_selector(SGTongQueLayer::startMoveHaHa));
    }

}

void SGTongQueLayer::dealGetDobkItem()
{
    tognqueDokbBt->setEnabled(true);
    guangBioaGuan->resumeSchedulerAndActions();
    guangBioaGuan->setVisible(true);
    guangBioaGuan->setPosition(guangBioa->getPosition());
    
    gotItem = getBtnByTag(stopPosId + TONGQUEITEMSTARTAG);
    if (gotItem) {
        CCSprite * haveGot = CCSprite::createWithSpriteFrameName("tongqueGet.png");
        gotItem->addChild(haveGot,10);
        haveGot->setPosition(ccp(gotItem->getContentSize().width - haveGot->getContentSize().width*0.2, haveGot->getContentSize().height*0.1));
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_TongQueLayer_str13,
                                                       ((SGCCLabelTTF*)gotItem->getChildByTag(ITEMNAMETAG))->getString())->getCString());
    }
    
    stopPosId = 10;
    
    CCSpriterX *newRewardEffect = CCSpriterX::create("animationFile/G_Starlight.scml", true, true);
    newRewardEffect->setanimaID(0);
    newRewardEffect->setisloop(true);
    newRewardEffect->setPosition(tognqueDokbBt->getPosition());
    newRewardEffect->setScale(0.95);
    this->addChild(newRewardEffect,100,ZHUFUEFFECTAG);
    newRewardEffect->play();
    newRewardEffect->runAction(CCSequence::create(CCMoveTo::create(1, updateProgress->getPosition()),
                                                  CCCallFunc::create(this, callfunc_selector(SGTongQueLayer::refreshTongQuePercetage)),NULL));
    
}

void SGTongQueLayer::startRefreshTimeHaHa(float dt)
{
    int useTime = (int)dt;
    refreshTimeCount -= useTime;
    if (refreshTimeCount > 0) {
        int hours = refreshTimeCount / 60 /60;
        int minutes = refreshTimeCount/60 - hours * 60 ;
        int secs = refreshTimeCount - hours*60 *60 - minutes*60;
 
        char timeStr[32];
        memset(timeStr, 0, 32);
        snprintf(timeStr, 32, "%02d : %02d : %02d", hours, minutes, secs);
        refeshTimeCount->setString(timeStr);
    }
    else
    {
        unschedule(schedule_selector(SGTongQueLayer::startRefreshTimeHaHa));
        tongQueRefreshSend(NULL);
    }
}

void SGTongQueLayer::refreshTongQuePercetage()
{
    removeChildByTag(ZHUFUEFFECTAG);
    
    updateProgress->runAction(CCSequence::create(CCProgressTo::create(0.4, m_blessValue),
                                                   CCCallFunc::create(this, callfunc_selector(SGTongQueLayer::setYangXinStatus)),NULL));
    yangxindian->setEnabled(false);
}

void SGTongQueLayer::setYangXinStatus()
{
    yangxindian->setEnabled(true);
    if (m_blessValue >= 100) {
        EFFECT_PLAY(MUSIC_69);
        
        yangxindian->init("yangxin2.png", NULL, this, menu_selector(SGTongQueLayer::showYangXinItem),CCPointZero,false,true);
        yangxindian->setPosition(ccpAdd(updateProgress->getPosition(), ccp(updateProgress->getContentSize().width + 20, yangxindian->getContentSize().height*0.2)));
        yangxinLabel->setString(str_TongQueLayer_str9);
        yangxinLabel->setPosition(ccpAdd(yangxindian->getPosition(), ccp(0, -yangxindian->getContentSize().height/2- yangxinLabel->getContentSize().height/2)));
        
        rewardEffect->setPosition(ccp(yangxindian->getContentSize().width*0.44,yangxindian->getContentSize().height*0.34));
        rewardEffect->setVisible(true);
        SGMainManager::shareMain()->showMessage(str_TongQueLayer_str17);
        yangxinRotation();
    }
}


void SGTongQueLayer::createTongQueCard(CCPoint tongQuePos)
{
    SGButton *visitCardbg = NULL;
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    float widDis = s.width*0.1;
    float heightDis = s.height*0.12;
    CCPoint cardPos;
    movePosList.clear();
    gotItem = NULL;
    //item
    int elementNum = m_tongqueItem.size();
    SGCCLabelTTF *itemName = NULL;
    SGCCLabelTTF * itemNumLab = NULL;
    CCString * nameStr = NULL;
    CCString * numStr = NULL;
    SGMainManager * sgMainManager = SGMainManager::shareMain();
    SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
    canDobk = false;

    for (int k=0; k<elementNum; k++)
    {
        visitCardbg = NULL;
        switch (m_tongqueItem[k].getItemType())
        {
                case 0:
                {
                    visitCardbg = SGButton::create("big_yb.png", "equipPinzhi_4_0.png", this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    nameStr = CCString::createWithFormat(str_Format_yuanbao__);
                }
                    break;
                case 1:
                {
                    sgMainManager->addHeadIconPlistByNum(9999,sg_tongQueLayer);
                    visitCardbg = SGButton::create("head9999.png", "equipPinzhi_3_0.png", this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    nameStr = CCString::createWithFormat(str_Format_tongMoney__);
                }
                    break;
                case 2:
                {
                    sgMainManager->addHeadIconPlistByNum(9999,sg_tongQueLayer);
                    visitCardbg = SGButton::create("head9999.png", "equipPinzhi_3_0.png", this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    nameStr = CCString::createWithFormat(str_Format_pvpScore__);
                }
                    break;
                case 3://废弃
                {
                    sgMainManager->addHeadIconPlistByNum(9997,sg_tongQueLayer);
                    visitCardbg = SGButton::create("head9997.png", "equipPinzhi_3_0.png", this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    nameStr = CCString::createWithFormat(str_Format_jungong__);
                    numStr = CCString::createWithFormat("x%d" , m_tongqueItem[k].getItemCount());
                }
                    break;
                case 4://废弃
                {
                    sgMainManager->addHeadIconPlistByNum(9998,sg_tongQueLayer);
                    visitCardbg = SGButton::create("head9998.png", "equipPinzhi_3_0.png", this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    nameStr = CCString::createWithFormat(str_Format_smalll_laba,m_tongqueItem[k].getItemCount());
                }
                    break;
                case 5:
                {
                    sgMainManager->addHeadIconPlistByNum(9998,sg_tongQueLayer);
                    visitCardbg = SGButton::create("head9998.png", "equipPinzhi_3_0.png", this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    nameStr = CCString::createWithFormat(str_Format_liandian__);
                }
                    break;
                case 6:
                {
                    SGOfficerDataModel *temp = staticDataManager->getOfficerById(m_tongqueItem[k].getItemId());
                    if (temp->getIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_tongQueLayer);
                    int starlvl = temp->getOfficerCurrStarLevel();
                    if (starlvl>1) {
                        starlvl-=1;
                    }
                    visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),
                                                   CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString(),
                                                   this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setPosition(ccp(visitCardbg->getContentSize().width*0.28,visitCardbg->getContentSize().height - countryName->getContentSize().height*0.1));
                    visitCardbg->addChild(countryName,30,30);
                    
                    if (temp->getAdvNum() > 0) {
                        SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                        advNum->setInsideColor(ccGREEN);
                        advNum->ignoreAnchorPointForPosition(false);
                        advNum->setPosition(ccp(visitCardbg->getContentSize().width*0.8,visitCardbg->getContentSize().height - advNum->getContentSize().height/2));
                        visitCardbg->addChild(advNum,40,40);
                    }
                    
                    nameStr = CCString::createWithFormat("%s",temp->getOfficerName()->getCString());
                    
                }
                    break;
                case 7:
                {
                    SGEquipmentDataModel *temp = staticDataManager->getEquipById(m_tongqueItem[k].getItemId());
                    if (temp->getIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_tongQueLayer);
                    visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),
                                                   CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString(),
                                                   this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    
                    if (temp->getAdvNum() > 0) {
                        SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                        advNum->setInsideColor(ccGREEN);
                        advNum->setAnchorPoint(ccp(1,1));
                        advNum->ignoreAnchorPointForPosition(false);
                        advNum->setPosition(ccp(visitCardbg->getContentSize().width*0.8,visitCardbg->getContentSize().height - advNum->getContentSize().height));
                        visitCardbg->addChild(advNum,40,40);
                    }
                    
                    nameStr = CCString::createWithFormat("%s",temp->getEquipName()->getCString());
                    
                }
                    break;
                case 9:
                {
                    SGPropsDataModel *temp = staticDataManager->getPropsById(m_tongqueItem[k].getItemId());
                    if (temp->getIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_tongQueLayer);
                    visitCardbg = SGButton::create(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getPropsCurrStarLevel()-1)->getCString(),
                                                   CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),
                                                   this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    
                     nameStr = CCString::createWithFormat("%s",temp->getPropsName()->getCString());
                }
                    break;
                case 10:
                {
                    SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(m_tongqueItem[k].getItemId());
                    int starlvl = pieces->getStarLevel();
                    if (starlvl > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
                    {
                        starlvl -= 1;
                    }
                    CCString *str_kuang = NULL;
                    if (pieces->getCallingCardId() == -1)//如果是通用碎片
                    {
                        str_kuang = CCString::createWithFormat("common_pieces_border.png");
                    }
                    else if (pieces->getPiecesType() == 1)//装备碎片
                    {
                        str_kuang  = CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl);
                    }
                    else if (pieces->getPiecesType() == 0)//武将碎片
                    {
                        str_kuang  = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
                    }
                    
                    
                    int iconId = 0;
                    if (pieces->getPiecesType() == 0)//武将
                    {
                        SGOfficerDataModel *of = staticDataManager->getOfficerById(pieces->getCallingCardId());
                        iconId = of->getIconId();
                    }
                    else if (pieces->getPiecesType() == 1)//装备
                    {
                        SGEquipmentDataModel *eq = staticDataManager->getEquipById(pieces->getCallingCardId());
                        iconId = eq->getIconId();
                    }
                    if (iconId < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(iconId,sg_tongQueLayer);
                    
                    visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",iconId)->getCString(),str_kuang->getCString(),
                                                    this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
                    {
						ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_tongQueLayer);
                        CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                        mask->setPosition(ccp(visitCardbg->getContentSize().width / 2, visitCardbg->getContentSize().height / 2));
                        visitCardbg->addChild(mask);
                    }
           
                    nameStr = CCString::createWithFormat("%s",pieces->getPiecesName()->getCString());
                }
                    break;
                case 15:
                {
                    SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(m_tongqueItem[k].getItemId());
                    if (consume == NULL || consume->getIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(consume->getIconId(), sg_tongQueLayer);
                    int starLvl = consume->getConsumeStarLvl();
                    if (starLvl > 1)
                        starLvl -= 1;
                    visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png", consume->getIconId())->getCString(),
                                                   CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString(),
                                                   this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    
                    nameStr = CCString::createWithFormat("%s",consume->getConsumeName()->getCString());
                }
                    break;
                case 16:
                {
                    visitCardbg = SGButton::create("tongqueXunBig.png", "equipPinzhi_3_0.png", NULL, NULL,CCPointZero,false,true);
                    nameStr = CCString::create(str_Currency_XunZhang);
                }
                    break;
                case 17://材料
                {
                    
                  
                    
                    SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(m_tongqueItem[k].getItemId());
                    if (materialModel == NULL || materialModel->getMaterialIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(materialModel->getMaterialIconId(), sg_tongQueLayer);
                    int starLvl = materialModel->getMaterialBorderColor();
                    if (starLvl > 1)
                        starLvl -= 1;
                    visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png", materialModel->getMaterialIconId())->getCString(),
                                                   CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString(),
                                                   this, menu_selector(SGTongQueLayer::selectTongQueCard),CCPointZero,false,true);
                    
                    nameStr = CCString::createWithFormat("%s",materialModel->getMaterialName()->getCString());
                }
                    break;
                default:
                    break;
                    
            }
            numStr = CCString::createWithFormat("x%d" , m_tongqueItem[k].getItemCount());
            if (visitCardbg)
            {
                //pos
                switch (k)
                {
                    case 0:
                    {
                        cardPos =  ccpAdd(tongQuePos, ccp(-widDis , heightDis));
                    }
                        break;
                    case 1:
                    {
                        cardPos =  ccpAdd(tongQuePos, ccp(widDis , heightDis));
                    }
                        break;
                    case 2:
                    {
                        cardPos =  ccpAdd(tongQuePos, ccp(widDis * 2.2, 0));
                    }
                        break;
                    case 3:
                    {
                        cardPos =  ccpAdd(tongQuePos, ccp(widDis, -heightDis));
                    }
                        break;
                    case 4:
                    {
                        cardPos =  ccpAdd(tongQuePos, ccp(-widDis, -heightDis));
                    }
                        break;
                    case 5:
                    {
                        cardPos =  ccpAdd(tongQuePos, ccp(-widDis * 2.2, 0));
                    }
                        break;
                    default:
                        break;
                }
                
                if (!(s.height > 960)) {
                    visitCardbg->setScale(0.9);
                }
                
                visitCardbg->setPosition(cardPos);
                movePosList.push_back(cardPos);
                removeBtn((CCMenuItem *)getBtnByTag(TONGQUEITEMSTARTAG + k));
                addBtn(visitCardbg);
                visitCardbg->setTag(TONGQUEITEMSTARTAG + k);
                
                CCSprite *cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
                cardBg->setPosition(ccp(visitCardbg->getContentSize().width/2, visitCardbg->getContentSize().height/2));
                visitCardbg->addChild(cardBg,-1,-1);
                
                if (m_tongqueItem[k].getItemBtId() == 1) {
                    CCSprite * haveGot = CCSprite::createWithSpriteFrameName("tongqueGet.png");
                    visitCardbg->addChild(haveGot,10);
                    haveGot->setPosition(ccp(visitCardbg->getContentSize().width - haveGot->getContentSize().width*0.1, haveGot->getContentSize().height*0.1));
                    
                    haveGot = CCSprite::createWithSpriteFrameName("tongQueItemBg.png");
                    visitCardbg->addChild(haveGot,9);
                    haveGot->setScale(1.3*visitCardbg->getScale());
                    haveGot->setPosition(ccp(visitCardbg->getContentSize().width/2 , visitCardbg->getContentSize().height/2));
                    visitCardbg->setEnabled(false);
                }
                else
                {
                    canDobk = true;
                }
    
                float posYAdd = 0;
                float posYAdd2 = 0;
                if(m_tongqueItem[k].getItemType() == 9)
                {
                    posYAdd = 18;
                    posYAdd2 = 22;
                }
                itemName = SGCCLabelTTF::create(nameStr->getCString(), FONT_PANGWA, 20,CCSizeMake(visitCardbg->getContentSize().width*0.9, 0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop , COLOR_YELLOW);
                
                
                itemName->setPosition(ccp(visitCardbg->getContentSize().width/2 ,posYAdd2 -itemName->getContentSize().height/2-4));
                visitCardbg->addChild(itemName,1,ITEMNAMETAG);
                
                itemNumLab = SGCCLabelTTF::create(numStr->getCString(), FONT_PANGWA, 23,ccGREEN);
                itemNumLab->setAnchorPoint(ccp(0.5, 0));
                itemNumLab->setPosition(ccp(visitCardbg->getContentSize().width*0.5 , posYAdd) );
                visitCardbg->addChild(itemNumLab , 1);

        }
    }
    
    if (canDobk)
    {
       tognqueDokbBt->init("tongqueDobk.png", NULL, this, menu_selector(SGTongQueLayer::tongQueDobkSend),CCPointZero,false,true);
    }
    else
    {
        tognqueDokbBt->init("tongqueDobkDis.png", NULL, this, menu_selector(SGTongQueLayer::tongQueDobkSend),CCPointZero,false,true);
    }
    showDobkCostInfo();
}

void SGTongQueLayer::yangxinRotation()
{
    if (updateProgress->getPercentage() < 100) {
        yangxindian->resumeSchedulerAndActions();
    }
    else
    {
        yangxindian->pauseSchedulerAndActions();
        
    }
}

void SGTongQueLayer::refreshGold()
{
    if (havegold ) {
        havegold->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString());
        havejg->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString());
    }
}

void SGTongQueLayer::showtongqueItemDetail(SGItemInfo &tongqueItemInfo)
{
    SGBaseMilitaryCard * YangXinShowItem = NULL;
    int type = 0;
    SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
    switch (tongqueItemInfo.getItemType()) {
        case 0:
        {
            //元宝
            //                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("元宝x%d",tongqueItemInfo.getItemCount())->getCString(), FONT_BOXINFO, 20);
        }
            break;
        case 1:
        {
            //铜钱
            //                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("铜钱x%d",tongqueItemInfo.getItemCount())->getCString(), FONT_BOXINFO, 20);
        }
            break;
        case 2:
        {
            //Pvp积分
            //                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("Pvp积分x%d",tongqueItemInfo.getItemCount())->getCString(), FONT_BOXINFO, 20);
        }
            break;
        case 3:
        {
            //军功
            //                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("军功x%d",tongqueItemInfo.getItemCount())->getCString(), FONT_BOXINFO, 20);
        }
            break;
        case 4:
        {
            //小喇叭
            //                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("小喇叭x%d",tongqueItemInfo.getItemCount())->getCString(), FONT_BOXINFO, 20);
        }
            break;
        case 5:
        {
            //炼丹秘方
            //                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("炼丹秘方x%d",tongqueItemInfo.getItemCount())->getCString(), FONT_BOXINFO, 20);
        }
            break;
        case 6:
        {
            //武将
            SGOfficerCard *officerCard = new SGOfficerCard();
            YangXinShowItem = officerCard;
            type = 1;
            CCDictionary *dicc = SGCardAttribute::getValue(tongqueItemInfo.getItemLevel(), tongqueItemInfo.getItemId());
            SGOfficerDataModel *temp = staticDataManager->getOfficerById(tongqueItemInfo.getItemId());
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(tongqueItemInfo.getItemId());
            
//            if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = staticDataManager->getExpByTypeAndId(type,tongqueItemInfo.getItemLevel()+1);
            
            officerCard->setItemId(tongqueItemInfo.getItemId());
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
            officerCard->setCurrLevel(tongqueItemInfo.getItemLevel());
            officerCard->setMaxExp(expdata->getExpValue());
            officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
            officerCard->setOfficerName(temp->getOfficerName());
            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
            officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            officerCard->setRound(temp->getOfficerRound());
            officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            officerCard->setGoverning(temp->getOfficerGoverning());
            officerCard->setRace(temp->getOfficerRace());
            officerCard->setItemType(temp->getOfficerType());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            
            std::vector<int> reFateList =  temp->getOfficerFateList();
            
            officerCard->setFateList(reFateList);
            
            //转生次数
            officerCard->setAdNum(temp->getAdvNum());
            //武将原型ID
            officerCard->setProtoId(temp->getOfficerProtoId());
            //速度修正系数
            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
            
            officerCard->setHeadSuffixNum(temp->getIconId());//12.05
            
            if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
            {
                std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                officerCard->setOfficerSkil(atoi(generalskill.c_str()));
            }
            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
            {
                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                officerCard->setLordSkill(atoi(lordskill.c_str()));
            }
            
            officerCard->autorelease();
        }
            break;
        case 7:
        {
            SGEquipCard *equipCard = new SGEquipCard();
            YangXinShowItem = equipCard;
            type = 2;
            CCDictionary *dicc = SGCardAttribute::getValue(tongqueItemInfo.getItemLevel(), tongqueItemInfo.getItemId());
            SGEquipmentDataModel *temp = staticDataManager->getEquipById(tongqueItemInfo.getItemId());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(tongqueItemInfo.getItemId());
//            if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = staticDataManager->getExpByTypeAndId(type, tongqueItemInfo.getItemLevel()+1);
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            equipCard->setItemId(tongqueItemInfo.getItemId());
            //            equipCard->setSetNum(cardInfo.setnum());
            equipCard->setCurrLevel(tongqueItemInfo.getItemLevel());
            equipCard->setCurrExp(0);
            
            equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            equipCard->setMaxExp(expdata->getExpValue());
            equipCard->setRound(temp->getEquipRound());
            equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            equipCard->setExpRate(temp->getEquipExpRate());
            equipCard->setCurrStar(temp->getEquipCurrStarLevel());
            equipCard->setGoverning(temp->getEquipGoverning());
            equipCard->setItemType(temp->getEquipType());
            equipCard->setMaxStar(temp->getEquipNextId());
            equipCard->setOfficerName(temp->getEquipName());
            equipCard->setMaxLevel(temp->getEquipMaxLevel());
            
            equipCard->setHeadSuffixNum(temp->getIconId());//12.06
            
            if (temp->getEquipNextId()) {
                equipCard->setCost(temp->getEquipCost());
            }
            equipCard->autorelease();
            
        }
            break;
        case 9:
        {
            SGPropsCard *propCard = new SGPropsCard();
            YangXinShowItem = propCard;
            type = 4;
            SGPropsDataModel *prop = staticDataManager->getPropsById(tongqueItemInfo.getItemId());
            
            propCard->setItemId(tongqueItemInfo.getItemId());
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
            propCard->setHeadSuffixNum(prop->getIconId());
            propCard->autorelease();
        }
            break;
        case 10:
        {
            SGPiecesCard*piecesCard = new SGPiecesCard();
            YangXinShowItem = piecesCard;
            type = 5;
            SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(tongqueItemInfo.getItemId());
            piecesCard->setItemId(tongqueItemInfo.getItemId());
            piecesCard->setSell(pieces->getPiecesPrice());//价格
            piecesCard->setOfficerName(pieces->getPiecesName());//名称
            piecesCard->setPiecesName(pieces->getPiecesName());
            piecesCard->setCurrLevel(pieces->getStarLevel());//星级
            piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
            piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
            piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
            piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
            piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
            piecesCard->setItemType(pieces->getPiecesType());
            piecesCard->setCurrStar(pieces->getStarLevel());

            piecesCard->autorelease();
        }
            break;
        case 15:
        {
            SGConsumableDataModel *consumeModel = staticDataManager->getConsumableById(tongqueItemInfo.getItemId());
            if (consumeModel) {
                SGConsumableCard *consumeCard = new SGConsumableCard();
                YangXinShowItem = consumeCard;
                type = 6;
                
                
                consumeCard->setItemId(tongqueItemInfo.getItemId());
                consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
                consumeCard->setSell(consumeModel->getConsumePrice());
                consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
                consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
                consumeCard->setOfficerName(consumeModel->getConsumeName());
                consumeCard->setMoneyType(consumeModel->getMoneyType());
                consumeCard->setItemType(consumeModel->getConsumeType());
                consumeCard->setHeadSuffixNum(consumeModel->getIconId());
                consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
                consumeCard->setPicId(consumeModel->getConsumePicId());
                
                consumeCard->autorelease();
            }
        }
        break;
        case 17://材料
        {
            SGMaterialCard *materialCard = new SGMaterialCard();
            YangXinShowItem = materialCard;
            
            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(tongqueItemInfo.getItemId());
            
            materialCard->setItemId(tongqueItemInfo.getItemId());
            materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
            materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
            materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
            materialCard->setOfficerName(materialModel->getMaterialName());
            materialCard->setSell(materialModel->getMaterialSoldPrice());
            
            materialCard->autorelease();
        }
            break;

        default:
          break;
            
    }
    if (YangXinShowItem) {
        //SGMainManager::shareMain()->openChatShowLayerByType(YangXinShowItem,type,sg_tongQueLayer);
        SGMainManager::shareMain()->showCommonInfoBoxInProto(YangXinShowItem, this, 20001, false);
    }
}

void SGTongQueLayer::backHandler(CCObject * obj)
{
    main::TongQueYangXinRequest *request=new main::TongQueYangXinRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, request);
}










