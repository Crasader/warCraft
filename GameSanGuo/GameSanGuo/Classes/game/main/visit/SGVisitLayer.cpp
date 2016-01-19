//
//  SGVisitLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-18.
//
//

#include "SGVisitLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "cocos-ext.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "LotteryModule.pb.h"
#include "SGCantVisitBox.h"
#include "SGStaticDataManager.h"
#include "SGVisitShowCardLayer.h"
#include "SGGuideManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGStaticDataManager.h"
#define   VISITITEMNUM  2
USING_NS_CC_EXT;


int  SGVisitLayer::outType = 1;

SGVisitLayer::SGVisitLayer()
:legofftime(0),
legeqptime(0),
advofftime(0),
adveqptime(0)
,item1(NULL)
,item2(NULL)
,comboCard(NULL)
,progressCard(NULL)
,qidai(NULL)
,canVisitHuodong(false)
,singleCost(100)
,comboCost(1000)
,activitylotteryviplv(0)
, _touch(false)
,tag(0)
,time1(0)
,time2(0)
,progress(NULL)
,visit(NULL)
,havegold(NULL)
,havejg(NULL)
,nJunGongMxa(0)//军工上限
,currentTime(0)
,huoDongLayer(NULL)
,visit1(NULL)
,huoDongVisitBtn(NULL)
,normolVisitBtn(NULL)
,huoDongBg(NULL)
,huoDongLableBg(NULL)
,huoDongLable(NULL)
,singleVisit1(NULL)
,yb1(NULL)
,singleVisitMoney1(NULL)
,scrollview2(NULL)
,lotteryName(NULL)
,activityLotteryName(NULL)
,huoDong(NULL)
,isSongHuodong(true)
,haveRequestHuoVisit(false)
,canVisitCard(true)
{
    _touch= true;
}
SGVisitLayer::~SGVisitLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_VISIT_PURCH);
    notification->removeObserver(this, MSG_VISIT_HUODONG);
    notification->removeObserver(this, MSG_VISIT_HUODONGDATAS);
    
    m_visitActivityList.clear();
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_visitLayer);
}
SGVisitLayer *SGVisitLayer::create(int visitHuo,int a,int b,int c,int d,int j,std::vector<VisitActivity>& visitActivityList,const char*lotteryName, const char * activityLotteryName)
{
    SGVisitLayer *visitLayer = new SGVisitLayer();
    visitLayer->legofftime = a;
    visitLayer->legeqptime = b;
    visitLayer->advofftime = c;
    visitLayer->adveqptime = d;
    visitLayer->lotteryName = lotteryName;
    visitLayer->activityLotteryName = activityLotteryName;
    visitLayer->setVisitActivity(visitActivityList);
    visitLayer->activitylotteryviplv  = j;
    visitLayer->isSongHuodong  = visitHuo > 0 ? true : false;
    if (visitLayer && visitLayer->init(NULL, sg_visitLayer))
    {
        visitLayer->initView();
        visitLayer->autorelease();
        return visitLayer;
    }
    CC_SAFE_DELETE(visitLayer);
    return NULL;
}
void SGVisitLayer::initView()
{
    
    ResourceManager::sharedInstance()->bindTexture(SGMainManager::shareMain()->getPlistWithChannel().c_str(), RES_TYPE_LAYER_UI, sg_visitLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvisitlayer/sgvisitlayer2.plist",RES_TYPE_LAYER_UI ,sg_visitLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist",RES_TYPE_LAYER_UI ,sg_visitLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist",RES_TYPE_LAYER_UI ,sg_visitLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/Tips.plist",RES_TYPE_LAYER_UI ,sg_visitLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist",RES_TYPE_LAYER_UI ,sg_visitLayer);
    ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_visitLayer);

    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_VISIT_PURCH,
                              this,
                              callfuncO_selector(SGVisitLayer::buyLegListener));
    
    notification->addObserver(MSG_VISIT_HUODONG,
                              this,
                              callfuncO_selector(SGVisitLayer::buyLegHuoDongListener));
    notification->addObserver(MSG_VISIT_HUODONGDATAS,
                              this,
                              callfuncO_selector(SGVisitLayer::refreshHuoDongListener));
    
    float headhgt = SGMainManager::shareMain()->getVisitHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    

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
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_visitLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, btmhgt,bg->getContentSize().width, s.height - gonggao_3_2->getContentSize().height - btmhgt);
    bg->setTextureRect(r);
    //bg->setScaleY(r.size.height / bg->getContentSize().height);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt-45)));

    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(title_bg);
    
    if (activityLotteryName) {
        normolVisitBtn = SGButton::createFromLocal("visitHuoBtBg.png", lotteryName, this, menu_selector(SGVisitLayer::showNormalVisit),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    }
    else
    {
        normolVisitBtn = SGButton::createFromLocal("visitHuoBtBg.png", str_TongQueLayer_str28, this, menu_selector(SGVisitLayer::showNormalVisit),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    }
    menu->addChild(normolVisitBtn);
    normolVisitBtn->selected();
    normolVisitBtn->setPosition(ccpAdd(title_bg->getPosition(), ccp(-normolVisitBtn->getContentSize().width*0.7, -title_bg->getContentSize().height*.52)));
   
    if (lotteryName) {
        huoDongVisitBtn = SGButton::createFromLocal("visitHuoBtBg.png", activityLotteryName , this, menu_selector(SGVisitLayer::requestHuoDongVisit),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    }
    else
    {
        huoDongVisitBtn = SGButton::createFromLocal("visitHuoBtBg.png", str_TongQueLayer_str29, this, menu_selector(SGVisitLayer::requestHuoDongVisit),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    }
    menu->addChild(huoDongVisitBtn);
    huoDongVisitBtn->setPosition(ccpAdd(title_bg->getPosition(), ccp(huoDongVisitBtn->getContentSize().width*0.7, -title_bg->getContentSize().height*.52)));
    huoDong = CCSprite::createWithSpriteFrameName("visitHuoLable.png");
    addChild(huoDong,10);
    huoDong->runAction(CCRepeatForever::create( CCSequence::create(CCScaleTo::create(0.4, 0.8),CCScaleTo::create(0.4, 1),NULL) ));
    huoDong->setPosition(ccpAdd(title_bg->getPosition(), ccp(huoDongVisitBtn->getContentSize().width*0.4, -title_bg->getContentSize().height*.3)));
    huoDong->setVisible(false);
    
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_32);
    tableView->setTag(modelTag);
    CCSprite *heitiao = CCSprite::createWithSpriteFrameName("visit_heitiao.png");
    if (!heitiao)
    {
        return;
    }
    this->addChild(heitiao);
    heitiao->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width*.5f - heitiao->getContentSize().width/2,
                                                             -title_bg->getContentSize().height -heitiao->getContentSize().height*.5f)));
    
    tableViewHeight = (s.height - 900)/2 + 310;
    tableView->setDown((tableViewHeight - 310)/2 - 30);

    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - btmhgt*.68 -heitiao->getContentSize().height- headhgt +2));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp( - s.width/2,btmhgt*.68)));
 
    SGCCLabelTTF *font1 = SGCCLabelTTF::create(str_TongQueLayer_str30, FONT_BOXINFO, 28);
    addChild(font1,11);
    font1->setPosition(ccpAdd(heitiao->getPosition(), ccp(-140, 3.5)));

    
    
    CCSprite *yb = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yb,10);
    yb->setPosition(ccpAdd(font1->getPosition(), ccp(font1->getContentSize().width*.5f +yb->getContentSize().height*.5f +5 , 0)));

    havegold = SGCCLabelTTF::create(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString(), FONT_BOXINFO, 28);
    addChild(havegold,10);
    havegold->setAnchorPoint(ccp(0, .5f));
    havegold->setPosition(ccpAdd(yb->getPosition(), ccp(yb->getContentSize().width*.5f +5, 0)));
    
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("1");
     nJunGongMxa =dic1->valueForKey("value")->intValue();
    
    
    SGCCLabelTTF *font12 = SGCCLabelTTF::create(str_TongQueLayer_str42, FONT_BOXINFO, 28);
    addChild(font12,11);
    font12->setPosition(ccpAdd(heitiao->getPosition(), ccp(130, 1.5)));
    
    //改为金钱
    CCSprite *coin = CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(coin, 1);
    coin->setPosition(ccpAdd(font12->getPosition(), ccp(font12->getContentSize().width / 1.4, 0)));
    

    //2014-10-20，军功改为金钱
    havejg = SGCCLabelTTF::create(CCString::createWithFormat(" %d",SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString(), FONT_BOXINFO, 28);
    addChild(havejg,10);
    havejg->setAnchorPoint(ccp(0, .5f));
    havejg->setPosition(ccpAdd(coin->getPosition(), ccp(coin->getContentSize().width / 2, 1.5)));
    
    
    //活动探访界面
    
    CCSize layerSize = tableView->getContentSize();
    huoDongLayer = CCLayer::create();
    huoDongLayer->setContentSize(layerSize);
    huoDongLayer->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp( -s.width/2,btmhgt*0.68)));
    addChild(huoDongLayer);
    
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/visitHuoBg.plist", RES_TYPE_LAYER_UI, sg_visitLayer);
    
    huoDongBg = CCSprite::createWithSpriteFrameName("visitHuoBg.png");
    huoDongBg->setPosition(ccp(layerSize.width/2, layerSize.height+4));
    huoDongBg->setAnchorPoint(ccp(0.5, 1));
    
    float scaleRate = layerSize.width/huoDongBg->getContentSize().width;
    huoDongBg->setScale(scaleRate);
    huoDongBg->ignoreAnchorPointForPosition(false);
    huoDongLayer->addChild(huoDongBg);
   
    huoDongLableBg = CCSprite::createWithSpriteFrameName("visit_zik.png");
    huoDongLableBg->setPosition(ccp(layerSize.width/2, 0));
    huoDongLableBg->setAnchorPoint(ccp(0.5, 0));
    huoDongLableBg->setScaleX(layerSize.width/huoDongLableBg->getContentSize().width);
    huoDongLableBg->ignoreAnchorPointForPosition(false);
    
    float gap = layerSize.height -  huoDongBg->getContentSize().height/2 - huoDongLableBg->getContentSize().height;
    gap = gap > 0 ? (1 + gap/huoDongLableBg->getContentSize().height) : 1;
    huoDongLableBg->setScaleY(gap);
    huoDongLayer->addChild(huoDongLableBg,-1);
    
    visit1 = SGButton::create("visit_anniu.png", NULL, this, menu_selector(SGVisitLayer::sendHuodong1Visit),CCPointZero,false,true);
    visit1->setPosition(ccp(layerSize.width/2, layerSize.height - huoDongBg->getContentSize().height * scaleRate * 0.75 + visit1->getContentSize().height));
    menu->addChild(visit1);
    
    singleVisit1 = SGRichLabel::getFomLabel(str_TongQueLayer_str32, visit1->getContentSize(), FONT_BOXINFO, 27,ccRED,ccWHITE,ccRED);
    singleVisit1->setPosition(ccpAdd(visit1->getPosition(), ccp(28, -visit1->getContentSize().height*0.46))  );
    this->addChild(singleVisit1, 2);
    
    yb1 = CCSprite::createWithSpriteFrameName("store_yb.png");
    yb1->setPosition(ccpAdd(visit1->getPosition(), ccp(6*3-2, 0)));
    this->addChild(yb1,2);
    
    singleVisitMoney1 = SGCCLabelTTF::create("10", FONT_BOXINFO, 32);
    singleVisitMoney1->setPosition(ccpAdd(yb1->getPosition(), ccp(yb1->getContentSize().width + 5, 0)));
    this->addChild(singleVisitMoney1,2);
    
    
    huoDongLable = CCSprite::createWithSpriteFrameName("visitHuoDetail.png");
    huoDongLable->setPosition(ccp(0, layerSize.height - huoDongBg->getContentSize().height * scaleRate));
    huoDongLable->setAnchorPoint(ccp(0, 1));
    huoDongLable->ignoreAnchorPointForPosition(false);
    huoDongLayer->addChild(huoDongLable);
    
    scrollview2 = CCScrollView::create(CCSizeMake(layerSize.width, huoDongLable->getPositionY() - huoDongLable->getContentSize().height - btmhgt*0.3));
    scrollview2->setPosition(ccp(0, btmhgt * 0.9));
    scrollview2->setDirection(kCCScrollViewDirectionVertical);
    this->addChild(scrollview2);
    
