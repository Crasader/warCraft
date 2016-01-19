//
//  SGSoulCardInfoLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#include "SGSoulCardInfoLayer.h"
#include "SGMainManager.h"
#include "cocos-ext.h"
#include "SGSellBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "itemmodule.pb.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"
USING_NS_CC_EXT;
SGSoulCardInfoLayer::SGSoulCardInfoLayer()
:_card(NULL),
protectBtn(NULL),
hd(NULL),
btnwidth(0.0f),
btnwid(0.0f),
lock(NULL),
enterType(0)
{
}
SGSoulCardInfoLayer::~SGSoulCardInfoLayer()
{
 
	
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_soulCardLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_PROP_DISPROTECT);
    notification->removeObserver(this, MSG_PROP_PROTECT);

}
SGSoulCardInfoLayer *SGSoulCardInfoLayer::create(SGPropsCard *card,int enterType)
{
    SGSoulCardInfoLayer *soulCardInfoLayer = new SGSoulCardInfoLayer();
    if (soulCardInfoLayer && soulCardInfoLayer->init(NULL, sg_soulCardLayer))
    {
        soulCardInfoLayer->_card = card;
        soulCardInfoLayer->enterType = enterType;
        soulCardInfoLayer->initView();
        soulCardInfoLayer->autorelease();
        return soulCardInfoLayer;
    }
    CC_SAFE_DELETE(soulCardInfoLayer);
    return NULL;
}

void SGSoulCardInfoLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    ResourceManager::sharedInstance()->bindTexture("prop/prop1-20.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer, LIM_PNG_AS_PNG);

    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_PROP_DISPROTECT,
                              this,
                              callfuncO_selector(SGSoulCardInfoLayer::disprotectListener));
    notification->addObserver(MSG_PROP_PROTECT,
                              this,
                              callfuncO_selector(SGSoulCardInfoLayer::propprotectListener));

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight= SGMainManager::shareMain()->getBottomHeight();
    //float frameheight = skewingY(110);
    float hgt = skewingY(316);
    if (s.height == 1136) {
        CCLOG("1111");
        //frameheight = skewing(110);
        hgt = skewing(316);
    }
    
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/qhtexiao.plist");
     ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    //左火
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/qhhyA.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect, 10, 0);
    //右火
    CCSpriterX *fermEffect1 = CCSpriterX::create("animationFile/qhhyB.scml", true, true);
    fermEffect1->setCCSpriterXFlipX(true);
    fermEffect1->setanimaID(0);
    fermEffect1->setisloop(true);
    addChild(fermEffect1, 10, 1);
    
//    //地圈
//    CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/qh_000.scml", true, true);
//    fermEffect2->setanimaID(0);
//    fermEffect2->setisloop(true);
//    addChild(fermEffect2, 1, 2);
//    //光圈
//    CCSpriterX *fermEffect3 = CCSpriterX::create("animationFile/qh_001.scml", true, true);
//    fermEffect3->setanimaID(0);
//    fermEffect3->setisloop(true);
//    addChild(fermEffect3, 4, 3);
    
    CCString *str = CCString::createWithFormat("prop%d.png",_card->getItemId());
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    this->addChild(item,2);
    item->setAnchorPoint(ccp(0.5, 0));

    if (s.height == 960) {
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
//        fermEffect2->setScale(s.width/768);
//        fermEffect3->setScale(s.width/768);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287+26)));
//        fermEffect2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -430)));
//        
//        fermEffect3->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -430)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -365)));
      
    }
    else if (s.height == 1136) {
        frameSize = CCRectMake(0, 520 -(s.height - hgt*0.87 - title_bg->getContentSize().height), 768, (s.height - hgt*0.87 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);
//        fermEffect2->setScaleX(s.width/768);
//        fermEffect3->setScaleX(s.width/768);
        fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361 +26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26)));
//        fermEffect2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(1.5, -530)));
//        
//        fermEffect3->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(1.5, -530)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -450)));
       
    }else
    {
        CCSprite *a = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        this->addChild(a,-100);
        a->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - a->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width*.41, a->getContentSize().height*.02 -a->getContentSize().height*.155)));
        fermEffect1->setPosition(ccpAdd(a->getPosition(), ccp(a->getContentSize().width*.41, a->getContentSize().height*.02-a->getContentSize().height*.155)));
//        fermEffect2->setPosition(ccpAdd(a->getPosition(), ccp(1.5, -a->getContentSize().height*.365-a->getContentSize().height*.195)));
//        fermEffect3->setPosition(ccpAdd(a->getPosition(), ccp(1.5, -a->getContentSize().height*.365-a->getContentSize().height*.195)));
        item->setPosition(ccpAdd(a->getPosition(), ccp(0, -a->getContentSize().height*.362)));
    }
    fermEffect->play();
    fermEffect1->play();
