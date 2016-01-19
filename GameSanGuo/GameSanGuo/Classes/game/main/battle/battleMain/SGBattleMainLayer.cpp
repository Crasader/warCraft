//
//  SGBattleMainLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#include "SGBattleMainLayer.h"
#include "SGMainManager.h"
#include "SGBMatchStartBox.h"
#include "SGBattleRankLayer.h"
#include "SGBattleScoreLayer.h"

#include "SGTestSwitch.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PvpFight.pb.h"
#include "ShopModule.pb.h"
#include "Utils.h"
#include "SGCCLabelTTF.h"
#include "SGTestSwitch.h"
#include "SGStaticDataManager.h"
#include "SGCantAdvanceBox.h"
#include "ResourceManager.h"
#include "SGTeamgroup.h"
#include "SGHelpBookInfo.h"
#include "PvpFight.pb.h"
#include "RankModule.pb.h"
#include  "SGFirstLayer.h"
#include "SGStringConfig.h"
#include "RoleShopModule.pb.h" //新商城消息
#include "SGMallLayer.h"


CCSize size;
CCPoint center;
float headH = 0;
float bottomH = 0;
float viewWid = 0;
float discH = 0;
float i4discH = 0;
float i5discH = 0;

#define INFOGAP 16
SGBattleMainLayer::SGBattleMainLayer(void)
: _selectIndex(0), // 选中的按钮
battleData(NULL),
ary(NULL),
_ttjfv(NULL),
_nJifen(NULL),
m_jieshu(NULL)
{

}
SGBattleMainLayer::~SGBattleMainLayer(void)
{
 
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SEASONWAR_RANK);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SEASONWAR_CDLIST);
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVP_EVERYDAY);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVP_SEASON);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PVP_LAST_LIST);
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RANK);
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleMainLayer);
}

SGBattleMainLayer* SGBattleMainLayer::create(SGBattleData *data)
{
    SGBattleMainLayer *layer = new SGBattleMainLayer();
    if (layer && layer->init(NULL, sg_battleMainLayer)) {
        layer->battleData = data;
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGBattleMainLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_battleMainLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_battleMainLayer, LIM_PNG_AS_PNG);//灰背景下面的两个角

    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_battleMainLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_battleMainLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfirstbutton/sgfirstbutton.plist", RES_TYPE_LAYER_UI, sg_battleMainLayer);
    viewWid = 600;
    center = SGLayout::getPoint(kMiddleCenter);
    size = CCDirector::sharedDirector()->getWinSize();
    headH = SGMainManager::shareMain()->getVisitHgt();
    bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_SEASONWAR_RANK, this, callfuncO_selector(SGBattleMainLayer::showRankLayer));
    notification->addObserver(MSG_SEASONWAR_CDLIST, this, callfuncO_selector(SGBattleMainLayer::showChgScoreLayer));
//    notification->addObserver(MSG_PVP_EVERYDAY, this, callfuncO_selector(SGBattleMainLayer::showpvpEverydayLayer));
    notification->addObserver(MSG_PVP_SEASON, this, callfuncO_selector(SGBattleMainLayer::showSeasonListLayer));
    notification->addObserver(MSG_PVP_LAST_LIST, this, callfuncO_selector(SGBattleMainLayer::showLastListlayer));
//    notification->addObserver(MSG_RANK, this, callfuncO_selector(SGFirstLayer::rankListener));
    
    SGPlayerInfo::sharePlayerInfo()->setPvpCredit(battleData->getScore());
    
    ary = CCArray::create();
    for (int i =0;i<5;i++) {
        SGJunbuff *jun = SGStaticDataManager::shareStatic()->getjindutiaobyid(i);
        ary->addObject(jun);
    }
    
    this->initTitle();
    this->initSeasonTimeView();
    this->initViewBgs();
    
    //this->setlianshengjiangli();

    
    
}

void SGBattleMainLayer::setlianshengjiangli()
{

    SGJunbuff *buf = (SGJunbuff*)ary->objectAtIndex(4);
    int maxchang = buf->getMax();
    int x=battleData->getLiansheng();
    if(x>maxchang)
        x=maxchang;
    CCSprite *bg = (CCSprite*)this->getChildByTag(100);
    
    for (int i=0;i<5;i++) {
        SGJunbuff *buf = (SGJunbuff*)ary->objectAtIndex(i);
         CCProgressTimer * p = (CCProgressTimer*)bg->getChildByTag(200+i);
        if (x < buf->getMin()) {
           
            p->setPercentage(0);
        }
        else if (x>=buf->getMax())
        {
           p->setPercentage(100);
        }
        else
        {
            float rate = (float)(x-buf->getMin())/(buf->getMax()-buf->getMin());
            p->setPercentage(rate*100);
        }
        if (p->getPercentage()<100) {
            CCSprite *chang = (CCSprite*)bg->getChildByTag(300+i);
            if (chang) {
                CCSprite *s = (CCSprite*)chang->getChildByTag(0);
                s->setDisplayFrame(CCSprite::createWithSpriteFrameName("addg.png")->displayFrame());
                s = (CCSprite*)chang->getChildByTag(2);
                s->setDisplayFrame(CCSprite::createWithSpriteFrameName("scoreg.png")->displayFrame());
                SGCCLabelTTF* lab = (SGCCLabelTTF*)chang->getChildByTag(1);
                lab->setInsideColor(ccGRAY);
            }
        }
        
    }
}