//    main::ActivityLotteryEntryRequest *request = new main::ActivityLotteryEntryRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_VISIT_HUODONGDATAS, request);
    if(SGPlayerInfo::sharePlayerInfo()->getShowvip() > 0)
    {
        if(SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel() >=activitylotteryviplv-1)
        {
            huoDongVisitBtn->setVisible(true);
            huoDongVisitBtn->setEnabled(true);
            huoDong->setVisible(true);
            normolVisitBtn->setPosition(ccpAdd(title_bg->getPosition(), ccp(-normolVisitBtn->getContentSize().width*0.7, -title_bg->getContentSize().height*.52)));
            
        }
        else{
            huoDongVisitBtn->setVisible(false);
            huoDongVisitBtn->setEnabled(false);
            normolVisitBtn->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
//            spr_visit->setPosition(ccpAdd(normolVisitBtn->getPosition() , ccp(-normolVisitBtn->getContentSize().width/2, normolVisitBtn->getContentSize().height*.38)));
        }
    }
    
    showNormalVisit(normolVisitBtn);
    EFFECT_PLAY(MUSIC_ITEM);
    if(outType == 2)
    {
        SGVisitLayer::outType = 1;
        main::ActivityLotteryEntryRequest *request = new main::ActivityLotteryEntryRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_VISIT_HUODONGDATAS, request);
    }
        
}

 void SGVisitLayer::showNormalVisit(CCObject *obj)
{
    tableView->setVisible(true);
    tableView->setTouchEnabled(false);
    huoDongLayer->setVisible(false);
    visit1->setVisible(false);
    visit1->setEnabled(false);
    singleVisit1->setVisible(false);
    yb1->setVisible(false);
    singleVisitMoney1->setVisible(false);
    
    
    scrollview2->setVisible(false);
    scrollview2->setTouchEnabled(false);
    huoDongVisitBtn->selected();
    normolVisitBtn->unselected();
    
    if (qidai) {
        qidai->setVisible(false);
    }

}


void SGVisitLayer::requestHuoDongVisit(CCObject *obj)
{
    if (haveRequestHuoVisit)
    {
        this->showHuoDongVisit(NULL);
    }
    else
    {
        main::ActivityLotteryEntryRequest *request = new main::ActivityLotteryEntryRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_VISIT_HUODONGDATAS, request);
    }

}