//    fermEffect2->play();
//    fermEffect3->play();
    menu->setZOrder(7);
   
    this->setstar(_card->getCurrStar());
    this->setTitle(_card->getOfficerName()->getCString());
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGSoulCardInfoLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSoulCardInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    CCSprite *bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
    bg1->setAnchorPoint(ccp(0.5, 0));
    bg1->setScaleX(s.width/bg1->getContentSize().width);
    bg1->setScaleY((hgt*0.871-btmheight*.68)/bg1->getContentSize().height);
    bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68)));
    this->addChild(bg1,5);
    
    CCSprite* jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
    jinbian->setPosition(ccp(s.width/2, hgt*0.871));
    jinbian->setScaleX(s.width/jinbian->getContentSize().width);
    this->addChild(jinbian,6);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 320));
    this->addChild(frame2,7);
    frame2->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, -frame2->getContentSize().height/2 - 30)));

    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(320/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    
    CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2l,6);
    guang2l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2r,6);
    guang2r->setFlipX(true);
    guang2r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guang2m,6);
    guang2m->setScaleX(468/guang2m->getContentSize().width);
    guang2m->setAnchorPoint(ccp(0.5, 1));
    
    guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
    guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(284+10, 0)));
    guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-284-10, 0)));
    
    CCScale9Sprite *font_bg = CCScale9Sprite::createWithSpriteFrameName("barrack_info_bg.png");
    font_bg->setPreferredSize(CCSizeMake(538, 42));
    font_bg->setPosition(ccpAdd(frame2->getPosition(), ccp(0, 109)));
    this->addChild(font_bg,6);
    
    SGCCLabelTTF *label_exp = SGCCLabelTTF::create(CCString::createWithFormat(str_SoulCardInfoLayer_str1,_card->getValue())->getCString(), FONT_PANGWA, 28);
    this->addChild(label_exp,6);
    label_exp->setAnchorPoint(ccp(0, 0.5));
    label_exp->setPosition(ccpAdd(font_bg->getPosition(), ccp(-font_bg->getContentSize().width*.5f +12, 0)));
    
    CCScale9Sprite *fontbg = CCScale9Sprite::createWithSpriteFrameName("box_fontbg.png");
    fontbg->setPreferredSize(CCSizeMake(538, 170));
    this->addChild(fontbg,6);
    fontbg->setPosition(ccpAdd(frame2->getPosition(), ccp(0, -9)));

    hd = CCSprite::createWithSpriteFrameName("barrack_huibg.png");
    this->addChild(hd,6);
    hd->setScaleX(538/hd->getContentSize().width);
    hd->setPosition(ccpAdd(frame2->getPosition(), ccp(0, -124)));
    
    SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
    SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getPropsInfo()->getCString(), FONT_BOXINFO, 28, CCSizeMake(504,300));
    a->setAnchorPoint(ccp(0.5, 1));
    this->addChild(a,6);
    a->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, fontbg->getContentSize().height*.33)));

    
    SGCCLabelTTF *tishi = SGCCLabelTTF::create(str_SoulCardInfoLayer_str2, FONT_BOXINFO,26 , ccc3(0xc4, 0xc4, 0xc4));
    tishi->setPosition(hd->getPosition());
    this->addChild(tishi,7);
    //modify by:zyc. merge into create.
    //tishi->setColor(ccc3(0xc4, 0xc4, 0xc4));

    if(_card->getIsProt() == 1)
    {
        protectBtn = SGButton::create("store_exchangebtnbg.png",
                                        "font_jiechu.png",
                                        this,menu_selector(SGSoulCardInfoLayer::disProtectHandler),
                                        ccp(0, 0),
                                        false,
                                        true);
        
    }
    else if(_card->getIsProt() == 0)
    {
        protectBtn = SGButton::create("store_exchangebtnbg.png",
                                        "font_protect.png",
                                        this,menu_selector(SGSoulCardInfoLayer::protectHandler),
                                        ccp(0, 0),
                                        false,
                                        true);
        
    }
    this->addBtn(protectBtn);
    SGButton *sellBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_Show_, this, menu_selector(SGSoulCardInfoLayer::sellHandler), CCPointZero, FONT_PANGWA, ccWHITE, 32, false, true);

    this->addBtn(sellBtn);
    
    btnwidth = skewing(5);
    btnwid = skewing(33);
    protectBtn->setPosition(ccpAdd(hd->getPosition(), ccp(btnwid + btnwidth, -hd->getContentSize().height/2 - protectBtn->getContentSize().height-10)));
    sellBtn->setPosition(ccp(s.width/2 + btnwid*3 + btnwidth * 3,protectBtn->getPosition().y));
    
    lock = CCSprite::createWithSpriteFrameName("card_suo.png");
    this->addChild(lock,10);
    lock->setAnchorPoint(ccp(0.8, 0.8));
    lock->setPosition(ccpAdd(sellBtn->getPosition(), ccp(sellBtn->getContentSize().width/2, sellBtn->getContentSize().height/2)));
    lock->setVisible(false);
    if (_card->getIsProt()) {
        lock->setVisible(true);
    }    
    SGPropsDataModel *general = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
    this->setCardType(general->getPropsType());
    
    if (enterType == CHATSHOWENTRYTAG) {
        sellBtn->setVisible(false);
        protectBtn->setVisible(false);
    }
}

void SGSoulCardInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (enterType == CHATSHOWENTRYTAG) {
        SGMainManager::shareMain()->closeChatShowLayer();
    }
    else
    {
        SGMainManager::shareMain()->showSoldierslayer();
    }
    
}
void SGSoulCardInfoLayer::protectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PropsProtectionRequest *request = new main::PropsProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_PROP_PROTECT, request);
//    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(this,_card,2,NULL);
//    SGMainManager::shareMain()->showBox(cantadvanceBox);
}
void SGSoulCardInfoLayer::disProtectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PropsUnProtectionRequest *request = new main::PropsUnProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_PROP_DISPROTECT, request);

}
void SGSoulCardInfoLayer::showBoxCall(cocos2d::CCObject *obj)
{
    if (obj)
    {
        
    }
}
void SGSoulCardInfoLayer::disprotectListener(CCObject *obj)
{
    
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::PropsUnProtectionResponse *response = (main::PropsUnProtectionResponse *)sr->m_msg;
    const char * msg = str_Equipslayer_str4;
    if (sr)
    {
        if (response->state() == 1)
        {
            msg = str_Equipslayer_str5;
            _card->setIsProt(0);
            menu->removeChild(protectBtn, true);
            protectBtn = SGButton::create("store_exchangebtnbg.png",
                                            "font_protect.png",
                                            this,menu_selector(SGSoulCardInfoLayer::protectHandler),
                                            ccp(0, 0),
                                            false,
                                            true);
            protectBtn->setPosition(ccpAdd(hd->getPosition(), ccp(btnwid + btnwidth, -hd->getContentSize().height/2 - protectBtn->getContentSize().height - 10)));
            this->addBtn(protectBtn);
            lock->setVisible(false);
        }
        else if(response->state() == 0)
        {
            msg = str_Equipslayer_str4;
            CCLOG("取消保护失败");
        }
    }
    SGMainManager::shareMain()->showMessage(msg);

    
}
void SGSoulCardInfoLayer::sellHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGPropsDataModel *card =  SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
    SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(_card->getOfficerName(), 4, card->getPropsCurrStarLevel(), _card->getSsid());
//    if(_card->getIsProt() == 1)
//    {
//        
//        SGMainManager::shareMain()->showMessage("卡片处于保护状态,解保后才能卖出");
//    }
//    else{
//    SGSellBox *sellBox = SGSellBox::create(this,_card,4);
//        SGMainManager::shareMain()->showBox(sellBox);}
}

void SGSoulCardInfoLayer::propprotectListener(cocos2d::CCObject *obj)
{
    
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_soulCardLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
        
    if (sr)
    {
        main::PropsProtectionResponse *response = (main::PropsProtectionResponse *)sr->m_msg;

        if (response->state() == 1) {
            menu->removeChild(protectBtn, true);
           
                protectBtn = SGButton::create("store_exchangebtnbg.png",
                                              "font_jiechu.png",
                                              this,menu_selector(SGSoulCardInfoLayer::disProtectHandler),
                                              ccp(0, 0),
                                              false,
                                              true);
                protectBtn->setPosition(ccpAdd(hd->getPosition(), ccp(btnwid + btnwidth, -hd->getContentSize().height/2 - protectBtn->getContentSize().height - 10)));
                this->addBtn(protectBtn);
                _card->setIsProt(1);
                lock->setVisible(true);
        
            SGMainManager::shareMain()->showMessage(str_Equipslayer_str6);
        }else
        {
            CCLOG("设为保护失败");
        }
    }
}