void SGBattleMainLayer::initViewBgs()
{

    CCSprite *gbg = (CCSprite *)this->getChildByTag(100);
    CCPoint viewPos = ccp(gbg->getPositionX(), gbg->getPositionY()-gbg->getContentSize().height);
    CCPoint temp = ccpSub(viewPos, ccp(0,bottomH));
    CCPoint buttomcenter = ccp(temp.x,temp.y/2+bottomH);
    CCSize vSize = CCSizeMake(size.width,temp.y);

    CCSprite* bgbtn = CCSprite::createWithSpriteFrameName("dz.png");
    bgbtn->setPosition(ccpAdd(buttomcenter,ccp(0,0)));
    bgbtn->setOpacity(100);
    bgbtn->setScale(1.2);
    this->addChild(bgbtn);
    
    //参战button
    SGButton *wycz = SGButton::create("pvpjoin.png",NULL, this, menu_selector(SGBattleMainLayer::joininAction),CCPointZero,false,true);        wycz->setPosition(ccpAdd(buttomcenter,ccp(0,-vSize.height/3)));
        this->addBtn(wycz);

    SGButton *gzsm = SGButton::createFromLocal("an.png", str_BattleMainLayer_str1, this, menu_selector(SGBattleMainLayer::getRankAction),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    gzsm->setPosition(ccpAdd(buttomcenter,ccp(200,0)));
    gzsm->setOutsideColor(ccWHITE);
    this->addBtn(gzsm);
    
    SGButton *sjbd = SGButton::createFromLocal("an.png", str_BattleMainLayer_str2, this, menu_selector(SGBattleMainLayer::shangjiebangdan),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    sjbd->setPosition(ccpAdd(buttomcenter,ccp(-200,0)));
    sjbd->setOutsideColor(ccWHITE);
    this->addBtn(sjbd);
    
    SGButton *sjjl = SGButton::createFromLocal("an.png", str_BattleMainLayer_str3, this, menu_selector(SGBattleMainLayer::saijijiangli),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    sjjl->setPosition(ccpAdd(buttomcenter,ccp(-200,vSize.height/3-30)));
    sjjl->setOutsideColor(ccWHITE);
    this->addBtn(sjjl);
    
    //如果每日奖励可领取  则显示叹号
    if (battleData->getHasSeasonReward() )
    {
        CCSprite * spriteNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
        spriteNotice->setPosition( ccp(sjjl->getContentSize().width - 8 , sjjl->getContentSize().height-8 ) );
        sjjl->addChild(spriteNotice);
    }

    
    SGButton *jfhj = SGButton::createFromLocal("an.png", str_BattleMainLayer_str4, this, menu_selector(SGBattleMainLayer::chgScoreAction),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    jfhj->setPosition(ccpAdd(buttomcenter,ccp(0,vSize.height/3-30)));
    jfhj->setOutsideColor(ccWHITE);
    this->addBtn(jfhj);
    
    SGButton *mrjl = SGButton::createFromLocal("an.png", str_BattleMainLayer_str5, this, menu_selector(SGBattleMainLayer::meirijiangli),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    mrjl->setPosition(ccpAdd(buttomcenter,ccp(200,vSize.height/3-30)));
    mrjl->setOutsideColor(ccWHITE);
    this->addBtn(mrjl);

    //如果每日奖励可领取  则显示叹号
    if (battleData->getHasDayReward())
    {
        CCSprite * spriteNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
        spriteNotice->setPosition( ccp(mrjl->getContentSize().width - 8 , mrjl->getContentSize().height-8 ) );
        mrjl->addChild(spriteNotice);
    }
    
    
    
    SGCCLabelTTF *tiantiMsgLable = SGCCLabelTTF::create(CCString::createWithFormat(str_PvpMainCell_str2,battleData->getStartTime()->getCString(),
                                                                                   battleData->getEndTime()->getCString())->getCString(), FONT_PANGWA, 26);
    this->addChild(tiantiMsgLable);
    tiantiMsgLable->setPosition(ccp(jfhj->getPosition().x, jfhj->getPosition().y + jfhj->getContentSize().height));

}

void SGBattleMainLayer::saijijiangli()
{
    main::PvPSeasonAwardGiftEntryRequest *request = new main::PvPSeasonAwardGiftEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_PVP_SEASON, request);
}
void SGBattleMainLayer::meirijiangli()
{
    main::PvPDayAwardGiftEntryRequest *request = new main::PvPDayAwardGiftEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_PVP_EVERYDAY, request);
}
void SGBattleMainLayer::shangjiebangdan()
{
    if (m_jieshu > 1)
    {
        main::PvpOldRankRequest *request = new main::PvpOldRankRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_PVP_LAST_LIST, request);
    }
    else
    {
        SGMainManager::shareMain()->showMessage(str_BattleMainLayer_str18);
    }
}

void SGBattleMainLayer::helphandle()
{
    SGHelpTitleDataModel *tit = new SGHelpTitleDataModel;
    tit->setbeg(1601);
    tit->setend(1627);
    tit->setnum(27);
    CCString *str = CCString::create(str_BattleMainLayer_str6);
    tit->settitle(str);
    
    SGHelpBookInfo *info = SGHelpBookInfo::create(tit,2);
    SGMainManager::shareMain()->showLayer(info);
    delete tit;

}

void SGBattleMainLayer::initTitle()
{
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_battleMainLayer);


    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, stdSize.height/2-(size.height-headH-bottomH)/2, bgImg->getContentSize().width, size.height-headH-bottomH);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setTextureRect(rect);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -5);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("upwar_title_ss_n.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(CCString::createWithFormat(str_BattleMainLayer_str7,battleData->getJieshu())->getCString(), FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));
    this->addChild(title);
    m_jieshu = battleData->getJieshu();

    
    CCLayerColor *greybg = CCLayerColor::create(ccc4(0,0,0,100), size.width,size.height*.25);
    greybg->setPosition(ccpAdd(title_bg->getPosition(),ccp(0, -title_bg->getContentSize().height)));
    greybg->ignoreAnchorPointForPosition(false);
    greybg->setAnchorPoint(ccp(0.5,1));
    this->addChild(greybg,2,100);
    
    CCSprite *greybg_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
//    title_bg_r->setFlipX(true);
    greybg_l->setFlipY(true);
    greybg_l->setAnchorPoint(ccp(0, 0));
    greybg_l->setPosition(ccpAdd(greybg->getPosition(), ccp(-greybg->getContentSize().width/2,  -greybg->getContentSize().height)));
    this->addChild(greybg_l,5);
    
    CCSprite *greybg_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    greybg_r->setFlipX(true);
    greybg_r->setFlipY(true);
    greybg_r->setAnchorPoint(ccp(1, 0));
    greybg_r->setPosition(ccpAdd(greybg->getPosition(), ccp(greybg->getContentSize().width/2,  -greybg->getContentSize().height)));
    this->addChild(greybg_r,5);
    
    CCSprite *greybg_m = CCSprite::createWithSpriteFrameName("pvpline.png");
    greybg_m->setScaleX(4);
    greybg_m->setAnchorPoint(ccp(0.5,0));
    greybg_m->setPosition(ccpAdd(greybg->getPosition(),ccp(0,-greybg->getContentSize().height)));
    this->addChild(greybg_m,4);
    
}