void SGVisitLayer::showHuoDongVisit(CCObject *obj)
{
    tableView->setVisible(false);
    tableView->setTouchEnabled(false);
    huoDongLayer->setVisible(true);
    huoDongVisitBtn->unselected();
    normolVisitBtn->selected();
    
    if (canVisitHuodong) {
        visit1->setVisible(true);
        visit1->setEnabled(true);
        singleVisit1->setVisible(true);
        yb1->setVisible(true);
        singleVisitMoney1->setVisible(true);
        
        scrollview2->setVisible(true);
        scrollview2->setTouchEnabled(true);
    }
    
    if (qidai)
    {
        qidai->setVisible(true);
    }
}

void SGVisitLayer::sendHuodong1Visit(CCObject *obj)
{
    if (!canVisitCard)
    {
        return;
    }
    limitMenuTouch();
    
    CCDictionary * dict = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary * dict1 = (CCDictionary*)dict->objectForKey("31");
    int vipLev = dict1->valueForKey("value")->intValue();//标记为废弃。
    
    dict1 = (CCDictionary*)dict->objectForKey("49");
    int cost = dict1->valueForKey("value")->intValue();
    
    if(SGPlayerInfo::sharePlayerInfo()->getShowvip() > 0)
    {
//        if ( SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel() < vipLev)//activitylotteryviplv)
        if ( SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel() < activitylotteryviplv)
        {
            SG_SHOW_WINDOW(CCString::createWithFormat(str_TongQueLayer_str41,activitylotteryviplv)->getCString());
        }
        else
        {
            if ( SGPlayerInfo::sharePlayerInfo()->getPlayerGold() < cost)
            {
//                SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,777 ,1);
                SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,1);
                SGMainManager::shareMain()->showBox(cantVisitBox);
            }
            else
            {
                main::ActivityLotteryRequest *request = new main::ActivityLotteryRequest();
                request->set_iscombo(0);
                SGSocketClient::sharedSocketClient()->send(MSG_VISIT_HUODONG, request);
                visit1->setEnabled(false);
            }
        }
    }
    else
    {
        SG_SHOW_WINDOW(str_MainLayer_str15);
    }
}

void SGVisitLayer::sendHuodong10Visit(CCObject *obj)
{
    CCAssert(false, "unknow func.");
    if (!canVisitCard)
    {
        return;
    }
    limitMenuTouch();
    
    CCDictionary * dict = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary * dict1 = (CCDictionary*)dict->objectForKey("31");
    int vipLev = dict1->valueForKey("value")->intValue();
    
    if ( SGPlayerInfo::sharePlayerInfo()->getPlayerGold() < comboCost) {
//        SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,777,1);
        SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,1);
        SGMainManager::shareMain()->showBox(cantVisitBox);
    }
    else
    {
        main::ActivityLotteryRequest *request = new main::ActivityLotteryRequest();
        request->set_iscombo(1);
        SGSocketClient::sharedSocketClient()->send(MSG_VISIT_HUODONG, request);
    }
}



SNSTableViewCellItem *SGVisitLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGVisitItem1 *item11 = (SGVisitItem1*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGVisitItem2 *item22 = (SGVisitItem2*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

    if (num<1)
    {
        if (NULL == item11)
        {
            item11 = SGVisitItem1::create(this,num,
                                            legofftime,
                                              0,
                                              isSongHuodong);
        }
        VisitActivity* visitActivity = getVisitActivityByTyew(num);
        if (visitActivity != NULL) {
            item11->setActivityMsg(currentTime, (time_t)visitActivity->starTime,(time_t)visitActivity->endTime,visitActivity->description);
        }
        item11->setIsCanTouch(_touch);
    }
    else
    {
        if (NULL == item22) {
            item22 = SGVisitItem2::create(this,2,advofftime,
                                          adveqptime,
                                          0,
                                          0);
        }
        VisitActivity* visitActivity = getVisitActivityByTyew(num);
        if (visitActivity != NULL) {
            item22->setActivityMsg(currentTime,(time_t)visitActivity->starTime,(time_t)visitActivity->endTime,visitActivity->description);
        }
        item22->setIsCanTouch(_touch);
        
    }
    
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    
    if (num <VISITITEMNUM/2) {
        int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        if (guideId == guide_tag_31 || guideId == guide_tag_32) { //新手引导或reload的时候都屏蔽Cell
            item11->setIsCanTouch(false);
        }
        return item11;
    }
    else{
        if (guideId == guide_tag_31 || guideId == guide_tag_32) { //新手引导或reload的时候都屏蔽Cell
            item22->setIsCanTouch(false);
        }
        return item22;
    }
}
    
unsigned int  SGVisitLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    currentTime = (time_t)getCurrentTime();
    if (VISITITEMNUM < 4) {
        tableView->setTouchEnabled(false);
    }
    return VISITITEMNUM;
}
void SGVisitLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_33);
}

void SGVisitLayer::backHandler()
{
    SGMainManager::shareMain()->showHelpLayer();
}



void SGVisitLayer::limitMenuTouch()
{
    schedule(schedule_selector(SGVisitLayer::resetMenuTouch) , 2);
    canVisitCard = false;
}

void SGVisitLayer::resetMenuTouch()
{
    unschedule(schedule_selector(SGVisitLayer::resetMenuTouch));
    canVisitCard = true;
}

