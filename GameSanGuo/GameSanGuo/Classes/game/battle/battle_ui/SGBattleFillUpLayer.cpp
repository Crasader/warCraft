//
//  SGBattleFillUpLayer.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-5.
//
//

#include "SGBattleFillUpLayer.h"
#include "SGSkillManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"


#define ROUND_PROMPT_W 410
#define ROUND_PROMPT_H 250

SGBattleFillUpLayer::SGBattleFillUpLayer()
{
    
}

SGBattleFillUpLayer::~SGBattleFillUpLayer()
{
    CCLOG("~SGBattleFillUpLayer()");
}

SGBattleFillUpLayer *SGBattleFillUpLayer::create(SGBoxDelegate *delegate)
{
    SGBattleFillUpLayer *fillUpLayer = new SGBattleFillUpLayer();
    if (fillUpLayer &&
        fillUpLayer->init(delegate , box_fillUp , CCSizeMake(530, 260), false)) {
        
        fillUpLayer->initLayer();
        fillUpLayer->autorelease();
        
        return fillUpLayer;
    }
    
    CC_SAFE_DELETE(fillUpLayer);
    return fillUpLayer;
}

void SGBattleFillUpLayer::initLayer()
{
    int w = this->getContentSize().width;
    int h = this->getContentSize().height;
    
    CCString *text = CCString::createWithFormat(str_Is_fill_soldier);
    
    CCLabelTTF *label = CCLabelTTF::create(text->getCString(), "", 20, CCSize(ROUND_PROMPT_W * 0.7, ROUND_PROMPT_H * 0.3), kCCTextAlignmentCenter);
    label->setPosition(ccp(w * 0.5, h * 0.5));
    this->addChild(label);
    
    SGButton *Ok = SGButton::create("public_btn_5.png", "public_font_queding.png", this, menu_selector(SGBattleFillUpLayer::buttonOk), CCPointZero, false, true);
    SGButton *close = SGButton::create("public_btn_4.png", "public_font_qixiao.png", this, menu_selector(SGBattleFillUpLayer::buttonClose),CCPointZero,false,true);
    
    this->addBtn(Ok);
    this->addBtn(close);
    
    Ok->setPosition(ccp(w * 0.35, h * 0.4));
    close->setPosition(ccp(w * 0.65, h * 0.4));
}

void SGBattleFillUpLayer::buttonOk()
{
    EFFECT_PLAY(MUSIC_BTN);
    setState(1);
    this->boxClose();
}

void SGBattleFillUpLayer::buttonClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    setState(0);
    this->boxClose();
}

void SGBattleFillUpLayer::setState(int num){
    SGSkillManager::shareSkill()->sendSkillRequest(num);
}