void SGBattleMainLayer::initSeasonTimeView()
{
    CCSprite *gbg = (CCSprite *)this->getChildByTag(100);

    CCSize vSize = gbg->getContentSize();

    float fSize = 26;
    const char *fName = FONT_PANGWA;


    float disX = 20;


   
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    SGCCLabelTTF *sdesc = SGCCLabelTTF::create(str_BattleMainLayer_str8, fName, fSize , ccYELLOW);


    sdesc->setAnchorPoint(ccp(0, 0.5f));
    sdesc->setPosition(ccp(disX, vSize.height - vSize.height/5));
    gbg->addChild(sdesc);
    
    SGCCLabelTTF *svalue = SGCCLabelTTF::create(CCString::createWithFormat("%s",battleData->getTimeDesc())->getCString(), fName, fSize , ccWHITE);


    svalue->setAnchorPoint(ccp(0, 0.5f));
    svalue->setPosition(ccpAdd(sdesc->getPosition(), ccp(26*5, 0)));
    gbg->addChild(svalue);

    
    SGButton * buttonRule = SGButton::create("battle_rules.png", NULL, this, menu_selector(SGBattleMainLayer::helphandle),CCPointZero,false,true);
    buttonRule->setAnchorPoint(ccp(0, 0.5f));
    buttonRule->setPosition(ccp(winSize.width - buttonRule->getContentSize().width - disX,svalue->getPosition().y));
    
    ///天梯声望
    SGCCLabelTTF *dqjf = SGCCLabelTTF::create(str_BattleMainLayer_str12, fName, fSize , ccYELLOW);
    //modify by:zyc. merge into create.
    //dqjf->setColor(ccYELLOW);
    dqjf->setAnchorPoint(ccp(0, 0.5f));
    dqjf->setPosition(ccp(disX, vSize.height -vSize.height*2/5));
    gbg->addChild(dqjf);
    
    CCString *string2 = CCString::createWithFormat("%d", battleData->getShengwang());
    SGCCLabelTTF *lvalue2 = SGCCLabelTTF::create(string2->getCString(), fName, fSize , ccWHITE);
    //modify by:zyc. merge into create.
    //lvalue2->setColor(ccWHITE);
    lvalue2->setAnchorPoint(ccp(0, 0.5f));
    lvalue2->setPosition(ccpAdd(dqjf->getPosition(), ccp(26*5, 0)));
    gbg->addChild(lvalue2);
    
    ///当前军粮
    SGCCLabelTTF *tianiJunliang = SGCCLabelTTF::create(str_BattleMainLayer_str19, fName, fSize , ccYELLOW);
    //modify by:zyc. merge into create.
    //dqjf->setColor(ccYELLOW);
    tianiJunliang->setAnchorPoint(ccp(0, 0.5f));
    tianiJunliang->setPosition(ccpAdd(dqjf->getPosition(), ccp(winSize.width * 0.4,0)));
    gbg->addChild(tianiJunliang);
    
    CCSprite * junliangPayKind = CCSprite::createWithSpriteFrameName("mail_food.png");
    junliangPayKind->setPosition(ccpAdd(tianiJunliang->getPosition(), ccp(26*5 + 4, 0)) );
    gbg->addChild(junliangPayKind,10);
    junliangPayKind->setScale(0.4);

    int limitJunliang = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("13"))->objectForKey("value"))->intValue();
    CCString *stringjunliang = CCString::createWithFormat("%d/%d", SGPlayerInfo::sharePlayerInfo()->getPvpPower(),limitJunliang);
    junliangvalue2 = SGCCLabelTTF::create(stringjunliang->getCString(), fName, fSize , ccWHITE);
    //modify by:zyc. merge into create.
    //lvalue2->setColor(ccWHITE);
    junliangvalue2->setAnchorPoint(ccp(0, 0.5f));
    junliangvalue2->setPosition(ccpAdd(tianiJunliang->getPosition(), ccp(26*6, 0)));
    gbg->addChild(junliangvalue2);
    
    
    
    
    ////声望排名
    SGCCLabelTTF *swpj = SGCCLabelTTF::create(str_BattleMainLayer_str13, fName, fSize , ccYELLOW);
    //modify by:zyc. merge into create.
    //swpj->setColor(ccYELLOW);
    swpj->setAnchorPoint(ccp(0, 0.5f));
    swpj->setPosition(ccp(disX, vSize.height - vSize.height*3/5));
    gbg->addChild(swpj);
    
    
    if (battleData->getRankshengwang() < 1) {
        string2 = CCString::createWithFormat("%s", str_BattleMainLayer_str17);
    }
    else
    {
        string2 = CCString::createWithFormat("%d", battleData->getRankshengwang());
    }
    SGCCLabelTTF *swpmv = SGCCLabelTTF::create(string2->getCString(), fName, fSize , ccWHITE);
    //modify by:zyc. merge into create.
    //swpmv->setColor(ccWHITE);
    swpmv->setAnchorPoint(ccp(0, 0.5f));
    swpmv->setPosition(ccpAdd(swpj->getPosition(), ccp(26*5, 0)));
    gbg->addChild(swpmv);
    
    
    ////天梯积分
    SGCCLabelTTF *ttjf = SGCCLabelTTF::create(str_BattleMainLayer_str11, fName, fSize , ccYELLOW);
    //modify by:zyc. merge into create.
    //ttjf->setColor(ccYELLOW);
    ttjf->setAnchorPoint(ccp(0, 0.5f));
    ttjf->setPosition(ccpAdd(swpj->getPosition(), ccp(winSize.width * 0.4,0)));
    gbg->addChild(ttjf);
    
    CCSprite * tianTiPayKind = CCSprite::createWithSpriteFrameName("store_tianti.png");
    tianTiPayKind->setPosition(ccpAdd(ttjf->getPosition(), ccp(26*5 + 4, 0)) );
    gbg->addChild(tianTiPayKind,10);
    

    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("2");
    _nJifen=dic1->valueForKey("value")->intValue();
    
    CCString *string = CCString::createWithFormat("%d/%d", battleData->getScore(),_nJifen);
    _ttjfv = SGCCLabelTTF::create(string->getCString(), fName, fSize , ccWHITE);
    //modify by:zyc. merge into create.
    //_ttjfv->setColor(ccWHITE);
    _ttjfv->setAnchorPoint(ccp(0, 0.5f));
    _ttjfv->setPosition(ccpAdd(ttjf->getPosition(), ccp(26*6, 0)));
    gbg->addChild(_ttjfv);
    
    
    
    ////当前胜率
    SGCCLabelTTF *tdesc = SGCCLabelTTF::create(str_BattleMainLayer_str9, fName, fSize , ccYELLOW);
    //modify by:zyc. merge into create.
    //tdesc->setColor(ccYELLOW);
    tdesc->setAnchorPoint(ccp(0, 0.5f));
    tdesc->setPosition(ccp(disX, vSize.height - vSize.height*4/5));
    gbg->addChild(tdesc);
    

    string = CCString::createWithFormat("%.2f%%", battleData->getWinrate());
    SGCCLabelTTF *lvalue = SGCCLabelTTF::create(string->getCString(), fName, fSize , ccWHITE);
    //modify by:zyc. merge into create.
    //lvalue->setColor(ccWHITE);
    lvalue->setAnchorPoint(ccp(0, 0.5f));
    lvalue->setPosition(ccpAdd(tdesc->getPosition(), ccp(26*5, 0)));
    gbg->addChild(lvalue);
    
    
    ////当前军衔
    SGCCLabelTTF *junx = SGCCLabelTTF::create(str_BattleMainLayer_str14, fName, fSize , ccYELLOW);
    //modify by:zyc. merge into create.
    //junx->setColor(ccYELLOW);
    junx->setAnchorPoint(ccp(0, 0.5f));
    junx->setPosition(ccpAdd(tdesc->getPosition(), ccp(winSize.width * 0.4, 0)));
    gbg->addChild(junx);
    
    string2 = CCString::createWithFormat("%s", (battleData->getJunxian()));
    SGCCLabelTTF *jvalue = SGCCLabelTTF::create(string2->getCString(), fName, fSize*1.5);
    jvalue->setAnchorPoint(ccp(0, 0.5f));
    jvalue->setPosition(ccpAdd(junx->getPosition(), ccp(26*5, 0)));
    gbg->addChild(jvalue);
    
    //////////////
    SGButton *btn = SGButton::create("questionpic.png", NULL, this, menu_selector(SGBattleMainLayer::jxbuff),CCPointZero,false,true);