void SGVisitLayer::buyLegHandler(SGVisitItem1 *visitItem)
{
    if (!canVisitCard)
    {
        return;
    }
    limitMenuTouch();
    
    
    item1 = visitItem;
    tag = visitItem->choose;
    GPCCLOG("tag is %d",tag);
    int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
    
    main::LotteryPurchRequest *request = new main::LotteryPurchRequest();

    SGStaticDataManager * dataM = SGStaticDataManager::shareStatic();
    CCDictionary * dict = dataM->getDisplayValueDict();
    
    CCDictionary * one = (CCDictionary *) dict->objectForKey("45");
    int oneCost = one->valueForKey("value")->intValue();
    
    CCDictionary * ten = (CCDictionary *)dict->objectForKey("46");
    int tenCost = ten->valueForKey("value")->intValue();
    
    progress = visitItem->left;
    visit = visitItem->visit1;
    if (tag == 110)   //元宝一次
    {
        time1 = visitItem->officerTime;
        if (gold <oneCost && time1 != 0)
        {
            SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,1);
            SGMainManager::shareMain()->showBox(cantVisitBox);
        }
        else
        {
        request->set_type(0);
        request->set_iscombo(0);
       
        if ( time1 == 0)
        {
            request->set_iscombo(2);
        }
            SGMainManager::shareMain()->trackPurchase(CCString::create(str_TongQueLayer_str34), 1, 300);
            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_PURCH, request);
        }
    }
    else if(tag == 120)  //元宝十次
    {
        if (gold < tenCost)
        {
            SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,1);
            SGMainManager::shareMain()->showBox(cantVisitBox);
        }
        else
        {
            request->set_type(0);
            request->set_iscombo(1);
            CCLOG("120");
            SGMainManager::shareMain()->trackPurchase(CCString::create(str_TongQueLayer_str34), 10, 300);
            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_PURCH, request);
        }
    }
    else if(tag == 111)
    {

    }
    else if(tag == 121)
    {

    }

}
void SGVisitLayer::buyHandler(SGVisitItem2 *visitItem)
{
    if (!canVisitCard)
    {
        return;
    }
    limitMenuTouch();

    ResourceManager::sharedInstance()->bindTexture(SGMainManager::shareMain()->getPlistWithChannel().c_str(), RES_TYPE_LAYER_UI, sg_visitLayer);

    item2 = visitItem;
    tag = visitItem->type;
    int gold = SGPlayerInfo::sharePlayerInfo()->getPlayerCoins();
    
    main::LotteryPurchRequest *request = new main::LotteryPurchRequest();
    
    SGStaticDataManager * dataM = SGStaticDataManager::shareStatic();
    CCDictionary * dict = dataM->getDisplayValueDict();
    
    CCDictionary * one = (CCDictionary *) dict->objectForKey("47");
    int oneCost = one->valueForKey("value")->intValue();
    
    CCDictionary * ten = (CCDictionary *)dict->objectForKey("48");
    int tenCost = ten->valueForKey("value")->intValue();
    
    
    progress = visitItem->left;
    visit = visitItem->visit1;
    
    GPCCLOG("visit2 tag: %d", tag);
    if (tag == 112)   //金币探访一次
    {
        time1 = visitItem->officerTime;
        if (gold <oneCost && time1 != 0)
        {
            SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,2);
            SGMainManager::shareMain()->showBox(cantVisitBox);
        }
        else
        {
            request->set_type(2);
            request->set_iscombo(0);
            
            if ( time1 == 0)
                request->set_iscombo(2);
            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_PURCH, request);
        }
    }
    else if(tag == 122)  //金币探访10次
    {

        if (gold < tenCost)
        {
            
//            SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,11 ,1);
            SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,2);
            SGMainManager::shareMain()->showBox(cantVisitBox);
        
        }else
        {
            request->set_type(2);
            request->set_iscombo(1);

            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_PURCH, request);
        }
        
    }
    else if(tag == 113)
    {
        time2 = visitItem->equipTime;
        if (gold <100 && time2 != 0)
        {
            SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,4,1);
            SGMainManager::shareMain()->showBox(cantVisitBox);
        }
        else
        {
            request->set_type(3);
            request->set_iscombo(0);
            
            if ( time2 == 0)
            {
                request->set_iscombo(2);
            }
            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_PURCH, request);
        }
    }
    else if(tag == 123)
    {
        
        if (gold < 1000)
        {
            if (gold <100)
            {
                SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,12,1);
                SGMainManager::shareMain()->showBox(cantVisitBox);
            }
            else
            {
            SGCantVisitBox *cantVisitBox = SGCantVisitBox::create(this,8,1);
            SGMainManager::shareMain()->showBox(cantVisitBox);
            }
        }
        else
        {
            request->set_type(3);
            request->set_iscombo(1);
            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_PURCH, request);
        }
        
    }
}


