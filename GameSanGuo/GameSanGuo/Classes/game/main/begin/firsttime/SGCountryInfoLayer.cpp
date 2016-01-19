//
//  SGCountryInfoLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

/*
 
 吴国的小兵是红色
 蜀国的小兵是绿色
 魏国的小兵是蓝色
 
 */

#include "SGCountryInfoLayer.h"
#include "SGNikeNameBox.h"
#include "SGMainManager.h"
#include "SGCountryLayer.h"
#include "ResourceManager.h"
#include "PlatformAdp.h"
#include "SGStaticDataManager.h"

SGCountryInfoLayer::SGCountryInfoLayer()
:countryId(0),
_dele(NULL)
{
}

SGCountryInfoLayer::~SGCountryInfoLayer()
{
	//SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GUIDE_STEP);
     ResourceManager::sharedInstance()->unBindLayerTexture(sg_countryInfoLayer);
}
SGCountryInfoLayer *SGCountryInfoLayer::create(SGBoxDelegate *dele,int countryid_)
{
    SGCountryInfoLayer *countryLayer = new SGCountryInfoLayer();
    if (countryLayer && countryLayer->init(dele, box_countryInfoLayer,  CCSizeMake(411,300),false,false))
    {
        countryLayer->countryId = countryid_;
        countryLayer->initView();
        countryLayer->_dele = dele;
        countryLayer->autorelease();
        return countryLayer;
    }
    CC_SAFE_DELETE(countryLayer);
    return NULL;
}
void SGCountryInfoLayer::initView()
{
	//SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_GUIDE_STEP, this, callfuncO_selector(SGCountryInfoLayer::stepListener));
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI,sg_countryInfoLayer );
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI,sg_countryInfoLayer, LIM_PNG_AS_PNG );
    ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist", RES_TYPE_LAYER_UI,sg_countryInfoLayer );
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI,sg_countryInfoLayer );
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI,sg_countryInfoLayer , LIM_PNG_AS_PNG);
    CCPoint center = ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 50));
    CCSize size = CCSize(580, 765);
//黑底
    CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 220), skewing(320)*3, skewingY(960)*3);
    this->addChild(bgLayer,-3);
    bgLayer->setPosition(ccpAdd(CCPointZero, ccp(-bgLayer->getContentSize().width*.5, -bgLayer->getContentSize().height*.5)));

//边框 背景 标题线
    CCSprite* biao = CCSprite::createWithSpriteFrameName("box_bian.png");
    this->addChild(biao,1);
    biao->setPosition(ccpAdd(center, ccp(0, size.height*.5 -55)));
    
    cocos2d::extension::CCScale9Sprite *frame = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("box_kuang.png", CCRectMake(60, 60, 120, 120));
    this->addChild(frame,1);
    frame->setPreferredSize(CCSizeMake(size.width, size.height));
    frame->setPosition(center);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_countryInfoLayer);

    CCSprite *spriteBigBG = CCSprite::createWithSpriteFrameName("storyBg.png");
    //CCRect r = CCRectMake(0,1136/2-60-(float)spriteBigBG->getContentSize().width/580*765/2, spriteBigBG->getContentSize().width,
                          //((float)spriteBigBG->getContentSize().width/580)*765);
    
    //spriteBigBG->setTextureRect(r);
    spriteBigBG->setScaleY(CCDirector::sharedDirector()->getWinSize().height/2 / spriteBigBG->getContentSize().height);
    spriteBigBG->setScaleX(CCDirector::sharedDirector()->getWinSize().width / spriteBigBG->getContentSize().width);
    spriteBigBG->setScaleY(((float)580/768));
    this->addChild(spriteBigBG);
    spriteBigBG->setPosition(center);
    
    CCSprite *infobg = CCSprite::createWithSpriteFrameName("country_infobg.png");
    this->addChild(infobg);
    infobg->setPosition(ccpAdd(center, ccp(0, 95)));
    
//国家特性部分
    CCSprite *fontbg = CCSprite::createWithSpriteFrameName("barrack_title_bg.png");
    fontbg->setTextureRect(CCRect(0, 0, 580, 50));
    this->addChild(fontbg);
    fontbg->setPosition(ccpAdd(center, ccp(0, 31)));
    
    CCSprite *bian = CCSprite::createWithSpriteFrameName("advance_bian.png");
    bian->setAnchorPoint(ccp(0, .5));
    this->addChild(bian);
    bian->setPosition(ccpAdd(fontbg->getPosition(), ccp(-290, 0)));
    
    CCSprite *bianr = CCSprite::createWithSpriteFrameName("advance_bian.png");
    bianr->setAnchorPoint(ccp(1, .5));
    this->addChild(bianr);
    bianr->setFlipX(true);
    bianr->setPosition(ccpAdd(fontbg->getPosition(), ccp(290, 0)));
    
    CCSprite *spriteCountryCharacter = CCSprite::createWithSpriteFrameName("country_artWord1.png");
    this->addChild(spriteCountryCharacter);
    spriteCountryCharacter->setPosition(fontbg->getPosition());
    
    