//    btn->setPosition(ccpAdd(gbg->getPosition(), ccp(-vSize.width/2+jvalue->getPosition().x+jvalue->getContentSize().width+btn->getContentSize().width/2, -vSize.height+jvalue->getPosition().y)));
    btn->setPosition(ccpAdd(jvalue->getPosition(),ccp(jvalue->getContentSize().width+btn->getContentSize().width/2,0)));
//    this->addBtn(btn);
    CCMenu* menugrey = CCMenu::create();//因为是cclayercolor,所以只能在上面加按键才不会被cclayercolor遮住
    menugrey->setPosition(ccp(0,0));
    gbg->addChild(menugrey,2);
    menugrey->addChild(btn);
    
    //本函数内，上边。  添加规则图片button。buttonRule。
    menugrey->addChild(buttonRule);

}

//void SGBattleMainLayer::showpvpEverydayLayer(CCObject *sender)
//{
//    //0:金子 1:铜钱 2:天梯积分  5:炼丹秘方 6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌  10:碎片 11军魂 12体力 13军粮 14: 15消耗品 16 勋章 17 材料
//    SGSocketRequest *sr = (SGSocketRequest *)sender;
//    main::PvPDayAwardGiftEntryResponse *response = (main::PvPDayAwardGiftEntryResponse *)sr->m_msg;
//    if (sr)
//    {
//        CCArray *arrary = CCArray::create();
//        
//        int len = response->list_size();
//        int count = response->fightcounter();
//        for (int i = 0 ; i <len; i++) {
//            SGVipGift *vipGift = new SGVipGift();
//            main::PvPDayAwardGiftBag gifts = response->list(i);
//            vipGift->setGiftName(CCString::create(gifts.name()));
//            vipGift->setGiftInfo(CCString::create(gifts.info()));
//            vipGift->setGiftDiscount(gifts.giftcostdiscount());
//            vipGift->setPvpgifttype(gifts.count());
//            vipGift->setGiftCoin(gifts.giftcoin());
//            vipGift->setGiftAward(gifts.giftaward());
//            vipGift->setGiftSpeak(gifts.speaker());
//            vipGift->setGiftLiandan(gifts.liandan());
//            vipGift->setHaveBuy(gifts.isbuy());
//
//            int lenth = gifts.list_size();
//            for (int j = 0; j <lenth; j ++) {
//                SGGift *gift = new SGGift;
//                main::PvPDayAwardGiftItem GiftItem = gifts.list(j);
//                gift->setItemId(GiftItem.itemid());
//                gift->setNum(GiftItem.num());
//                
//                
//                int type = GiftItem.itemtype();
//                SGBaseItemType enumType = (SGBaseItemType)type;
//                gift->setType(type);
//                switch (enumType)
//                {
//                    case BIT_INGOT:
//                        gift->setItemId(10000);
//                        gift->setName(CCString::create(str_Currency_YanBao));
//                        break;
//                    case BIT_GOLD: //coin
//                        gift->setItemId(9999);
//                        gift->setName(CCString::create(str_Currency_TongQian));
//                        break;
//                    case BIT_GRADE:// 天梯积分  图片名称 store_tianti.png 在public3中
//                        gift->setItemId(9995);
//                        gift->setName(CCString::create(str_Currency_TianTiJiFen));
//                        break;
//                    case BIT_RECIPE: //炼丹
//                        gift->setItemId(9996);
//                        gift->setName(CCString::create(str_liandianmifang));
//                        break;
//                        
//                    case BIT_SOUL://军魂
//                        gift->setItemId(9994);
//                        gift->setName(CCString::create(str_FriendMailLayer_str8));
//                        break;
//                    case BIT_VIGOUR: //体力
//                        gift->setItemId(9993);
//                        gift->setName(CCString::create(str_CantAdvance_tips64));
//                        break;
//                    case BIT_GRAIN: //军粮
//                        gift->setItemId(9992);
//                        gift->setName(CCString::create(str_CantAdvance_tips65));
//                        break;
//                    case BIT_MEDAL://勋章
//                        gift->setItemId(9991);
//                        gift->setName(CCString::create(str_Currency_XunZhang));
//                        break;
//                    case BIT_OFFICER:
//                    {
//                        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(GiftItem.itemid());
//                        gift->setItemId(temp->getOfficerId());
//                        
//                        //服务器不再发送， 从表中读取。
//                        gift->setName(temp->getOfficerName());
//                        gift->setStar(temp->getOfficerCurrStarLevel());
//                        gift->setAdvnum(temp->getAdvNum());
//                    }
//                        
//                        break;
//                    case BIT_EQUIP:
//                    {
//                        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(GiftItem.itemid());
//                        gift->setItemId(temp->getEquipmentId());
//                        
//                        //服务器不再发送， 从表中读取。
//                        gift->setName(temp->getEquipName());
//                        gift->setStar(temp->getEquipCurrStarLevel());
//                        gift->setAdvnum(temp->getAdvNum());
//                    }
//                        
//                        break;
//                    case BIT_PROP:
//                    {
//                        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(GiftItem.itemid());
//                        gift->setItemId(temp->getPropsId());
//                        
//                        //服务器不再发送， 从表中读取。
//                        gift->setName(temp->getPropsName());
//                        gift->setStar(temp->getPropsCurrStarLevel());
//                    }
//                        
//                        break;
//                    case BIT_CONSUMABLE:
//                    {
//                        SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(GiftItem.itemid());
//                        gift->setItemId(temp->getConsumId());
//                        
//                        //服务器不再发送， 从表中读取。
//                        gift->setName(temp->getConsumeName());
//                        gift->setStar(temp->getConsumeStarLvl());
//                    }
//                        break;
//                    case BIT_PIECE:
//                    {
//                        SGPiecesDataModel * temp = SGStaticDataManager::shareStatic()->getPiecesDictById(GiftItem.itemid());
//                        gift->setItemId(temp->getPiecesId());
//                        
//                        //
//                        gift->setStar(temp->getStarLevel());
//                        gift->setName(temp->getPiecesName());
//                    }
//                        break;
//                    case BIT_MATERIAL:
//                    {
//                        SGMaterialDataModel * temp = SGStaticDataManager::shareStatic()->getMaterialById(GiftItem.itemid());
//                        gift->setItemId(temp->getMaterialId());
//                        gift->setName(temp->getMaterialName());
//                    }
//                        break;
//                        
//                    default:
//                        CCAssert(false,"the value of type is wrong.");
//                        break;
//                }
//                
//                vipGift->addGiftList(gift);
//                gift->autorelease();
//            }
//
//            arrary->addObject(vipGift);
//            vipGift->autorelease();
//        }
//        arrary->retain();
////        SGMainManager::shareMain()->showGiftsLayer(vipLvl,_vipGifts,curr,max);
//        SGMainManager::shareMain()->showpvpEverydayLayer(count,arrary);
//        arrary->release();
//    }
//}