//探访确认之后， 这里接收服务器发送过来探访到的所有的东西
void SGVisitLayer::buyLegListener(CCObject *obj)
{

    ResourceManager::sharedInstance()->bindTexture(SGMainManager::shareMain()->getPlistWithChannel().c_str(), RES_TYPE_LAYER_UI, sg_visitLayer);

    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::LotteryPurchResponse *response = (main::LotteryPurchResponse *)sr->m_msg;
    SGPlayerInfo* playerInfo = SGPlayerInfo::sharePlayerInfo();
    //放所有的探访到的物品
    CCArray *_array = CCArray::create();
    
    if (sr)
    {
        
        //要购买的东西，传递给要显示的layer
        //不是台湾的，打开经验丹显示
        if (!SGMainManager::shareMain()->getCurrentChannelIsTW())
        {
            main::PropsCardItemProto cardInfo = response->buypropscard();
            if (&cardInfo)
            {
                propBuyCard = new SGPropsCard();
                SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
                
                propBuyCard->setSsid(cardInfo.ssid());
                propBuyCard->setItemId(cardInfo.itemid());
                propBuyCard->setIsProt(0);
                
                propBuyCard->setSell(prop->getpropsSell());
                propBuyCard->setValue(prop->getpropsValue());
                propBuyCard->setRace(prop->getPropsCountry());
                propBuyCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
                propBuyCard->setType(prop->getPropsPropsType());
                propBuyCard->setItemType(prop->getPropsType());
                propBuyCard->setCurrStar(prop->getPropsCurrStarLevel());
                propBuyCard->setHeadSuffixNum(prop->getIconId());//12.05
                propBuyCard->setCurNum(cardInfo.curritemnumber());
                
                //_array->addObject(propBuyCard);
                //playerInfo->addPropCard(propBuyCard);
                propBuyCard->retain();
                propBuyCard->autorelease();
            }
        }
        
        //武将
        int len = response->officercards_size();
        for (int i = 0 ; i <len; i++)
        {
            SGOfficerCard *officerCard = new SGOfficerCard();
            main::OfficerCardItemProto proto = response->officercards(i);
            
            //同名武将会因碎裂而获得碎片，记录数据方便探访页面显示。
            if (proto.iscrash())
            {
                officerCard->setCrashNum(proto.crashfragmentnum());
            }
     
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
            
//            CCLOG("VISIT OFFICER || %d -- itemId=%d, officerName=%s, fragId=%d, fragNum=%d", proto.iscrash(), proto.itemid(), temp->getOfficerName()->m_sString.c_str(), proto.crashfragmentid(), proto.crashfragmentnum());
            
            officerCard->setSsid(proto.ssid());
            officerCard->setItemId(proto.itemid());
            officerCard->setCurrExp(0);
            officerCard->setLordId(proto.lordskillid());
            officerCard->setIsProt(0);
            officerCard->setCurrLevel(proto.currlevel());
            
            if (temp->getOfficerNextId())
            {
                officerCard->setCost(temp->getOfficerCost());
            }
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
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
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            officerCard->setFateList(temp->getOfficerFateList());
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

                _array->addObject(officerCard);
                //playerInfo->addOfficerCard(officerCard);
                officerCard->autorelease();             
 
        }        
        //装备
        int equiplen = response->equipmentcards_size();
        for (int i = 0 ; i <equiplen; i++)
        {
            SGEquipCard *equipCard = new SGEquipCard();
            main::EquipmentCardItemProto cardInfo = response->equipmentcards(i);            
            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());

            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            equipCard->setSsid(cardInfo.ssid());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            equipCard->setItemId(cardInfo.itemid());
            //            equipCard->setSetNum(cardInfo.setnum());
            equipCard->setCurrLevel(cardInfo.currlevel());
            equipCard->setCurrExp(0);
            equipCard->setOfficerCardId(0);
            equipCard->setIsProt(0);
            if (cardInfo.officercarditemid() >0)
            {
                SGOfficerCard *officer = playerInfo->getOfficerCard(cardInfo.officercarditemid());
                officer->addEquip(equipCard);
            }
            
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
            if (temp->getFateDesc())
            {
                equipCard->setFateDesc(temp->getFateDesc());
            }

			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
			
            if (temp->getEquipNextId())
            {
                equipCard->setCost(temp->getEquipCost());
            }
            _array->addObject(equipCard);
            //playerInfo->addEquipCard(equipCard);
            equipCard->autorelease();            
        }
        //道具
        int proplen = response->propscards_size();
        for (int i = 0 ; i <proplen; i++)
        {
            SGPropsCard *propCard = new SGPropsCard();
            main::PropsCardItemProto cardInfo = response->propscards(i);
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
          
            propCard->setSsid(cardInfo.ssid());
            propCard->setItemId(cardInfo.itemid());
            propCard->setIsProt(0);
          
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
            propCard->setHeadSuffixNum(prop->getIconId());//12.05
            propCard->setCurNum(cardInfo.curritemnumber());
			  
            _array->addObject(propCard);
            //playerInfo->addPropCard(propCard);
            propCard->autorelease();
        }
        //碎片
        int piecesize=response->fragmentcards_size();
        for (int i=0; i<piecesize;i++)
        {
            SGPiecesCard *piececard = new SGPiecesCard();
            main::OfficerFragmentCardItemProto pieceinfo = response->fragmentcards(i);
            SGPiecesDataModel *piece = SGStaticDataManager::shareStatic()->getPiecesDictById(pieceinfo.itemid());
            
            piececard->setSsid(pieceinfo.ssid());
            piececard->setItemId(pieceinfo.itemid());
            piececard->setPiecesCurNum(pieceinfo.curritemnumber());// 碎片数量
            piececard->setSell(piece->getPiecesPrice());//价格
            piececard->setOfficerName(piece->getPiecesName());//名称
            piececard->setPiecesName(piece->getPiecesName());
            piececard->setItemType(piece->getPiecesType());//类型
            piececard->setCurrLevel(piece->getStarLevel());//星级
            piececard->setPiecesDesc(piece->getPiecesDesc());//碎片描述
            piececard->setCanUseCommon(piece->getCommonPieces());//可使用的通用碎片数量
            piececard->setCallCardId(piece->getCallingCardId());//设置碎片合成后召唤的武将id
            piececard->setMaxNum(piece->getMaxPiecesNum());//碎片最大数量
            piececard->setSummonNeed(piece->getcallNeedPieces());//召唤所需要的碎片的数量
			piececard->setCurrStar(piece->getStarLevel());
            
            _array->addObject(piececard);
            //playerInfo->addPiecesCard(piececard);
            piececard->autorelease();

        }
        
#pragma mark ==============================接收所有得到的东西=====================================
        //这里做所有可能出现的类型的，包括所有的东西
        
        
        //消耗品
		int consumeLen = response->consumablecards_size();
		for (int i = 0; i < consumeLen; i++)
		{
			main::ConsumableCardItemProto consume = response->consumablecards(i);
			SGConsumableCard *consumeCard = new SGConsumableCard();
			SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(consume.itemid());
			
            if (consumeModel)
            {
                consumeCard->setCurrNum(consume.curritemnumber());
                consumeCard->setSsid(consume.ssid());
                consumeCard->setItemId(consume.itemid());
                
                consumeCard->setSell(consumeModel->getConsumePrice());
                consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
                consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
                consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
                consumeCard->setOfficerName(consumeModel->getConsumeName());
                consumeCard->setMoneyType(consumeModel->getMoneyType());
                consumeCard->setItemType(consumeModel->getConsumeType());
                consumeCard->setHeadSuffixNum(consumeModel->getIconId());
                consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
                consumeCard->setPicId(consumeModel->getConsumePicId());
                consumeCard->setCurrNum(consume.curritemnumber());
                
                _array->addObject(consumeCard);
                //playerInfo->addConsumableCard(consumeCard);
            }
            consumeCard->autorelease();
		}
        //材料
        int materialLen = response->materialitems_size();
        
        for (int i = 0; i < materialLen; i++)
        {
            main::MaterialItemProto materialInfo = response->materialitems(i);
            SGMaterialCard *materialCard = new SGMaterialCard();
            
            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(materialInfo.itemid());
            
            
            if (materialModel)
            {
                materialCard->setSsid(materialInfo.ssid());
                materialCard->setMaterialCurrNum(materialInfo.itemnum());
                materialCard->setItemId(materialInfo.itemid());
                
                materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
                materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
                materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
                materialCard->setOfficerName(materialModel->getMaterialName());
                materialCard->setSell(materialModel->getMaterialSoldPrice());
                _array->addObject(materialCard);

                //playerInfo->addMaterialCard(materialCard);
            }
            materialCard->autorelease();
        }
        
    }  
        //int percent = response->progresspercent();
        //progress->setPercentage(percent);
    
        int balance = response->balance();
    
        int currTime = response->updatetime();
        if (tag == 110)
        {
            if (item1)
            {
                item1->officerTime = currTime;
                item1->m_noticeGoldFree->setVisible(false);
                legofftime = currTime;
            }
        }
        if (tag == 111)
        {
            if (item1)
            {
                item1->officerTime =currTime;
                item1->m_noticeGoldFree->setVisible(false);
                legeqptime = currTime;
            }
        }
        if (tag == 112)
        {
            if (item2)
            {
                item2->officerTime = currTime;
                item2->m_noticeCoinFree->setVisible(false);
                advofftime = currTime;
            }
        }
        if (tag == 113)
        {
            if (item2)
            {
                item2->equipTime = currTime;
                item2->m_noticeCoinFree->setVisible(false);
                adveqptime = currTime;
            }
        }
       // CCLOG("%d",currTime);
    
    
    if (tag == 110) {
        if (legeqptime != 0 &&
            advofftime !=0 &&
            adveqptime !=0) {
            SGPlayerInfo::sharePlayerInfo()->setFreevisit(0);
        }

    }
    if (tag == 111) {
        if (legofftime != 0 &&
            advofftime !=0 &&
            adveqptime !=0) {
            SGPlayerInfo::sharePlayerInfo()->setFreevisit(0);
        }

    }
    if (tag == 112) {
        if (legofftime != 0 &&
            legeqptime != 0 &&
            adveqptime !=0) {
            SGPlayerInfo::sharePlayerInfo()->setFreevisit(0);
        }

    }
    if (tag == 113) {
        if (legofftime != 0 &&
            legeqptime != 0 &&
            advofftime !=0 ) {
            SGPlayerInfo::sharePlayerInfo()->setFreevisit(0);
        }

    }
