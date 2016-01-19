//
//  SGCantVisitBox.cpp
//  GameSanGuo
//

#include "SGCantVisitBox.h"
#include "ShopModule.pb.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGShowString.h"

SGCantVisitBox::SGCantVisitBox()
: m_type(-1)
, m_cost(100000)
, m_delegate(NULL)
{
}

SGCantVisitBox::~SGCantVisitBox()
{
    
}

SGCantVisitBox *SGCantVisitBox::create(SGBoxDelegate *bdg,int type , int cost)
{
    SGCantVisitBox * ret = new SGCantVisitBox();
    if (ret && ret->init(NULL,box_cantVisitBox,CCSize(500,360),false,false ) )
    {
        
        ret->m_delegate = bdg;
        ret->m_type = type;
        ret->m_cost = cost;
        
        ret->initView();
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}


void SGCantVisitBox::initView()
{
    SGButton * okBtn = NULL;
    SGCCLabelTTF * title = NULL;
    SGCCLabelTTF * contentText = NULL;
    if(1==m_type)
    {
        okBtn = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips35_31, this, menu_selector(SGCantVisitBox::gotoChargeLayer) ,CCPointZero , FONT_PANGWA , ccWHITE , 28 );
        title = SGCCLabelTTF::create(str_CantAdvance_tips46, FONT_PANGWA, 32);
        contentText = SGCCLabelTTF::create( str_CantAdvance_tips125 , FONT_PANGWA , 28, CCSize(400 , 200) , kCCTextAlignmentLeft , kCCVerticalTextAlignmentCenter);
    }
    else
    {
        okBtn = SGButton::createFromLocal("box_btn_newbg.png", str_CantAdvance_tips127, this, menu_selector(SGCantVisitBox::gotoExchangeLayer) ,CCPointZero , FONT_PANGWA , ccWHITE , 28 );
        title = SGCCLabelTTF::create(str_CantAdvance_tips124, FONT_PANGWA, 32);
        contentText = SGCCLabelTTF::create( str_CantAdvance_tips126 , FONT_PANGWA , 28, CCSize(400 , 200) , kCCTextAlignmentLeft , kCCVerticalTextAlignmentCenter);
    }
    CCScale9Sprite * outBg = CCScale9Sprite::createWithSpriteFrameName("Tips.png");
    outBg->setPreferredSize(CCSize(500 , 360));
    outBg->setPosition(SGLayout::getPoint(kMiddleCenter));
    this->addChild(outBg);
    
    CCScale9Sprite * insideBg = CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
    insideBg->setPreferredSize(CCSize(440,180));
    insideBg->setPosition(ccpAdd(outBg->getPosition(), ccp(0, 0) ));
    this->addChild(insideBg , outBg->getZOrder() + 1);
    
    title->setAnchorPoint(ccp(0.5, 1));
    title->setPosition(ccpAdd(outBg->getPosition(), ccp(0, outBg->getContentSize().height*0.5 - 32)));
    this->addChild(title ,1);
    
    contentText->setPosition(insideBg->getPosition());
    this->addChild(contentText, 5);
    
    okBtn->setAnchorPoint(ccp(1, 0));
    okBtn->setPosition(ccpAdd(outBg->getPosition(), ccp(-20 , -outBg->getContentSize().height*0.5 + 25)));
    addBtn(okBtn);
    
    SGButton * cancelBtn =  SGButton::createFromLocal("box_btn_newbgHui.png", str_Cancel, this, menu_selector(SGCantVisitBox::boxClose) ,CCPointZero , FONT_PANGWA , ccWHITE , 28 );
    cancelBtn->setAnchorPoint(ccp(0, 0));
    cancelBtn->setPosition(ccp(outBg->getPositionX() + 20 , okBtn->getPositionY()));
    addBtn(cancelBtn);
                          
}

void SGCantVisitBox::gotoChargeLayer()
{
    this->boxClose();
    
    if(!SGPlayerInfo::sharePlayerInfo()->getHaveCharged())
    {
        main::LuaActivityDetailRequest* req = new main::LuaActivityDetailRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_LUA_ACTIVITY, req);
    }
    else
    {
        main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
    }

}

void SGCantVisitBox::gotoExchangeLayer()
{
    this->boxClose();
    
    main::ShopExchangeCoinEntryRequest *request = new main::ShopExchangeCoinEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGE_ENTER, request);
}

void SGCantVisitBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
   
    SGBaseBox::boxClose();
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }

}