void SGBattleMainLayer::showSeasonListLayer(CCObject *sender)
{
    //0:金子 1:铜钱 2:天梯积分 3:军功 4:小喇叭 5:炼丹秘方 6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌  10:碎片 11军魂 12体力 13军粮 14: 15消耗品
    //请参考 sgbaseitemtype.h
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    main::PvPSeasonAwardGiftEntryResponse *response = (main::PvPSeasonAwardGiftEntryResponse *)sr->m_msg;
    if (sr)
    {
        CCArray *arrary = CCArray::create();
        
        int len = response->list_size();
        int count = response->seasoncount();
        for (int i = 0 ; i <len; i++) {
            SGVipGift *vipGift = new SGVipGift();
            main::PvPDayAwardGiftBag gifts = response->list(i);
            vipGift->setGiftName(CCString::create(gifts.name()));
            vipGift->setGiftInfo(CCString::create(gifts.info()));
            vipGift->setGiftCondition(CCString::create(gifts.custom() ) );
            vipGift->setGiftDiscount(gifts.giftcostdiscount());
            vipGift->setPvpgifttype(gifts.count());
            vipGift->setGiftCoin(gifts.giftcoin());
            vipGift->setGiftLiandan(gifts.liandan());
            vipGift->setHaveBuy(gifts.isbuy());

            int lenth = gifts.list_size();
            for (int j = 0; j <lenth; j ++)
            {
                SGGift *gift = new SGGift;
                main::PvPDayAwardGiftItem GiftItem = gifts.list(j);
                gift->setItemId(GiftItem.itemid());
                gift->setNum(GiftItem.num());
                //服务器 不再发送此消息
                int type = GiftItem.itemtype();
                SGBaseItemType enumType = (SGBaseItemType)type;
                gift->setType(type);
                switch (enumType)
                {
                    case BIT_INGOT:
                        gift->setItemId(10000);
                        gift->setName(CCString::create(str_Currency_YanBao));
                        break;
                    case BIT_GOLD: //coin
                        gift->setItemId(9999);
                        gift->setName(CCString::create(str_Currency_TongQian));
                        break;
                    case BIT_GRADE:// 天梯积分  图片名称 store_tianti.png 在public3中
                        gift->setItemId(9995);
                        gift->setName(CCString::create(str_Currency_TianTiJiFen));
                        break;
//                    case 3://废弃
//                        gift->setIconid(9997);
//                        gift->setType(type);
//                        gift->setName(CCString::create(str_Currency_JunGong));
//                        break;
//                    case 4: //废弃
//                        gift->setIconid(9998);
//                        gift->setType(type);
//                        gift->setName(CCString::create(str_Small_laba));
//                        break;
                    case BIT_RECIPE: //炼丹
                        gift->setItemId(9996);
                        gift->setName(CCString::create(str_liandianmifang));
                        break;
                        
                    case BIT_SOUL://军魂
                        gift->setItemId(9994);
                        gift->setName(CCString::create(str_FriendMailLayer_str8));
                        break;
                    case BIT_VIGOUR: //体力
                        gift->setItemId(9993);
                        gift->setName(CCString::create(str_CantAdvance_tips64));
                        break;
                    case BIT_GRAIN: //军粮
                        gift->setItemId(9992);
                        gift->setName(CCString::create(str_CantAdvance_tips65));
                        break;
                    case BIT_MEDAL://勋章
                        gift->setItemId(9991);
                        gift->setName(CCString::create(str_Currency_XunZhang));
                        break;
                    case BIT_OFFICER:
                    {
                        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(GiftItem.itemid());
                        gift->setItemId(temp->getOfficerId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getOfficerName());
                        gift->setStar(temp->getOfficerCurrStarLevel());
                        gift->setAdvnum(temp->getAdvNum());
                    }
                        
                        break;
                    case BIT_EQUIP:
                    {
                        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(GiftItem.itemid());
                        gift->setItemId(temp->getEquipmentId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getEquipName());
                        gift->setStar(temp->getEquipCurrStarLevel());
                        gift->setAdvnum(temp->getAdvNum());
                    }
                        
                        break;
                    case BIT_PROP:
                    {
                        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(GiftItem.itemid());
                        gift->setItemId(temp->getPropsId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getPropsName());
                        gift->setStar(temp->getPropsCurrStarLevel());
                    }
                        
                        break;
                    case BIT_CONSUMABLE:
                    {
                        SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(GiftItem.itemid());
                        gift->setItemId(temp->getConsumId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getConsumeName());
                        gift->setStar(temp->getConsumeStarLvl());
                    }
                        break;
                    case BIT_PIECE:
                    {
                        SGPiecesDataModel * temp = SGStaticDataManager::shareStatic()->getPiecesDictById(GiftItem.itemid());
                        gift->setItemId(temp->getPiecesId());
                        
                        //
                        gift->setStar(temp->getStarLevel());
                        gift->setName(temp->getPiecesName());
                    }
                        break;
                    case BIT_MATERIAL:
                    {
                        SGMaterialDataModel * temp = SGStaticDataManager::shareStatic()->getMaterialById(GiftItem.itemid());
                        gift->setItemId(temp->getMaterialId());
                        gift->setName(temp->getMaterialName());
                    }
                        break;
                        
                    default:
                        CCAssert(false,"the value of type is wrong.");
                        break;
                }

                vipGift->addGiftList(gift);
                gift->autorelease();
            }
            //            this->addGifts(vipGift);
            arrary->addObject(vipGift);
            vipGift->autorelease();
        }
        arrary->retain();
        //        SGMainManager::shareMain()->showGiftsLayer(vipLvl,_vipGifts,curr,max);
        SGMainManager::shareMain()->showSeasonListLayer(count,arrary);
        arrary->autorelease();
    }
}

