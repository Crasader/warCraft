//
//  SGPropInfoLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-19.
//
//

#include "SGPropInfoLayer.h"
#include "SGMainManager.h"
#include "cocos-ext.h"
#include "SGSellBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "itemmodule.pb.h"
#include "SGStaticDataManager.h"
#include "SGBattleScoreLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGPropInfoLayer::SGPropInfoLayer():
sellBtn(NULL),
enterType(0),
_card(NULL),
protectBtn(NULL),
btnwidth(0.0),
btnwid(0.0),
lock(NULL),
_currentPage(0)
{
}
SGPropInfoLayer::~SGPropInfoLayer()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_PROP_DISPROTECT);
    notification->removeObserver(this, MSG_PROP_PROTECT);
       CCLOG("~SGPropInfoLayer");
    CC_SAFE_RELEASE(_card);

 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_propInfoLayer);

}
SGPropInfoLayer *SGPropInfoLayer::create(SGPropsCard *card_,int type,int currentPage)
{
    SGPropInfoLayer *propInfoLayer = new SGPropInfoLayer();
    if (propInfoLayer && propInfoLayer->init(NULL, sg_propInfoLayer))
    {
        propInfoLayer->_currentPage=currentPage;
        propInfoLayer->enterType = type;
        propInfoLayer->_card = card_;
        propInfoLayer->_card->retain();
        propInfoLayer->initView();
        propInfoLayer->autorelease();
        return propInfoLayer;
    }
    CC_SAFE_DELETE(propInfoLayer);
    return NULL;
}
void SGPropInfoLayer::initView()
{
 
    ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_propInfoLayer);
 
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_PROP_DISPROTECT,
                              this,
                              callfuncO_selector(SGPropInfoLayer::disprotectListener));
    notification->addObserver(MSG_PROP_PROTECT,
                              this,
                              callfuncO_selector(SGPropInfoLayer::propprotectListener));
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight = SGMainManager::shareMain()->getBottomHeight();

    //float frameheight = skewingY(155);
    float hgt = skewingY(244);
    if (s.height == 1136) {
        //frameheight = skewing(155);
        hgt = skewing(244);
    }
    

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
    CCLOG("type%d",_card->getItemType());

    SGMainManager::shareMain()->addPropPng(_card->getHeadSuffixNum(),sg_propInfoLayer);
    CCString *str = CCString::createWithFormat("prop%d.png",_card->getHeadSuffixNum());
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());

    this->addChild(item,2);
    item->setAnchorPoint(ccp(0.5, 0));

    if (s.height == 960) {
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);

        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287+26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -365)));

        }
    else if (s.height == 1136) {
        frameSize = CCRectMake(0, 520 -(s.height - hgt*1.13 - title_bg->getContentSize().height), 768, (s.height - hgt*1.13 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);
    
        fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26)));
      
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -450)));


    }else
    {
        CCSprite *a = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        this->addChild(a,-100);
        a->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - a->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width*.41, a->getContentSize().height*.02 -a->getContentSize().height*.09)));
        fermEffect1->setPosition(ccpAdd(a->getPosition(), ccp(a->getContentSize().width*.41, a->getContentSize().height*.02-a->getContentSize().height*.09)));
        
        item->setPosition(ccpAdd(a->getPosition(), ccp(0, -a->getContentSize().height*.362)));
    }
    fermEffect->play();
    fermEffect1->play();
    menu->setZOrder(7);

    this->setstar(_card->getCurrStar());
    this->setTitle(_card->getOfficerName()->getCString());
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGPropInfoLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGPropInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    
    if(enterType == 101  || enterType==25)
    {
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_propInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.13)/2, s.width, hgt*1.13);
        //bg1->cocos2d::CCNode::setScale(r.size.width / bg1->getContentSize().width, r.size.height / bg1->getContentSize().height);
        bg1->setTextureRect(r);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
    }
    else
    {
    CCSprite *bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
    bg1->setAnchorPoint(ccp(0.5, 0));
    bg1->setScaleX(s.width/bg1->getContentSize().width);
    bg1->setScaleY((hgt*1.13-btmheight*.68)/bg1->getContentSize().height);
    bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68 )));
    this->addChild(bg1,5);
    }
    
    CCSprite* jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
    jinbian->setPosition(ccp(s.width/2, hgt*1.13));
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
    
    CCScale9Sprite *fontbg = CCScale9Sprite::createWithSpriteFrameName("box_fontbg.png");
    fontbg->setPreferredSize(CCSizeMake(538, 205));
    this->addChild(fontbg,6);
    fontbg->setPosition(ccpAdd(frame2->getPosition(), ccp(0, 26)));
    
    CCSprite* hd = CCSprite::createWithSpriteFrameName("barrack_huibg.png");
    this->addChild(hd,6);
    hd->setScaleX(538/hd->getContentSize().width);
    hd->setPosition(ccpAdd(frame2->getPosition(), ccp(0, -112)));
    
    SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
    SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getPropsInfo()->getCString(), FONT_BOXINFO, 28, CCSizeMake(504,400));
    a->setAnchorPoint(ccp(0.5f, 1));
    this->addChild(a,6);
     a->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, fontbg->getContentSize().height*.33)));
    
    if(_card->getIsProt() == 1)
    {
        protectBtn = SGButton::create("hotBtnBg.png",
                                      "font_jiechu.png",
                                      this,menu_selector(SGPropInfoLayer::disprotectHandler),
                                      ccp(0, 0),
                                      false,
                                      true);
        
    }
    else if(_card->getIsProt() == 0)
    {
        protectBtn = SGButton::create("hotBtnBg.png",
                                      "font_protect.png",
                                      this,menu_selector(SGPropInfoLayer::protectHandler),
                                      ccp(0, 0),
                                      false,
                                      true);
        
    }
    this->addBtn(protectBtn);
    sellBtn = SGButton::createFromLocal("hotBtnBg.png", str_Show_, this, menu_selector(SGPropInfoLayer::sellHandler), CCPointZero, FONT_PANGWA, ccWHITE, 32, false, true);
    
    this->addBtn(sellBtn);
    btnwidth = skewing(5);
    btnwid = skewing(33);
    

    protectBtn->setPosition(ccp(s.width/2 + btnwid + btnwidth-40, (frame2->getPosition().y -frame2->getContentSize().height/2  - btmheight*.68)/2 + btmheight*.68));
    sellBtn->setPosition(ccp(s.width/2 + btnwid*3 + btnwidth * 3,protectBtn->getPosition().y));
    
    SGCCLabelTTF *tishi = SGCCLabelTTF::create(str_PropInfolayer_str1, FONT_BOXINFO, 26 , ccc3(0xc4, 0xc4, 0xc4));
    //modify by:zyc. merge into create.
    //tishi->setColor(ccc3(0xc4, 0xc4, 0xc4));

    tishi->setPosition(hd->getPosition());
    this->addChild(tishi,6);
    
    lock = CCSprite::createWithSpriteFrameName("card_suo.png");
    this->addChild(lock,10);
    lock->setAnchorPoint(ccp(0.8, 0.8));
    lock->setPosition(ccpAdd(sellBtn->getPosition(), ccp(sellBtn->getContentSize().width/2-lock->getContentSize().width/2, sellBtn->getContentSize().height/2-lock->getContentSize().height/2)));
    lock->setVisible(false);
    if (_card->getIsProt()) {
        lock->setVisible(true);
    }

    
    if (enterType == 1 ||enterType == 6 ||enterType==4 ||enterType==5||enterType==10 || enterType == CHATSHOWENTRYTAG || enterType == 1980) {
        protectBtn->setVisible(false);
        sellBtn->setVisible(false);
    }
    
    SGPropsDataModel *general = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
    this->setCardType(general->getPropsType());

}
void SGPropInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
// 0正常进入 1进阶进入 2 武将进入 3装备进入 4抽奖结果进入 5强化进入 10积分兑换进入
    switch (enterType) {
        case 0:
            SGMainManager::shareMain()->showPropsLayer(true,_currentPage);
            break;
        case 1:
            showLayerByTag(sg_advanceLayer);
            break;
        case 2:
//            SGMainManager::shareMain()->showGeneralsLayer(0);
            SGMainManager::shareMain()->showGeneralsBagLayer(0);
            break;
        case 3:
            SGMainManager::shareMain()->showEquipsLayer(NULL, 0,0);
            break;
        case 4:
            showLayerByTag(sg_visitShowCard);
            break;
        case 5:
            showLayerByTag(sg_strengLayer);
            break;
        case 6:
            showLayerByTag(sg_cardsLayer);
            break;
        case 10:{
            showLayerByTag(sg_creditCardLayer);
        }break;
        case 101:{
            //showLayerByTag(sg_visitLayer);
            SGVisitLayer *layer = (SGVisitLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_visitShowCard);
            if (layer) {
                SGMainManager::shareMain()->showLayer(layer);
            }
            else
            {
              showLayerByTag(sg_visitLayer);
            }
            
            
        }
        case CHATSHOWENTRYTAG:
        {
            SGMainManager::shareMain()->closeChatShowLayer();
        }
            break;
           
        case 25:
        {
             showLayerByTag(sg_totalLoginReward);
        }
            break;
            
        default:
            break;
    }
    
}
void SGPropInfoLayer::showLayerByTag(LayerTag layerTag)
{
    
    SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(layerTag);
    if(NULL == layer)
    {
        return;
    }
    
    SGMainManager::shareMain()->showLayer(layer);
    
    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_propInfoLayer, true);
}