//    spr_visit->setVisible(SGPlayerInfo::sharePlayerInfo()->getFreevisit() == 1);
           if (tag == 110) {
           
            if (time1 == 0) {
                CCSprite *sprte = CCSprite::createWithSpriteFrameName("font_300.png");
                visit->setFontImage(sprte->displayFrame());                
            } else{
                 SGPlayerInfo::sharePlayerInfo()->setPlayerGold(balance);
            }
                   }
        if (tag == 111) {
            if (time2 == 0) {
                //CCSprite *sprte = CCSprite::createWithSpriteFrameName("font_100.png");
                //visit->setFontImage(sprte->displayFrame());
            }else{
             SGPlayerInfo::sharePlayerInfo()->setPlayerGold(balance);
            }
        }
        if (tag == 120) {
            if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold()>=3000) {
                SGPlayerInfo::sharePlayerInfo()->setPlayerGold(balance);

            }else{
                SGPlayerInfo::sharePlayerInfo()->setPlayerGold(balance);
            }
        }
        if (tag == 121)
       {
            
            if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold()>=1000) {
                SGPlayerInfo::sharePlayerInfo()->setPlayerGold(balance);
            }
            else{
                SGPlayerInfo::sharePlayerInfo()->setPlayerGold(balance);
            }
        }
        if (tag == 112)
            {
                
                if (time1 == 0)
                {
                    CCSprite *sprte = CCSprite::createWithSpriteFrameName("font_300_1.png");
                    visit->setFontImage(sprte->displayFrame());
                } else
                {
                    SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(balance);
                }
            }
        if (tag == 113)
            {
                if (time2 == 0)
                {
                    //CCSprite *sprte = CCSprite::createWithSpriteFrameName("font_100_1.png");
                    //visit->setFontImage(sprte->displayFrame());
                }else
                {
                    SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(balance);
                }
            }
        if (tag == 122)
              {
                  if (SGPlayerInfo::sharePlayerInfo()->getPlayerCoins()>=3000)
                 {
                    SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(balance);
                    
                 }else
                      {
                        SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(balance);
                      }
               }
        if (tag == 123)
               {
                
                if (SGPlayerInfo::sharePlayerInfo()->getPlayerCoins()>=1000)
                   {
                     SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(balance);
                   }
                else
                      {
                        SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(balance);
                      }

               }
    
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId <= guide_tag_40) {
        SGGuideManager::shareManager()->dismissLayer();
    }
    
    havegold->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString());
    havejg->setString(CCString::createWithFormat(" %d",SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString());
    
    bool useGoldDoor = (tag == 110|| tag == 111 || tag == 120 || tag == 121);
    
    if (_array->count() > 10 && comboCard == NULL)
    {
        comboCard = (SGBaseMilitaryCard *)_array->objectAtIndex(_array->count() - 1);
        _array->removeLastObject();
        
    }
    
    CCArray * arrayrand = CCArray::create();
    while(_array->count())
    {
        CCObject * obj = _array->randomObject();
        arrayrand->addObject(obj);
        _array->removeObject(obj);
    }
    //这里得到服务器发过来的所有的东西，都在_array里，之后的动画显示
    SGMainManager::shareMain()->showVisitCards(arrayrand,NULL,comboCard,progressCard, useGoldDoor,false, propBuyCard);
    
    SGMainManager::shareMain()->updataUserMsg();
    comboCard = NULL;
    _array = NULL;
    progressCard = NULL;
    
    
}

void SGVisitLayer::buyLegHuoDongListener(CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture(SGMainManager::shareMain()->getPlistWithChannel().c_str(), RES_TYPE_LAYER_UI, sg_visitLayer);
    SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ActivityLotteryResponse *response = (main::ActivityLotteryResponse *)sr->m_msg;
    CCArray *_array = CCArray::create();
    CCArray *_huoArray = CCArray::create();
    if (sr)
    {
        //要购买的东西，传递给要显示的layer
        //不是台湾的，打开经验丹显示
        if (!SGMainManager::shareMain()->getCurrentChannelIsTW())
        {
            main::PropsCardItemProto cardInfo = response->buypropscard();
            if (&cardInfo)
            {
                propBuyCard = new SGPropsCard();
                SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
                
                propBuyCard->setSsid(cardInfo.ssid());
                propBuyCard->setItemId(cardInfo.itemid());
                propBuyCard->setIsProt(0);
                
                propBuyCard->setSell(prop->getpropsSell());
                propBuyCard->setValue(prop->getpropsValue());
                propBuyCard->setRace(prop->getPropsCountry());
                propBuyCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
                propBuyCard->setType(prop->getPropsPropsType());
                propBuyCard->setItemType(prop->getPropsType());
                propBuyCard->setCurrStar(prop->getPropsCurrStarLevel());
                propBuyCard->setHeadSuffixNum(prop->getIconId());//12.05
                propBuyCard->setCurNum(cardInfo.curritemnumber());
                
                //_array->addObject(propBuyCard);
                //playerInfo->addPropCard(propBuyCard);
                propBuyCard->retain();
                propBuyCard->autorelease();
            }
        }
        
        //武将
        int len = response->officercards_size();
        for (int i = 0 ; i <len; i++)
        {
            SGOfficerCard *officerCard = new SGOfficerCard();
            main::OfficerCardItemProto proto = response->officercards(i);
            
            //同名武将会因碎裂而获得碎片，记录数据方便探访页面显示。
            if (proto.iscrash())
            {
                officerCard->setCrashNum(proto.crashfragmentnum());
            }
            
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
            
            
            officerCard->setSsid(proto.ssid());
            officerCard->setItemId(proto.itemid());
            officerCard->setCurrExp(0);
            officerCard->setLordId(proto.lordskillid());
            officerCard->setIsProt(0);
            officerCard->setCurrLevel(proto.currlevel());
            
            if (temp->getOfficerNextId())
            {
                officerCard->setCost(temp->getOfficerCost());
            }
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
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
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            officerCard->setFateList(temp->getOfficerFateList());
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
            
            _array->addObject(officerCard);
            //playerInfo->addOfficerCard(officerCard);
            officerCard->autorelease();
            
        }
        //装备
        int equiplen = response->equipmentcards_size();
        for (int i = 0 ; i <equiplen; i++)
        {
            SGEquipCard *equipCard = new SGEquipCard();
            main::EquipmentCardItemProto cardInfo = response->equipmentcards(i);
            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
            
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            equipCard->setSsid(cardInfo.ssid());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            equipCard->setItemId(cardInfo.itemid());
            //            equipCard->setSetNum(cardInfo.setnum());
            equipCard->setCurrLevel(cardInfo.currlevel());
            equipCard->setCurrExp(0);
            equipCard->setOfficerCardId(0);
            equipCard->setIsProt(0);
            if (cardInfo.officercarditemid() >0)
            {
                SGOfficerCard *officer = playerInfo->getOfficerCard(cardInfo.officercarditemid());
                officer->addEquip(equipCard);
            }
            
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
            if (temp->getFateDesc())
            {
                equipCard->setFateDesc(temp->getFateDesc());
            }
            
			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
			
            if (temp->getEquipNextId())
            {
                equipCard->setCost(temp->getEquipCost());
            }
            _array->addObject(equipCard);
            //playerInfo->addEquipCard(equipCard);
            equipCard->autorelease();
        }
        //道具
        int proplen = response->propscards_size();
        for (int i = 0 ; i <proplen; i++)
        {
            SGPropsCard *propCard = new SGPropsCard();
            main::PropsCardItemProto cardInfo = response->propscards(i);
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
            
            propCard->setSsid(cardInfo.ssid());
            propCard->setItemId(cardInfo.itemid());
            propCard->setIsProt(0);
            
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
            propCard->setHeadSuffixNum(prop->getIconId());//12.05
            propCard->setCurNum(cardInfo.curritemnumber());
            
            _array->addObject(propCard);
            //playerInfo->addPropCard(propCard);
            propCard->autorelease();
        }
        //碎片
        int piecesize=response->fragmentcards_size();
        for (int i=0; i<piecesize;i++)
        {
            SGPiecesCard *piececard = new SGPiecesCard();
            main::OfficerFragmentCardItemProto pieceinfo = response->fragmentcards(i);
            SGPiecesDataModel *piece = SGStaticDataManager::shareStatic()->getPiecesDictById(pieceinfo.itemid());
            
            piececard->setSsid(pieceinfo.ssid());
            piececard->setItemId(pieceinfo.itemid());
            piececard->setPiecesCurNum(pieceinfo.curritemnumber());// 碎片数量
            piececard->setSell(piece->getPiecesPrice());//价格
            piececard->setOfficerName(piece->getPiecesName());//名称
            piececard->setPiecesName(piece->getPiecesName());
            piececard->setItemType(piece->getPiecesType());//类型
            piececard->setCurrLevel(piece->getStarLevel());//星级
            piececard->setPiecesDesc(piece->getPiecesDesc());//碎片描述
            piececard->setCanUseCommon(piece->getCommonPieces());//可使用的通用碎片数量
            piececard->setCallCardId(piece->getCallingCardId());//设置碎片合成后召唤的武将id
            piececard->setMaxNum(piece->getMaxPiecesNum());//碎片最大数量
            piececard->setSummonNeed(piece->getcallNeedPieces());//召唤所需要的碎片的数量
			piececard->setCurrStar(piece->getStarLevel());
            
            _array->addObject(piececard);
            //playerInfo->addPiecesCard(piececard);
            piececard->autorelease();
            
        }
        
#pragma mark ==============================接收所有得到的东西=====================================
        //这里做所有可能出现的类型的，包括所有的东西
        
        
        //消耗品
		int consumeLen = response->consumablecards_size();
		for (int i = 0; i < consumeLen; i++)
		{
			main::ConsumableCardItemProto consume = response->consumablecards(i);
			SGConsumableCard *consumeCard = new SGConsumableCard();
			SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(consume.itemid());
			
            if (consumeModel)
            {
                consumeCard->setCurrNum(consume.curritemnumber());
                consumeCard->setSsid(consume.ssid());
                consumeCard->setItemId(consume.itemid());
                
                consumeCard->setSell(consumeModel->getConsumePrice());
                consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
                consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
                consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
                consumeCard->setOfficerName(consumeModel->getConsumeName());
                consumeCard->setMoneyType(consumeModel->getMoneyType());
                consumeCard->setItemType(consumeModel->getConsumeType());
                consumeCard->setHeadSuffixNum(consumeModel->getIconId());
                consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
                consumeCard->setPicId(consumeModel->getConsumePicId());
                consumeCard->setCurrNum(consume.curritemnumber());
                
                _array->addObject(consumeCard);
                //playerInfo->addConsumableCard(consumeCard);
            }
            consumeCard->autorelease();
		}
        //材料
        int materialLen = response->materialitems_size();
        
        for (int i = 0; i < materialLen; i++)
        {
            main::MaterialItemProto materialInfo = response->materialitems(i);
            SGMaterialCard *materialCard = new SGMaterialCard();
            
            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(materialInfo.itemid());
            
            
            if (materialModel)
            {
                materialCard->setSsid(materialInfo.ssid());
                materialCard->setMaterialCurrNum(materialInfo.itemnum());
                materialCard->setItemId(materialInfo.itemid());
                
                materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
                materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
                materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
                materialCard->setOfficerName(materialModel->getMaterialName());
                materialCard->setSell(materialModel->getMaterialSoldPrice());
                _array->addObject(materialCard);
                
                //playerInfo->addMaterialCard(materialCard);
            }
            materialCard->autorelease();
        }
    
        len = response->officerfragment_size();
        int itemType = 0;
        SGBaseMilitaryCard * showCard = NULL;
        for (int i = 0 ; i <len; i++)
        {
            main::ActivityLotteryItem lotteryProto = response->officerfragment(i);
            itemType = lotteryProto.type();
            showCard = setCardInfo(itemType,lotteryProto.itemid(),lotteryProto.num());
            if (showCard)
            {
                if (i == 0)
                {
                    progressCard = showCard;
                }
                else
                {
                    _huoArray->addObject(showCard);
                }
            }
        }
   
        visit1->setEnabled(true);
        refreshOwens();//服务器保证 先更新playerInfo void SGMainLayer::updatePlayerInfo(cocos2d::CCObject *sender)
        
        SGVisitLayer::outType = 2;

        
        SGMainManager::shareMain()->showVisitCards(_array,_huoArray,comboCard,progressCard, true,false , propBuyCard);
        
        comboCard = NULL;
        progressCard = NULL;
        _array = NULL;
        _huoArray = NULL;
   }
}