void SGBattleMainLayer::showLastListlayer(CCObject *sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    main::PvpOldRankResponse *response = (main::PvpOldRankResponse *)sr->m_msg;
    if (sr)
    {
        int count = response->seasoncount();
        int length = response->rankinfos_size();
        CCArray *array = CCArray::create();
//        array->retain();
        
        for (int i=0;i<length;i++)
        {
            main::RankProto rank = response->rankinfos(i);
            std::string name = rank.nickname();
            int roleid = rank.roleid();
            int nation = rank.nation();
            int headid = rank.headid();
            int value =rank.value();
            int serverid = rank.areaid();
            std::string servername = rank.areaname();
            
            SGRankPlayer *player = SGRankPlayer::create(name.c_str(),headid,value,0,0,0,serverid,servername.c_str(),roleid,nation);
            array->addObject(player);
            
        }
        array->retain();
        array->autorelease();
        SGMainManager::shareMain()->showlastListLayer(count,array);
     
    }
}

void SGBattleMainLayer::lianshenginfo()
{
    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 105, 0,NULL);
    SGMainManager::shareMain()->showBox(cantadvanceBox);

}

void SGBattleMainLayer::jxbuff()
{
    SGMainManager::shareMain()->showJunxianLayer(true);
}


#pragma mark  cdhk
void SGBattleMainLayer::joininAction()
{
    EFFECT_PLAY(MUSIC_BTN);

    if (checkGovernNeed())
    {
        CCLOG("统御力超出");
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this, NULL, 39, NULL);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
        return;
    }
    
    int junliang = SGPlayerInfo::sharePlayerInfo()->getPvpPower();
    if (junliang > 0)// 客户端判断参战条件是否成功，军粮大于0
    {
        main::PvpApplyRequest *request = new main::PvpApplyRequest();
        int type = SGPlayerInfo::sharePlayerInfo()->getBattleType();
        int wait = SGPlayerInfo::sharePlayerInfo()->get_Wait();
        if (wait == 1)
        {
            SGMainManager::shareMain()->getMainScene()->showMessage(str_BattleMainLayer_str16);
            return;
        }
        CCLOG("type===%d",type);
        request->set_type(type);
        SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_APPLY, request);
    }
    else// 提示军粮不够
    {
        main::ShopPurchPvPPowerEntryRequest *request = new main::ShopPurchPvPPowerEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_PURCHPVPPOWE_RENTER, request);
    }
}
#pragma mark rclk
void SGBattleMainLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showPvpMainLayer();
}

