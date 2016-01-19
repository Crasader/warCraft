			//
//  SGCreditCardBox.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-27.
//
//

#include "SGCreditCardBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PvpFight.pb.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

// SGCreditCostBox
SGCreditCostBox::SGCreditCostBox(void)
{
    sendId = 0;
    t_delegate = NULL;
}
SGCreditCostBox::~SGCreditCostBox(void)
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_creditCostBox);
}

SGCreditCostBox* SGCreditCostBox::create(SGBoxDelegate *delegate, int sid)
{
    SGCreditCostBox *box = new SGCreditCostBox();
    if (box && box->init(delegate, box_creditCostBox, CCSizeMake(535,340))) {
        box->t_delegate = delegate;
        box->sendId = sid;
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGCreditCostBox::initWithInfo(int score, int cost)
{
    

    ResourceManager::sharedInstance()->bindTexture("sgskillcardslayer/sgskillcardslayer.plist", RES_TYPE_LAYER_UI, sg_box_creditCostBox);
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_box_creditCostBox);
    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("title_quickWar.png");
//    title->setPosition(ccpAdd(center, ccp(0, size.height/2-skewing(22))));
//    title->setPosition(ccpAdd(center, ccp(0, size.height*.5 -55)));
//    this->addChild(title);
//    title->setVisible(false);
    
    float fontSize = 28;
    const char *fontName = FONT_PANGWA;
    float posX = -this->boxSize.width/2 + 70;
    const char *desc = CCString::createWithFormat(str_CreditCardBox_str1, score)->getCString();
    SGCCLabelTTF *label = SGCCLabelTTF::create(desc, fontName, fontSize);
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccpAdd(biao->getPosition(), ccp(posX, -70)));
    this->addChild(label);
    desc = CCString::createWithFormat(str_CreditCardBox_str2, cost)->getCString();
    label = SGCCLabelTTF::create(desc, fontName, fontSize);
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccpAdd(biao->getPosition(), ccp(posX, -70-40)));
    this->addChild(label);
    desc = CCString::createWithFormat(str_CreditCardBox_str3, cost)->getCString();
    label = SGCCLabelTTF::create(desc, fontName, fontSize);
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccpAdd(biao->getPosition(), ccp(posX, -70-40*2)));
    this->addChild(label);
    
    SGButton *okBtn = SGButton::create("box_btn_newbg.png",
                                       "public_font_queding.png",
                                       this,
                                       menu_selector(SGCreditCostBox::sureAction),
                                       ccp(0, 0),
                                       false,
                                       true);
    this->addBtn(okBtn);
    okBtn->setPosition(ccpAdd(center, ccp(-size.width/2+90, -size.height/2+40)));
    
    SGButton *cancelBtn = SGButton::create("box_btn_newbg.png",
                                           "public_font_quxiao.png",
                                           this,
                                           menu_selector(SGCreditCostBox::cancelAction),
                                           ccp(0, 0),
                                           false,
                                           true);
    this->addBtn(cancelBtn);
    cancelBtn->setPosition(ccpAdd(center, ccp(size.width/2-90, -size.height/2+40)));
}

void SGCreditCostBox::sureAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PvpShopPurchRequest *request = new main::PvpShopPurchRequest();
    request->set_uid(sendId);
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_CDCHG, request);
    
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
}

void SGCreditCostBox::cancelAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
}

void SGCreditCostBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}


// SGCantMessageBox
SGCantMessageBox::SGCantMessageBox(void)
{
    spriteBackground = NULL;
    labelContent = NULL;
}
SGCantMessageBox::~SGCantMessageBox(void)
{
}

bool SGCantMessageBox::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    labelContent = SGCCLabelTTF::create(str_haha, FONT_BOXINFO, FONTSIZE(14) , ccRED);
    //modify by:zyc. merge into create.
    //labelContent->setColor(ccRED);
    spriteBackground = CCLayerColor::create((ccColor4B){166,166,166, 255});
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCSize lsize = labelContent->getContentSize();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    spriteBackground->setContentSize(CCSizeMake(size.width, lsize.height+skewing(15)));
    
    float posx = spriteBackground->getContentSize().width/2 - size.width/2;
    float posy = -spriteBackground->getContentSize().height/2;
    spriteBackground->setPosition(ccpAdd(center, ccp(posx, posy)));
    spriteBackground->addChild(labelContent);
    this->addChild(spriteBackground);
    
    this->setVisible(false);
    return true;
}

SGCantMessageBox* SGCantMessageBox::create()
{
    SGCantMessageBox *box = new SGCantMessageBox();
    if (box && box->init()) {
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGCantMessageBox::showMessage(const char *content)
{
    this->setVisible(true);
    labelContent->setString(content);
    CCSize size = labelContent->getContentSize();
    spriteBackground->setContentSize(CCSizeMake(size.width+skewing(15), size.height+skewing(15)));
}

void SGCantMessageBox::dismiss()
{
    this->setVisible(false);
}
