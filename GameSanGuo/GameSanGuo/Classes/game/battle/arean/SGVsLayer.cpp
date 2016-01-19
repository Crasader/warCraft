//
//  SGVsLayer.cpp
//  GameSanGuo
//  点击对战进入该layer选择相应的pvp模式
//  Created by angel on 14-12-18.
//
//

#include "SGVsLayer.h"
#include "SGMainManager.h"
#include "SGStringConfig.h"
#include "ArenaModule.pb.h"
#include "SGStaticDataManager.h"

SGVsLayer::SGVsLayer()
{
}
SGVsLayer::~SGVsLayer()
{
     ResourceManager::sharedInstance()->unBindLayerTexture(sg_vsLayer);
}
SGVsLayer * SGVsLayer::create()
{
    SGVsLayer * vsLayer = new SGVsLayer();
    if(vsLayer&& vsLayer->init(NULL,sg_vsLayer))
    {
        vsLayer->initView();
        vsLayer->autorelease();
        return vsLayer;
    }
    CC_SAFE_DELETE(vsLayer);
    return NULL;
}
void SGVsLayer::initView()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH = SGMainManager::shareMain()->getBottomHeight();
    
    ResourceManager::sharedInstance()->bindTexture("sgAreanLayer/sgAreanLayer.plist", RES_TYPE_LAYER_UI, sg_vsLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/activity_growplan.plist", RES_TYPE_LAYER_UI, sg_vsLayer);//activity_growplan_downbg.png
    
    CCSprite * bg = CCSprite::createWithSpriteFrameName("activity_growplan_downbg.png");
    bg->setScaleX(size.width/bg->getContentSize().width * 1.01);
    bg->setScaleY((size.height-headH-bottomH)/bg->getContentSize().height* 1.01);
    bg->setAnchorPoint(ccp(0.5f, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    bg->setPosition(ccp(size.width/2,bottomH));
    this->addChild(bg,-5);
    
    
    
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    //    this->addChild(titlecenter,10);
	this->addChild(titlecenter,1);//10.28//george
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-size.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(size.width/2,  - title_bg->getContentSize().height -10)));
	this->addChild(title_bg_r,1);
    
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_sgareanlayer_str31, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.42)));
    this->addChild(title);
    
    //返回按钮
	SGButton *buttonBack = SGButton::create("chatbtn.png", "public_font_fanhui.png",this, menu_selector(SGVsLayer::backHandler),CCPointZero,false,true);
	buttonBack->setScale(1.05);
    buttonBack->setPosition(ccpAdd(title->getPosition(), ccp(-skewing(160), 0)));
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(buttonBack);
    
    
    
    SGButton * tiantiButton = SGButton::create("tian_ti_button_bg.png","tian_ti_button_bg.png",this,menu_selector(SGVsLayer::onClickTianTiButton),ccp(0,0),false,true,SGBE_GRAY_MAGNIFY);
    tiantiButton->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(180,-50)));
    //天梯引导多加一步
    tiantiButton->setTag(SGStaticDataManager::shareStatic()->getimitTagById(3, 3));
    
    
    SGButton * areanButton = SGButton::create("arean_button_bg.png","arean_button_bg.png",this,menu_selector(SGVsLayer::onClickAreanButton),ccp(0,0),false,true,SGBE_GRAY_MAGNIFY);
    areanButton->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-130,-50)));
    //引导用的tag
    areanButton->setTag(SGStaticDataManager::shareStatic()->getimitTagById(18, 17));
    
    addBtn(areanButton);
    addBtn(tiantiButton);

    
    
}
void  SGVsLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showFirstLayer();
}
void SGVsLayer::onClickTianTiButton()
{
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    
    if (!player->canBreakLimitById(limitPvpBattle)) {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str14,player->getLimitLevelById(limitPvpBattle))->getCString() );
    }
    else
    {
        
        SGMainManager::shareMain()->showPvpMainLayer();
    }
}
void SGVsLayer::onClickAreanButton()
{
   SGMainManager::shareMain()->goToArenaLayerRequest();    
}