#pragma mark tkwv ukrq t pltf rdtf
void SGBattleMainLayer::chgScoreAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_PVP,PVP_HOME_LAYER);
}

#pragma mark fcgl
void SGBattleMainLayer::getRankAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    // 0=shu, 1=wei, 2=wu, 3=kuafu, 4=benfu
//    main::PvpTopRequest *request = new main::PvpTopRequest();
//    request->set_page(1);
//    request->set_type(3);
//    
////    CCMenuItem *menu = (CCMenuItem *)sender;
////    _selectIndex = menu->getTag();
////    
////    switch (menu->getTag()) {
////        case rank_button_kua: {
////            request->set_type(3);
////            CCLOG("rank--------------rank_button_kua");
////        } break;
////        case rank_button_ben: {
////            request->set_type(4);
////            CCLOG("rank--------------rank_button_ben");
////        } break;
////        case rank_button_wei: {
////            request->set_type(1);
////            CCLOG("rank--------------rank_button_wei");
////        } break;
////        case rank_button_shu: {
////            request->set_type(0);
////            CCLOG("rank--------------rank_button_shu");
////        } break;
////        case rank_button_wu: {
////            request->set_type(2);
////            CCLOG("rank--------------rank_button_wu");
////        } break;
////        default:
////            break;
////    }
//    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_RANK, request);
//    
    
    main::RankRequest* req = new main::RankRequest();
    req->set_type(0);
    req->set_nationid(0);
    req->set_page(1);
    SGSocketClient::sharedSocketClient()->send(MSG_RANK, req);
    
    
    
}

