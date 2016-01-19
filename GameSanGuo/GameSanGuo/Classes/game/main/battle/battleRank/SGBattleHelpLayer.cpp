//
//  SGBattleHelpLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-20.
//
//

#include "SGBattleHelpLayer.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGTestSwitch.h"
#include "SGRichLabel.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

using namespace std;

SGBattleHelpBox::SGBattleHelpBox(void)
:t_delegate(NULL)
{
    
}
SGBattleHelpBox::~SGBattleHelpBox(void)
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName("upwar/upwar.plist");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_battleHelp);
}

SGBattleHelpBox* SGBattleHelpBox::create(SGBoxDelegate *delegate)
{
    SGBattleHelpBox *box = new SGBattleHelpBox();
    if (box && box->init(delegate, box_help, CCSizeMake(skewing(270), skewing(300)))) {
        box->t_delegate = delegate;
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGBattleHelpBox::initView()
{
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("upwar/upwar.plist");
      ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_box_battleHelp);
      ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_box_battleHelp, LIM_PNG_AS_PNG);
    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    
    CCSprite *title = CCSprite::createWithSpriteFrameName("upwar_box_match1.png");
    title->setPosition(ccpAdd(center, ccp(0, size.height*0.5f - 55)));
    this->addChild(title);
    
    const char *desc1 = str_BattleHelpLayer_str1;
    CCLabelTTF *label1 = CCLabelTTF::create(desc1, FONT_XINGKAI, FONTSIZE(16));
    label1->setHorizontalAlignment(kCCTextAlignmentLeft);
    label1->setDimensions(CCSizeMake(size.width-skewing(6), skewing(250)));
    label1->setPosition(ccpAdd(title->getPosition(), ccp(skewing(6)/2, -skewing(250)/2-skewing(15))));
//    this->addChild(label1);
    
    const char *descc = str_BattleHelpLayer_str2;
    const char *ddd = str_BattleHelpLayer_str3;
    
    CCSprite *label = SGRichLabel::getFomLabel(descc, CCSizeMake(size.width-skewing(75), skewing(230)), FONT_XINGKAI, FONTSIZE(14));
    label->setPosition(ccpAdd(title->getPosition(), ccp(skewing(6)/2, -skewing(250)/2-skewing(20))));
    this->addChild(label);
    SGRichLabel::setString(ddd, label);
    
    float posX = -size.width/2 + skewing(5);
    //返回按钮
    SGButton *backButton = SGButton::create("public_btn_6.png",
                                            "upwar_button_match1.png", this,
                                            menu_selector(SGBattleHelpBox::backHandler),
                                            ccp(0, 0), true, false);
    backButton->setScale(1.2f);
    backButton->setPosition(ccpAdd(center, ccp(posX, size.height*0.5f - 50)));
    backButton->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(backButton);
}


void SGBattleHelpBox::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
}

void SGBattleHelpBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
}