SGBaseMilitaryCard *SGVisitLayer::setCardInfo(int itemType,int cardId,int num)
{
    SGBaseMilitaryCard * showCard = NULL;
    switch (itemType) {
        case 6:
        {
            //武将
            SGOfficerCard *officerCard = new SGOfficerCard();
            showCard = officerCard;
            CCDictionary *dicc = SGCardAttribute::getValue(1, cardId);
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(cardId);
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(cardId);
            
//            if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, 1+1);
            officerCard->setCanAddNum(num);
            officerCard->setItemId(cardId);
            officerCard->setCurrExp(0);
            //officerCard->setLordId(lotteryProto.lordskillid());
            officerCard->setIsProt(0);
            officerCard->setCurrLevel(1);
            
            if (temp->getOfficerNextId()) {
                officerCard->setCost(temp->getOfficerCost());
                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
            }
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
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
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            officerCard->setFateList(temp->getOfficerFateList());
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
            showCard = equipCard;
            equipCard->setCanAddNum(num);
            CCDictionary *dicc = SGCardAttribute::getValue(1, cardId);
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardId);
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardId);
//            if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, 1+1);
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            equipCard->setItemId(cardId);
            
            equipCard->setCurrLevel(1);
            equipCard->setCurrExp(0);
            equipCard->setOfficerCardId(0);
            equipCard->setIsProt(0);
            
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
        case 8:
        {
            //士兵
        }
            break;
        case 9:
        {
            //道具
            SGPropsCard *propCard = new SGPropsCard();
            
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardId);
            
            //propCard->setSsid(cardInfo.ssid());
            propCard->setItemId(cardId);
            propCard->setIsProt(0);
            
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
            propCard->setHeadSuffixNum(prop->getIconId());//12.05
            propCard->setCurNum(num);
            
            showCard = propCard;
            propCard->autorelease();

        }
            break;
        case 10:
        {
            //武将碎片
			SGPiecesCard *piecesCard = new SGPiecesCard();

			SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(cardId);
            if (pieces) {
                showCard = piecesCard;
                piecesCard->setItemId(cardId);//碎片id
                piecesCard->setCanAddNum(num);
                piecesCard->setPiecesCurNum(num);// 碎片数量
                
                piecesCard->setSell(pieces->getPiecesPrice());//价格
                piecesCard->setOfficerName(pieces->getPiecesName());//名称
                piecesCard->setPiecesName(pieces->getPiecesName());
                piecesCard->setItemType(itemType);//类型
                piecesCard->setCurrLevel(pieces->getStarLevel());//星级
                piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
                piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
                piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
                piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
                piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
                piecesCard->setCurrStar(pieces->getStarLevel());

            }
            piecesCard->autorelease();
        }
            break;
        case 15:
        {
            //消耗物品
            SGConsumableCard *consumeCard = new SGConsumableCard();
			SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(cardId);
			
            if (consumeModel) {
                
                consumeCard->setCurrNum(num);
                
                consumeCard->setItemId(cardId);
                
                consumeCard->setSell(consumeModel->getConsumePrice());
                consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
                consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
                consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
                consumeCard->setOfficerName(consumeModel->getConsumeName());
                consumeCard->setMoneyType(consumeModel->getMoneyType());
                consumeCard->setItemType(consumeModel->getConsumeType());
                consumeCard->setHeadSuffixNum(consumeModel->getIconId());
                consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
                consumeCard->setPicId(consumeModel->getConsumePicId());
                
                
                showCard = consumeCard;
                
            }
			consumeCard->autorelease();

        }
            break;
        case 17:
        {
            SGMaterialCard *materialCard = new SGMaterialCard();
			SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(cardId);
			
            if (materialModel)
            {
                showCard = materialCard;
                materialCard->setItemId(cardId);
                
                materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
                materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
                materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
                materialCard->setOfficerName(materialModel->getMaterialName());
                materialCard->setSell(materialModel->getMaterialSoldPrice());
            }
			materialCard->autorelease();
        }
            break;
        default:
            break;
    }
    return showCard;
}