void SGBattleMainLayer::showChgScoreLayer(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpShopListResponse *response = (main::PvpShopListResponse *)req->m_msg;
    if (response) {
        CCArray *datas = CCArray::create();
        int size = response->items_size();
        
        DDLog("credit main-------------------%d", size);
        for (int ii = 0; ii < size; ii++) {
            main::PvpShopItem item = response->items(ii);
            int var1 = item.uid();
            int var2 = item.pvplevelmin();
            int var3 = item.costscore();
            int var4 = item.currentcount();
            int var5 = item.maxcount();
            //int itemId = item.itemid();
            
            SGCreditCardModel *model = SGCreditCardModel::create(var1, var2, var3, var4, var5);
            // 按服务器的15个type走
            model->setModelType(item.itemtype());
            model->setDisCount(item.discountcostscore());
            model->setitemId(item.itemid());
            
            datas->addObject(model);
        }
        SGMainManager::shareMain()->showChgCreditCardLayer(datas, 0);
    }
}

void SGBattleMainLayer::showRankLayer(CCObject *sender)
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    if (mainScene->nowLayerTag == sg_rankLayer) {
        return; // 当前为排行界面，直接返回
    }
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpTopResponse *response = (main::PvpTopResponse *)req->m_msg;
    if (response) {
        CCArray *datas = CCArray::create();
        int size = response->players_size();
        
        CCLog("rank main-------------------%d", size);
        for (int ii = 0; ii < size; ii++) {
            main::TopPlayerMessage player = response->players(ii);
            // create(const char *name, int hid, int rank, int levl, int ftime, float rwin);
            const char* var1 = player.name().c_str();
            int var2 = player.headid();
            int var3 = player.toppos();
            int var4 = player.toplevel();
            int var5 = player.fighttimes();
            float var6 = player.winratio();
            SGRankPlayer *ranker = SGRankPlayer::create(var1, var2, var3, var4, var5, var6,0,NULL);//最后是服务器id和名字
            datas->addObject(ranker);
        }
        // 测试代码
        if (false && size == 0) {
            for (int ii = 0; ii < 100; ii++) {
                const char* var1 = CCString::createWithFormat("NAME%d", ii)->getCString();
                int var2 = ii%3 + 1;
                int var3 = ii+1;
                int var4 = 1;
                int var5 = 10;
                float var6 = 0.3f;
                SGRankPlayer *ranker = SGRankPlayer::create(var1, var2, var3, var4, var5, var6,0,NULL);//最后是服务器id和名字
                datas->addObject(ranker);
            }
        }
        
        SGMainManager::shareMain()->showRankLayer(datas,response->allcount(),1);
    }
}

bool SGBattleMainLayer::checkGovernNeed()
{
//    CCArray *array = SGPlayerInfo::sharePlayerInfo()->getEmbattleOfficers();
    int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(id);
    CCObject *obj = NULL;
    int mor = 0;
    int speed = 0;
    int totleGovern = 0;
    CCARRAY_FOREACH(array, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        mor += card->getMorale();
        speed += card->getSpeed();
        totleGovern += card->getGoverning();
        if (card->getOfficerEquips()) {
            for (int i = 0; i <card->getOfficerEquips()->count(); i++) {
                SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
                mor += equip->getMorale();
                speed += equip->getSpeed();
                if ((card->getGoverning()+equip->getGoverning())<0) {
                    totleGovern -= card->getGoverning();
                }
                else{
                    totleGovern += equip->getGoverning();
                }
            }
        }
    }
    
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    int allGovern=player->getPlayerGoverning()+SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering();
    return totleGovern >allGovern ? true:false;
}

void SGBattleMainLayer::refreshView()
{
    CCString *str = CCString::createWithFormat("%d/%d", SGPlayerInfo::sharePlayerInfo()->getplayerPvpNum() ,_nJifen);
    _ttjfv->setString(str->getCString() );
}

void SGBattleMainLayer::updateJunLiang(const char * str)
{
    junliangvalue2->setString(str);
}