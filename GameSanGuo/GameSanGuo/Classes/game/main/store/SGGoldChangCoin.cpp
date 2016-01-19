//
//  SGGoldChangCoin.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//
//

#include "SGGoldChangCoin.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGCantAdvanceBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGPlayerInfo.h"
#include "ShopModule.pb.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGGoldChangCoin::SGGoldChangCoin():maxChance(0)
,currChance(0)
,vipLvl(0)
,cost_gold(0)
,get_coin(0)
,label3(NULL)
,duihuan(NULL)
,label1(NULL)
{

}
SGGoldChangCoin::~SGGoldChangCoin()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_goldChangeCoin);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EXCHANGE_COINS);
    notification->removeObserver(this, MSG_STORE_ENTER);
}
SGGoldChangCoin *SGGoldChangCoin::create(int lvl,int curr,int max,int gold,int coin)
{
    SGGoldChangCoin *exchangeLayer = new SGGoldChangCoin();
    if (exchangeLayer && exchangeLayer->init(NULL, sg_exchangCoin))
    {
        exchangeLayer->initView(lvl,curr,max,gold,coin);
        exchangeLayer->autorelease();
        return exchangeLayer;
    }
    CC_SAFE_DELETE(exchangeLayer);
    return NULL;
}
void SGGoldChangCoin::initView(int lvl,int curr,int max,int costgold,int getcoin)
{
    currChance = curr;
    maxChance = max;
    vipLvl = lvl;
    cost_gold = costgold;
    get_coin = getcoin;
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_goldChangeCoin, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_goldChangeCoin);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_goldChangeCoin);
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_goldChangeCoin);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_goldChangeCoin);
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_goldChangeCoin);

	

//    CCSprite *bg = CCSprite::create("sanguobigpic/store_bg.png",CCRectMake(0, 1028 - (s.height-headhgt - 106 -56)*768/s.width, 768, (s.height-headhgt - 106 -56)*768/s.width));
//    this->addChild(bg,-1);
//    bg->setScale(s.width/768);
//    bg->setAnchorPoint(ccp(0.5, 0));
//    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));


    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EXCHANGE_COINS,
                              this,
                              callfuncO_selector(SGGoldChangCoin::ExchangeListener));
    notification->addObserver(MSG_STORE_ENTER,
                              this,
                              callfuncO_selector(SGGoldChangCoin::storeEnterListener));
    
//    CCSprite *titlebg = CCSprite::createWithSpriteFrameName("hk.png");
//    titlebg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + titlebg->getContentSize().height/2)));
//    this->addChild(titlebg);  

    CCSprite *hqbg = CCSprite::createWithSpriteFrameName("store_huanqianbg.png");
    this->addChild(hqbg);
    hqbg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -18)));
    