//红框
    
    for (int i = 0; i<3; i++) {
        CCSprite *bgl = CCSprite::createWithSpriteFrameName("login_dik_l.png");
        this->addChild(bgl,0);
        bgl->setAnchorPoint(ccp(0, 0.5));
        bgl->setPosition(ccpAdd(center, ccp(-273.13, -66 -112*i)));
        
        CCSprite *bgr = CCSprite::createWithSpriteFrameName("login_dik_r.png");
        this->addChild(bgr,0);
        bgr->setAnchorPoint(ccp(1, 0.5));
        bgr->setPosition(ccpAdd(center, ccp(273.13,-66-112*i)));
        
        CCSprite *bgm = CCSprite::createWithSpriteFrameName("login_dik_m.png");
        this->addChild(bgm,0);
        bgm->setScaleX((550 - 50*2)/50);
        bgm->setPosition(ccpAdd(center, ccp(0, 1-66-112*i)));
    }
    
    
    SGCCLabelTTF *label_1 = NULL;
    SGCCLabelTTF *label_2 = NULL;
    SGCCLabelTTF *label_3 = NULL;
    CCSprite *spriteCountry = NULL;
    CCSprite *title = NULL;
//    countryId 魏 2   蜀 1   吴 3
    
    CCDictionary* dict = SGStaticDataManager::shareStatic()->getNationListPlist();
    
    CCDictionary *data = (CCDictionary*)dict->objectForKey(CCString::createWithFormat("%d",countryId)->getCString());
    
    CCString *tip = (CCString*)data->objectForKey("tip_text");
    CCString *tip1 = (CCString*)data->objectForKey("tip_text1");
    CCString *tip2 = (CCString*)data->objectForKey("tip_text2");
    label_1 = SGCCLabelTTF::create(tip->getCString(), FONT_FZXK, 26, CCSizeMake(430, 80));
    label_2 =  SGCCLabelTTF::create(tip1->getCString(), FONT_FZXK, 26, CCSizeMake(430, 80));
    label_3 =  SGCCLabelTTF::create(tip2->getCString(), FONT_FZXK, 26, CCSizeMake(430, 80));

    
    
    if(countryId == 2)
    {
        spriteCountry = CCSprite::createWithSpriteFrameName("country_wei.png");
        title = CCSprite::createWithSpriteFrameName("country_font_wei.png");
    }
    else if(countryId == 1)
    {
        spriteCountry = CCSprite::createWithSpriteFrameName("country_shu.png");
        title = CCSprite::createWithSpriteFrameName("country_font_shu.png");
    }
    else if(countryId == 3)
    {
        spriteCountry = CCSprite::createWithSpriteFrameName("country_wu.png");
        title = CCSprite::createWithSpriteFrameName("country_font_wu.png");
    }

    this->addChild(title,3);
    this->addChild(label_1);
    this->addChild(label_2);
    this->addChild(label_3);
    this->addChild(spriteCountry);
    if (title) {
        title->setPosition(biao->getPosition());
    }
    if (spriteCountry) {
        spriteCountry->setPosition(ccpAdd(center, ccp(0, 210)));
    }
    
    if (label_1) {
        label_1->setPosition(ccpAdd(center, ccp(0,-78)));
    }
    if (label_2) {
        label_2->setPosition(ccpAdd(center, ccp(0, -78 - 112)));
    }
    if (label_3) {
        label_3->setPosition(ccpAdd(center, ccp(0, -78 -224)));
    }
    
    SGButton *buttonOK = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGCountryInfoLayer::confirmHandler),CCPointZero,false,true);
    SGButton *buttonNO = SGButton::create("box_btnbg.png", "public_font_quxiao.png", this, menu_selector(SGCountryInfoLayer::cancelHandler),CCPointZero,false,true);
    this->addBtn(buttonOK);
    this->addBtn(buttonNO);
    buttonOK->setPosition(ccpAdd(center, ccp(-145,-765/2-45 -30)));
    buttonNO->setPosition(ccpAdd(center, ccp(145, -765/2-45 -30)));
    
}

void SGCountryInfoLayer::cancelHandler()
{
//    SGCountryLayer *layer = SGCountryLayer::create();
//    SGMainManager::shareMain()->showLayer(layer);
    this->boxClose();
}
void SGCountryInfoLayer::stepListener()
{
	CCLOG("");
}

void SGCountryInfoLayer::confirmHandler()
{
	// 新手进度，选择国家完成
    SGBoxDelegate* temp = _dele;
	SGMainManager::shareMain()->sendNewPlayerProcess(select_country, 0);
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGNikeNameBox *nikeNameBox = SGNikeNameBox::create(temp, countryId);
    SGMainManager::shareMain()->showBox(nikeNameBox);
}