void SGVisitLayer::setIsCanTouch(bool isTouch)
{
    _touch = isTouch;
    SGBaseTableLayer::setIsCanTouch(isTouch);
    tableView->reloadData(false);
}
void SGVisitLayer::setVisitActivity(std::vector<VisitActivity>& visitActivityList)
{
    int size = visitActivityList.size();
    for (int i= 0; i<size; i++) {
        VisitActivity visitActivity;
        visitActivity.subType = visitActivityList[i].subType;
        visitActivity.starTime = visitActivityList[i].starTime;
        visitActivity.endTime = visitActivityList[i].endTime;
        visitActivity.description = visitActivityList[i].description;
        m_visitActivityList.push_back(visitActivity);
    }
}

VisitActivity* SGVisitLayer::getVisitActivityByTyew(int subType)
{

    int size = m_visitActivityList.size();
    for (int i= 0; i<size; i++) {
        if (m_visitActivityList[i].subType == subType) {
            return &m_visitActivityList[i];
        }
    }
    return NULL;
}

long SGVisitLayer::getCurrentTime()
{
    struct cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec;
}
void SGVisitLayer::refreshHuoDongListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ActivityLotteryEntryResponse *response = (main::ActivityLotteryEntryResponse *)sr->m_msg;
    singleCost = response->single();
    comboCost = response->combo();
    int visitViewId = response->plistid();
    CCSize layerSize = tableView->getContentSize();
    float scaleRate = layerSize.width/huoDongBg->getContentSize().width;
    canVisitHuodong = false;

    SGVisitViewVO* visitViewData =  SGStaticDataManager::shareStatic()->getVisitViewModel(visitViewId);
    if (visitViewData == NULL)
    {
        visit1->setEnabled(false);
        visit1->setVisible(false);
        
//        visit10->setEnabled(false);
//        visit10->setVisible(false);
        
        qidai = SGCCLabelTTF::create(str_TongQueLayer_str36, FONT_BOXINFO, FONTSIZE(60));
        qidai->setPosition(ccp(layerSize.width*0.8, layerSize.height*0.6 * scaleRate  - huoDongBg->getContentSize().height * scaleRate * 0.75));
        addChild(qidai,1234,1234);
        qidai->setRotation(315);
        qidai->setVisible(false);
        if (huoDong) {
            huoDong->stopAllActions();
            huoDong->setVisible(false);
        }
        showNormalVisit(NULL);
        return;
    }
    
    if (huoDongLayer == NULL)
    {
        return;
    }
    if (huoDong) {
        huoDong->setVisible(true);
    }
    
    canVisitHuodong = true;
    if (huoDongBg) {
        huoDongBg->removeFromParent();
        huoDongBg = NULL;
    }
    
    std::string bgplist(visitViewData->viistViewPic->getCString());
    std::string bgplistName =  std::string("sanguobigpic/") + bgplist.substr(0,bgplist.find_first_of(".")) + std::string(".plist");
    ResourceManager::sharedInstance()->bindTexture(bgplistName.c_str(), RES_TYPE_LAYER_UI, sg_visitLayer);
    
    huoDongBg = CCSprite::createWithSpriteFrameName(visitViewData->viistViewPic->getCString());
    if (huoDong == NULL)
    {
        huoDongBg = CCSprite::createWithSpriteFrameName("visitHuoBg.png");
    }
    
    huoDongBg->setPosition(ccp(layerSize.width/2, layerSize.height+4));
    huoDongBg->setAnchorPoint(ccp(0.5, 1));
    
    huoDongBg->setScale(scaleRate);
    huoDongBg->ignoreAnchorPointForPosition(false);
    huoDongLayer->addChild(huoDongBg);
    
    if (huoDongLableBg) {
        huoDongLableBg->setScaleX(layerSize.width/huoDongLableBg->getContentSize().width);
        float gap = layerSize.height -  huoDongBg->getContentSize().height/2 - huoDongLableBg->getContentSize().height;
        gap = gap > 0 ? (1 + gap/huoDongLableBg->getContentSize().height) : 1;
        huoDongLableBg->setScaleY(gap);
    }
    
    SGStaticDataManager * dataM = SGStaticDataManager::shareStatic();
    CCDictionary * dict = dataM->getDisplayValueDict();
    
    CCDictionary * one = (CCDictionary *) dict->objectForKey("49");
    int oneCost = one->valueForKey("value")->intValue();

    visit1->setPosition(ccp(layerSize.width/2, layerSize.height - huoDongBg->getContentSize().height * scaleRate * 0.75 + visit1->getContentSize().height));
    singleVisit1->setPosition(ccpAdd(visit1->getPosition(), ccp(28, -visit1->getContentSize().height*0.46))  );
    yb1->setPosition(ccpAdd(visit1->getPosition(), ccp(6*3-2, 0)));
    singleVisitMoney1->setString(CCString::createWithFormat("%d",oneCost)->getCString());
    singleVisitMoney1->setPosition(ccpAdd(yb1->getPosition(), ccp(yb1->getContentSize().width + 5, 0)));
    
//    visit10->setPosition(ccp(layerSize.width/2 + visit10->getContentSize().width*0.6, layerSize.height - huoDongBg->getContentSize().height * scaleRate *0.75 + visit10->getContentSize().height));
//    singleVisit10->setPosition(ccpAdd(visit10->getPosition(), ccp(28, -visit10->getContentSize().height*0.46))  );
//    yb10->setPosition(ccpAdd(visit10->getPosition(), ccp(6*3+10, 0)));
//    singleVisitMoney10->setString(CCString::createWithFormat("%d",comboCost)->getCString());
//    singleVisitMoney10->setPosition(ccpAdd(yb10->getPosition(), ccp(yb10->getContentSize().width+10, 4)));
    
    if (huoDongLable) {
        huoDongLable->setPosition(ccp(0, layerSize.height - huoDongBg->getContentSize().height * scaleRate));
    }
    
    int swiftX = 0;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (winSize.width > 640)
    {
        swiftX = 40;
    }
    if (scrollview2) {
        CCLayer *la =  CCLayer::create();
        SGCCLabelTTF *detail = NULL;
        CCArray *detailArry = visitViewData->getModelMsg();
        int detailIndex = detailArry->count()-1;
        float posY = 0;
        
        for (int i = detailIndex; i>=0; i--) {
            detail = SGCCLabelTTF::create(((CCString *)detailArry->objectAtIndex(i))->getCString(), FONT_BOXINFO, 32);
            la->addChild(detail);
            detail->setPosition(ccp(swiftX + detail->getContentSize().width/2,posY + detail->getContentSize().height*0.6));
            posY += 40;
        }
        
        la->setContentSize(CCSizeMake(layerSize.width, posY));
        scrollview2->setContainer(la);
        scrollview2->setContentOffset(ccp(0,huoDongLable->getPositionY() - huoDongLable->getContentSize().height - SGMainManager::shareMain()->getBottomHeight()*0.3 - la->getContentSize().height ));
    }
    haveRequestHuoVisit = true;
    this->showHuoDongVisit(NULL);
}

void SGVisitLayer::refreshOwens()
{
    if (havegold && havejg) {
        havegold->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString());
        havejg->setString(CCString::createWithFormat(" %d",SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString());
    }
}