//    CCSprite *leftbian = CCSprite::createWithSpriteFrameName("hkhb.png");
//    leftbian->setPosition(ccp(leftbian->getContentSize().width/2, titlebg->getPosition().y));
//    this->addChild(leftbian);
//    
//    CCSprite *rightbian = CCSprite::createWithSpriteFrameName("hkhb.png");
//    rightbian->setPosition(ccp(s.width - rightbian->getContentSize().width/2, leftbian->getPosition().y));
//    rightbian->setFlipX(true);
//    this->addChild(rightbian);
//    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("title_goldtocoin.png");
//    this->addChild(title);
//    title->setPosition(titlebg->getPosition());
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
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
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_SystemMailLayer_str8, FONT_XINGKAI, 36 ,ccc3(0xff, 0x95, 0x0c) );
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(upCenter, ccp(0, -headhgt+65/2)));
    this->addChild(title);

    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGGoldChangCoin::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGGoldChangCoin::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.65, title->getPosition().y));
    
    float middle = s.height - btmhgt -headhgt;
    CCPoint bottom = SGLayout::getPoint(kBottomCenter);
    if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
            CCSprite *baidi = CCSprite::createWithSpriteFrameName("barrack_title_bg.png");
            baidi->setPosition(ccpAdd(bottom, ccp(0, btmhgt + middle*.085)));
            this->addChild(baidi);
        
            SGCCLabelTTF *label = SGCCLabelTTF::create(str_SystemMailLayer_str9, FONT_BOXINFO, 25);
            label->setAnchorPoint(ccp(0.6, 0.5));
            this->addChild(label);
            label->setPosition(baidi->getPosition());
        
            CCSprite *bian = CCSprite::createWithSpriteFrameName("advance_bian.png");
            bian->setAnchorPoint(ccp(.65, .5));
            this->addChild(bian);
            bian->setPosition(ccpAdd(baidi->getPosition(), ccp(-s.width*.5, 0)));
        
            SGButton *special = SGButton::create("store_exchangebtnbg.png", "font_becomevip.png", this, menu_selector(SGGoldChangCoin::becomeVipHandler),CCPointZero,false,true);
            this->addBtn(special);
            special->setPosition(ccpAdd(baidi->getPosition(), ccp(260, 0)));

    }
    
    
    duihuan = SGButton::create("store_lank_big.png", "store_duihuan.png", this, menu_selector(SGGoldChangCoin::exchangeHandler),CCPointZero,false,true);
    this->addBtn(duihuan);
    duihuan->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(180, -125)));
    label1 = SGCCLabelTTF::create(CCString::createWithFormat("%d",costgold)->getCString(), FONT_BOXINFO, 30);
    this->addChild(label1);
    label1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-35,50)));
    
    SGCCLabelTTF *label2 = SGCCLabelTTF::create(CCString::createWithFormat("%d",getcoin)->getCString(), FONT_BOXINFO, 30);
    this->addChild(label2);
    label2->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-35,-125)));
    
    SGCCLabelTTF *font_yb = SGCCLabelTTF::create(str_yuanbao, FONT_BOXINFO, 25);
    SGCCLabelTTF *font_tq = SGCCLabelTTF::create(str_Tong_money, FONT_BOXINFO, 25);
    this->addChild(font_tq);
    this->addChild(font_yb);
    font_yb->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(70,50)));
    font_tq->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(70,-125)));

    
    CCSprite *gold = CCSprite::createWithSpriteFrameName("store_k.png");
    this->addChild(gold);
    gold->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-180, 50)));;
    CCSprite *coin = CCSprite::createWithSpriteFrameName("store_k.png");
    this->addChild(coin);
    coin->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-180, -125)));
    
    CCSprite *yb = CCSprite::createWithSpriteFrameName("big_yb.png");
    this->addChild(yb);
    yb->setPosition(gold->getPosition());
    
    CCSprite *yb1 = CCSprite::createWithSpriteFrameName("big_tq.png");
    this->addChild(yb1);
    yb1->setPosition(coin->getPosition());

    CCString *str_ = NULL;
    if (lvl == 0)
    {
        if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
             str_ = CCString::createWithFormat(str_SystemMailLayer_str10,curr,max);
        }
      else
      {
        str_ = CCString::createWithFormat(str_SystemMailLayer_str11,curr,max);
      }
    }else
    {
        if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
            str_ = CCString::createWithFormat(str_SystemMailLayer_str12,lvl,curr,max);

        }
        else{
           str_ = CCString::createWithFormat(str_SystemMailLayer_str11,curr,max);
        }
    }
    label3 = SGCCLabelTTF::create(str_->getCString(), FONT_BOXINFO, 25);
    label3->setAnchorPoint(ccp(0, 0.5));
    this->addChild(label3);
    label3->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-245, 130)));
    
    if (maxChance >= currChance)
    {
        duihuan->setImage("store_lankhui.png");
    }
}
void SGGoldChangCoin::ExchangeListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_goldChangeCoin);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopExchangeCoinResponse *response = (main::ShopExchangeCoinResponse *)sr->m_msg;
    
    if (sr)
    {
        if (response->state() == 0) {
            
            CCString *str_ = NULL;
            if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
                if (vipLvl == 0) {
                    str_ = CCString::createWithFormat(str_SystemMailLayer_str10,currChance,++maxChance);
                }else{
                    str_ = CCString::createWithFormat(str_SystemMailLayer_str12,vipLvl,currChance,++maxChance);
                }
            }
            else
            {
            str_ = CCString::createWithFormat(str_SystemMailLayer_str11,currChance,++maxChance);
            }
           
            label3->setString(str_->getCString());
            label1->setString(CCString::createWithFormat("%d",response->nexttimecostgold())->getCString());
            if (maxChance >= currChance) {
                duihuan->setImage("store_lankhui.png");
            }
            
            
            SGPlayerInfo::sharePlayerInfo()->setPlayerGold(SGPlayerInfo::sharePlayerInfo()->getPlayerGold() - cost_gold);
            SGPlayerInfo::sharePlayerInfo()->setPlayerCoins(SGPlayerInfo::sharePlayerInfo()->getPlayerCoins() + get_coin);
            
            EFFECT_PLAY(MUSIC_34);
            
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_SystemMailLayer_str13,get_coin)->getCString());
            SGMainManager::shareMain()->updataUserMsg();
            cost_gold = response->nexttimecostgold();
        }
    }

}
void SGGoldChangCoin::exchangeHandler()
{
    
    if (maxChance >= currChance) {

        SGCantAdvanceBox *cantexchangeBox = SGCantAdvanceBox::create(this, NULL,71, cost_gold);
        SGMainManager::shareMain()->showBox(cantexchangeBox);
    }else if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold()<cost_gold) {

        SGCantAdvanceBox *cantexchangeBox = SGCantAdvanceBox::create(this, NULL,9, cost_gold);
        SGMainManager::shareMain()->showBox(cantexchangeBox);
    }else{

        main::ShopExchangeCoinRequest *request = new main::ShopExchangeCoinRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGE_COINS, request);
    }    
}
void SGGoldChangCoin::storeEnterListener(cocos2d::CCObject *obj)
{
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopChargeEntryResponse *response = (main::ShopChargeEntryResponse *)sr->m_msg;
    if (sr)
    {
        main::VIPInfo vip = response->vipinfo();
        int leng = response->chargeitems_size();
        CCArray *arrary = CCArray::create();
        CCArray *strary = CCArray::create();
        for (int i = 0;i<leng;i++) {
            main::ChargeItem item = response->chargeitems(i);
           CCString *str = CCString::createWithFormat("%d-%d-%d-%d-%d-%d-%d-%d",item.showtype(),item.chargetype(),item.discount(),item.gold(),item.rmb(),item.goldperday(),item.leftday(),item.rewardday());
            arrary->addObject(str);
            CCString *str2 = CCString::createWithFormat("%s",item.code().c_str());
            strary->addObject(str2);
            
        }
        
        
        SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,2,strary);
//        SGMainManager::shareMain()->showStoreLayer(response->currentviplevel(),response->nextviplevel(),response->updateneedrmb(),2);
    }

}
void SGGoldChangCoin::becomeVipHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (SGMainManager::shareMain()->getMainLayer()) {
        SGMainManager::shareMain()->getMainLayer()->requestStoreLayer();
    }
}
void SGGoldChangCoin::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showShopLayer();
}