void SGPropInfoLayer::getHandler()
{
}
void SGPropInfoLayer::showBoxCall(cocos2d::CCObject *obj)
{
    if (obj)
    {
        
    }
}

void SGPropInfoLayer::sellHandler()
{
    SGPropsDataModel *card =  SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
    SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(_card->getOfficerName(), 4, card->getPropsCurrStarLevel(), _card->getSsid());
//    if(_card->getIsProt() == 1)
//    {
//        EFFECT_PLAY(MUSIC_BTN);
//        SGMainManager::shareMain()->showMessage("卡片处于保护状态,解保后才能卖出");
//    }
//    else{
//    SGSellBox *sellBox = SGSellBox::create(this,_card,enterType);
//        SGMainManager::shareMain()->showBox(sellBox);
//    }
}
void SGPropInfoLayer::protectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PropsProtectionRequest *request = new main::PropsProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_PROP_PROTECT, request);
}
void SGPropInfoLayer::disprotectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PropsUnProtectionRequest *request = new main::PropsUnProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_PROP_DISPROTECT, request);

}
void SGPropInfoLayer::disprotectListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_propInfoLayer);
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
            protectBtn = SGButton::create("hotBtnBg.png",
                                          "font_protect.png",
                                          this,menu_selector(SGPropInfoLayer::protectHandler),
                                          ccp(0, 0),
                                          false,
                                          true);
            protectBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(btnwid + btnwidth-40, sellBtn->getPosition().y)));
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

bool SGPropInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (enterType == 101) {
        backHandler();
    }
    return false;
}

void SGPropInfoLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
}

void SGPropInfoLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void SGPropInfoLayer::propprotectListener(cocos2d::CCObject *obj)
{
   ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist",RES_TYPE_LAYER_UI ,sg_propInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_propInfoLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::PropsProtectionResponse *response = (main::PropsProtectionResponse *)sr->m_msg;
        
        if (response->state() == 1) {
            menu->removeChild(protectBtn, true);
           
                protectBtn = SGButton::create("hotBtnBg.png",
                                              "font_jiechu.png",
                                              this,menu_selector(SGPropInfoLayer::disprotectHandler),
                                              ccp(0, 0),
                                              false,
                                              true);
                protectBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(btnwid + btnwidth-40, sellBtn->getPosition().y)));
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