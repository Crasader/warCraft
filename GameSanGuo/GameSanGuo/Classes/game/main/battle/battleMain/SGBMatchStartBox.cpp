//
//  SGBMatchStartBox.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#include "SGBMatchStartBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"

#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "PvpFight.pb.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

#define DOTREPEATTIME   0.2
#define TITLEGAP        50
SGBMatchStartBox::SGBMatchStartBox(void)
:t_delegate(NULL),
titleDot1(NULL),
titleDot2(NULL),
titleDot3(NULL),
m_showDotCount(0)
{
    CCLOG("SGBMatchStartBox");
}
SGBMatchStartBox::~SGBMatchStartBox(void)
{
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_bMatchStartBox);
    unschedule(schedule_selector(SGBMatchStartBox::showTitleDot));
    CCLOG("~SGBMatchStartBox");
}

SGBMatchStartBox* SGBMatchStartBox::create(SGBoxDelegate *delegate)
{
    SGBMatchStartBox *box = new SGBMatchStartBox();
    if (box && box->init(delegate)) {
        box->t_delegate = delegate;
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

bool SGBMatchStartBox::init(SGBoxDelegate *delegate)
{
    if (!SGBaseBox::init(delegate, box_matchStart, CCSizeMake(530, 340))) {
        return false;
    }
    return true;
}

void SGBMatchStartBox::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_bMatchStartBox);
  
    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("upwar_box_match1.png");
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_BMatchStartBox_str1, FONT_PANGWA, 40 ,ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc  merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(ccpAdd(biao->getPosition(), ccp(-TITLEGAP,0)) );
    this->addChild(title);
    
    //////////////////动态点
    m_showDotCount = 0;
    
    titleDot1 = SGCCLabelTTF::create(" .", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //titleDot1->setOutSideColor(ccc3(143,87,0));
    titleDot1->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width/2 + 20, 0)));
    this->addChild(titleDot1);
    
    titleDot2 = SGCCLabelTTF::create(" .", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //titleDot2->setOutSideColor(ccc3(143,87,0));
    titleDot2->setPosition(ccpAdd(titleDot1->getPosition(), ccp(30, 0)));
    this->addChild(titleDot2);
    
    titleDot3 = SGCCLabelTTF::create(" .", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //titleDot3->setOutSideColor(ccc3(143,87,0));
    titleDot3->setPosition(ccpAdd(titleDot2->getPosition(), ccp(30, 0)));
    this->addChild(titleDot3);
    
    titleDot1->setVisible(false);
    titleDot2->setVisible(false);
    titleDot3->setVisible(false);
    
    schedule(schedule_selector(SGBMatchStartBox::showTitleDot), DOTREPEATTIME);

    //////////////////动态点
    
//    CCSprite *titleDesc = CCSprite::createWithSpriteFrameName("upwar_font_mt1.png");
//    titleDesc->setPosition(ccpAdd(title->getPosition(), ccp(0, -55)));
//    this->addChild(titleDesc);
//    titleDesc->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 1)));
    
    float fontSize = 26;
    const char *fontName = FONT_PANGWA;
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(str_BMatchStartBox_str2, fontName, fontSize);
    label1->setAnchorPoint(ccp(0, 0.5f));
    label1->setPosition(ccpAdd(ccpAdd(title->getPosition(), ccp(TITLEGAP, -80)), ccp(-boxSize.width*.42f, 0)));
    this->addChild(label1);
    SGCCLabelTTF *label2 = SGCCLabelTTF::create(str_BMatchStartBox_str3, fontName, fontSize,ccYELLOW);
    //modify by:zyc. merge into create.
    //label2->setColor(ccYELLOW);
    label2->setAnchorPoint(ccp(0, 0.5f));
    label2->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width, 0)));
    this->addChild(label2);
    SGCCLabelTTF *label3 = SGCCLabelTTF::create(str_BMatchStartBox_str4, fontName, fontSize);
    label3->setAnchorPoint(ccp(0, 0.5f));
    label3->setPosition(ccpAdd(label2->getPosition(), ccp(label2->getContentSize().width, 0)));
    this->addChild(label3);
    SGCCLabelTTF *label4 = SGCCLabelTTF::create(str_BMatchStartBox_str5, fontName, fontSize);
    label4->setAnchorPoint(ccp(0, 0.5f));
    label4->setPosition(ccpAdd(label1->getPosition(), ccp(0, -50)));
    this->addChild(label4);
    
    const char *desc2 = str_BMatchStartBox_str6;
    SGCCLabelTTF *labelTip = SGCCLabelTTF::create(desc2, FONT_BOXINFO, 28);
    labelTip->setPosition(ccpAdd(label1->getPosition(), ccp(labelTip->getContentSize().width/2, -100)));
    this->addChild(labelTip);
    
    SGButton *hideBtn = SGButton::create("box_btn_newbg.png",
                                         "upwar_button_match2.png",
                                         this,
                                         menu_selector(SGBMatchStartBox::hideBoxAction),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(hideBtn);
    hideBtn->setPosition(ccpAdd(center, ccp(-hideBtn->getContentSize().width*.75f, -size.height/2+40)));
    
    SGButton *stopBtn = SGButton::create("box_btn_newbg.png",
                                         "upwar_button_match1.png",
                                         this,
                                         menu_selector(SGBMatchStartBox::stopMatchAction),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(stopBtn);
    stopBtn->setPosition(ccpAdd(center, ccp(stopBtn->getContentSize().width*.75f, -size.height/2+40)));
}
void SGBMatchStartBox::showTitleDot()
{
    titleDot1->setVisible(m_showDotCount == 1||titleDot1->isVisible());
    titleDot2->setVisible(m_showDotCount == 2||titleDot2->isVisible());
    titleDot3->setVisible(m_showDotCount == 3||titleDot3->isVisible());
    
    m_showDotCount++;
    if (m_showDotCount > 4)
    {
        titleDot1->setVisible(false);
        titleDot2->setVisible(false);
        titleDot3->setVisible(false);
        m_showDotCount = 0;
    }
    
    
}

void SGBMatchStartBox::stopMatchAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PvpExitApplyRequest *request = new main::PvpExitApplyRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_SEASONWAR_SAPPLY, request, false);
    
    SGMainManager::shareMain()->getMainScene()->showPvpMatch(false);
    
    this->resetMainTouch(true);
    SGBaseBox::boxClose();
    
}

void SGBMatchStartBox::hideBoxAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->getMainScene()->showPvpMatch(true);
    SGBaseBox::boxClose();
    SGPlayerInfo::sharePlayerInfo()->setIspvp(true);
    this->resetMainTouch(true);
}

void SGBMatchStartBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch,false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}